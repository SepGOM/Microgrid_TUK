#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <NimBLEDevice.h>

Adafruit_INA219 ina219;

// --- BMS BLE 관련 설정 ---
static NimBLEAdvertisedDevice* bmsDevice = nullptr;
NimBLEClient* pClient = nullptr;
String bmsAddress = "A4:C1:37:55:C2:A3";

// BMS 데이터 변수
float bms_voltage = 0.0;
float bms_current = 0.0;
float bms_soc = 0.0;

// --- 핀 및 파라미터 설정 ---
const int analogPin = 35;          // 전압 센서 입력 핀
const int relayPin = 26;           // 릴레이 제어 핀
const float correctionFactor = 5.5; // 전압 보정 계수

// PWM 설정
const int pwmPin = 12;
const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

bool relayState = false;  // 릴레이 상태 기억용

// Soft Start (서서히 PWM 상승)
void softStartPWM(int channel, int maxDuty, float currentThreshold_mA, int stepDelay = 50) {
  for (int duty = 0; duty <= maxDuty; duty++) {
    ledcWrite(channel, duty);
    delay(stepDelay);
    float current_mA = ina219.getCurrent_mA();
    if (current_mA >= currentThreshold_mA) {
      Serial.print("Soft start stopped at duty: ");
      Serial.print(duty);
      Serial.print(", current: ");
      Serial.print(current_mA);
      Serial.println(" mA");
      break;
    }
  }
}

// BLE Notify 콜백 (BMS 데이터 수신 시 호출)
class BMSNotifyCallback : public NimBLENotifyCallback {
  void onNotify(NimBLERemoteCharacteristic* pChr, uint8_t* pData, size_t length, bool isNotify) override {
    // JBD BMS 데이터 포맷 해석
    if (length >= 13 && pData[0] == 0xDD) {
      uint16_t v = (pData[4] << 8) | pData[5];
      uint16_t c = (pData[6] << 8) | pData[7];
      uint8_t soc = pData[23];
      bms_voltage = v / 100.0;
      bms_current = ((int16_t)c) / 100.0;
      bms_soc = soc;
    }
  }
};

// BLE 연결
bool connectToBMS() {
  pClient = NimBLEDevice::createClient();
  NimBLEAddress addr(bmsAddress.c_str());
  if (!pClient->connect(addr)) return false;

  NimBLERemoteService* pService = pClient->getService("0000ff00-0000-1000-8000-00805f9b34fb");
  if (!pService) return false;
  NimBLERemoteCharacteristic* pChar = pService->getCharacteristic("0000ff01-0000-1000-8000-00805f9b34fb");
  if (!pChar) return false;

  pChar->subscribe(true, new BMSNotifyCallback());
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  if (!ina219.begin()) {
    Serial.println("INA219 not found. Check wiring!");
    while (1);
  }

  // PWM 설정 & 릴레이
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(pwmPin, pwmChannel);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // 초기 OFF 상태

  // BLE 초기화
  NimBLEDevice::init("ESP32_BMS_Client");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  connectToBMS();

  delay(1000);

  Serial.println("timestamp,pv_v(V),pv_i(mA),batt_v(V),batt_i(mA),bus_v(V),bms_v(V),bms_i(A),bms_soc(%)");
}

void loop() {
  static unsigned long lastCSV = 0;          
  const unsigned long csvInterval = 10000;   // 테스트용 10초, 실사용 시 60000(1분)

  // --- 아날로그 전압 측정 ---
  int adcValue = analogRead(analogPin);
  float voltage = (adcValue / 4095.0) * 3.3;
  float actualVoltage = voltage * correctionFactor;

  // --- 릴레이 제어 (히스테리시스) ---
  if (actualVoltage > 12.0) {
    if (relayState) {
      digitalWrite(relayPin, HIGH);
      relayState = false;
      Serial.println("Relay: OFF (protection triggered)");
    }
  } else if (!relayState && actualVoltage <= 11.2) {
    digitalWrite(relayPin, LOW);
    relayState = true;
    Serial.println("Relay: ON (normal operation)");
    softStartPWM(pwmChannel, 200, 100.0, 30);
  }

  // --- INA219 측정값 ---
  float bus_v = ina219.getBusVoltage_V();
  float shunt_v = ina219.getShuntVoltage_mV() / 1000.0;
  float current_mA = ina219.getCurrent_mA();
  float pv_v = actualVoltage;
  float pv_i = current_mA;
  float batt_v = bus_v;
  float batt_i = current_mA;

  // --- CSV 데이터 전송 (BMS 포함) ---
  if (millis() - lastCSV >= csvInterval) {
    lastCSV = millis();
    unsigned long timestamp = millis() / 1000; // 초 단위
    Serial.print(timestamp); Serial.print(",");
    Serial.print(pv_v, 2); Serial.print(",");
    Serial.print(pv_i, 2); Serial.print(",");
    Serial.print(batt_v, 2); Serial.print(",");
    Serial.print(batt_i, 2); Serial.print(",");
    Serial.print(bus_v, 2); Serial.print(",");
    Serial.print(bms_voltage, 2); Serial.print(",");
    Serial.print(bms_current, 2); Serial.print(",");
    Serial.println(bms_soc, 1);
  }

  // --- 디버그용 출력 (1초 주기) ---
  Serial.print("Measured Voltage: ");
  Serial.print(actualVoltage, 2);
  Serial.print(" V | Current: ");
  Serial.print(current_mA, 2);
  Serial.println(" mA");

  delay(1000);
}
