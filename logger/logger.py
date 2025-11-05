import serial, csv
from datetime import datetime

# 포트명은 실제 ESP32 연결 포트로 변경
ser = serial.Serial('/dev/tty.usbserial-1410', 115200)
filename = f"microgrid_log_{datetime.now().strftime('%Y%m%d_%H%M')}.csv"

with open(filename, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow([
    "timestamp(s)",
    "pv_v(V)",
    "pv_i(mA)",
    "batt_v(V)",
    "batt_i(mA)",
    "bus_v(V)",
    "bms_v(V)",
    "bms_i(A)",
    "bms_soc(%)"])
    print(f"▶ Logging started → {filename}")

    while True:
        line = ser.readline().decode('utf-8').strip()
        if not line or "timestamp" in line:
            continue
        data = line.split(',')
        if len(data) == 9:
            writer.writerow(data)
            print(data)