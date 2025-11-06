# Control and Operation Analysis of a DC Standalone Microgrid System
> DC 독립형 마이크로그리드의 제어 및 동작 특성 연구  
> by **Hunhui Kim** (Energy & Electrical Engineering, TUK)
    **Sumin Lee** (Energy & Electrical Engineering, TUK)
    **Kyungmo Park** (Energy & Electrical Engineering, TUK)
    **Byunghoon Lee** (Energy & Electrical Engineering, TUK)
> 지도교수: **Prof. Paul Jang**

---

## 1. Project Overview

본 연구는 **DC 독립형 마이크로그리드 시스템의 제어 및 동작 특성**을 실험적으로 검증하기 위해 수행.  
태양광 발전, 배터리 저장, 부하 제어의 상호작용을 ESP32 기반 제어 시스템으로 구현하고,  
센서 데이터 및 기상 데이터를 통합하여 **전력 흐름 안정성**과 **제어 응답성**을 분석하였음.

---

## 2. System Configuration

### Hardware Components
- **ESP32** - 주제어 MCU, BLE 통신 및 센서 데이터 수집
- **JBD BMS (ACEY-SP04S010)** - 배터리 보호 및 상태 관리 (BLE 통신)
- **PV Module (12V x2)** - 태양광 발전 소스
- **MPPT Converter** - 최대전력점추종 및 전압 안정화
- **Relay Module (4ch)** - PV-Battery 전환 및 보호 제어
- **INA219 Sensor** - 전류 및 전압 측정
- **LED Load (1.44W)** - 고정 부하

### Software Stack
| 구성요소 | 설명 |
|-----------|------|
| **ESP32 Firmware** | PV/배터리 제어 및 BLE-BMS 통신 (`src/main.cpp`) |
| **Python Logger** | ESP32 시리얼 로그 수집 및 CSV 저장 (`logger/logger.py`) |
| **KMA API Script** | 기상 데이터 자동 수집 (`logger/KMA_API.py`) |
| **Analysis Notebook** | PV-GHI 상관분석, SOC 변화 그래프 (`analysis/`) |

---

## 3. Data Description

### Data Flow
```
BMS  ←(BLE)→  ESP32  ←(Serial)→  Python Logger  →  CSV (microgrid_log_YYYYMMDD.csv)
                                                ↓
                                                 + KMA Weather Data (KMA_API.py)
                                                ↓
                                                 → Merged Dataset for Analysis
```

### Weather Data
본 연구에 사용된 기상 데이터는 **기상청(KMA) 공개자료**를 기반으로 하였으며,  
실제 1시간 단위 자료를 시뮬레이션 목적에 맞게 **1초 단위로 보간 및 확률적 변동을 추가**하여 구성.  

> **기상자료 출처:** [기상청 기상자료개방포털](https://data.kma.go.kr)

### 주요 데이터셋
| 파일명 | 설명 |
|--------|------|
| `microgrid_log_20250928.csv` | ESP32 로그 데이터 (전압·전류·SOC) |
| `microgrid_weather_subset.csv` | 기상 데이터 (기온·일사량·하늘상태 등) |
| `microgrid_merged.csv` | 병합 데이터셋 (기상+전력 흐름) |
| `microgrid_merged_weather_removed.csv` | 분석용 정제 데이터셋 |

---

## 4. Repository Structure
```
Microgrid_TUK/
├── src/                  # ⚙️ ESP32 펌웨어 (main.cpp)
│   └── main.cpp
│
├── logger/               # 🧾 데이터 수집 (Python scripts)
│   ├── logger.py
│   └── KMA_API.py
│
├── data/                 # 수집 및 가공된 CSV 데이터
│   ├── microgrid_log_20250928_1817.csv
│   ├── microgrid_weather_subset.csv
│   └── microgrid_merged.csv
│
├── analysis/             #� 데이터 분석 코드 (Jupyter / Python)
│   ├── microgrid_analysis.ipynb
│   └── PV_vs_GHI_plot.py
│
├── docs/                 # 논문 및 이미지 자료
│   ├── thesis_draft.md
│   ├── figures/
│   │   ├── Fig1_PV_Voltage_Current.png
│   │   ├── Fig2_Battery_Voltage_Current.png
│   │   ├── Fig3_Bus_Voltage_Stability.png
│   │   └── Fig4_Load_Response.png
│   └── README.md
│
└── README.md             # 프로젝트 개요 (현재 파일)
```
---

## 5. How to Run

### (1) ESP32 펌웨어 업로드
```bash
# PlatformIO (VSCode)
pio run --target upload
```

### (2) 실험 데이터 로깅
```bash
cd logger
python logger.py
```
→ data/microgrid_log_YYYYMMDD.csv 자동 생성

### (3) 기상 데이터 수집
```bash
python KMA_API.py
```
→ data/microgrid_weather_subset.csv 생성

### (4) 데이터 분석
```bash
jupyter notebook analysis/microgrid_analysis.ipynb
```

---

## 6. Figures (Representative Results)

### Fig. 1. PV Voltage & Current
![Fig.1 PV Voltage & Current](docs/figures/Fig.1_PV_Module_Voltage_and_Current_Characteristics.png)

### Fig. 2. Battery Voltage & Current
![Fig.2 Battery Voltage & Current](docs/figures/Fig.2_Battery_Module_Voltage_and_Current_Characteristics.png)

### Fig. 3. Bus Voltage Stability
![Fig.3 Bus Voltage Stability](docs/figures/Fig.3_Comparison_of_Bus_Voltage_Stability.png)

### Fig. 4. Load Response
![Fig.4 Load Response](docs/figures/Fig.4_Load_Response_According_to_Operating_Time_Sessions.png)

---

## 7. Author & Acknowledgements

	•	Author: Sumin Lee (이수민), Dept. of Energy & Electrical Engineering, TUK
                Kyungmo Park (박경모) , Dept. of Energy & Electrical Engineering, TUK
                Byunghoon Lee (이병훈) , Dept. of Energy & Electrical Engineering, TUK
                Hunhui Kim (김훈희) , Dept. of Energy & Electrical Engineering, TUK
	•	Advisor: Prof. Paul Jang
	•	Project Period: 2024.09 - 2025.11
	•	Keywords: DC Standalone Microgrid, Photovoltaic Generation, MPPT(Maximum Power Point Tracking), Battery Charge and Discharge Control, Bus Voltage Regulation, ESP32-based Control

---

## 8. Citation

    Hunhui, Kim. Lee, Sumin. Kyungmo, Park. Byunghoon, Lee. “Control and Operation Analysis of a DC Standalone Microgrid System.”
    TUK, Department of Energy & Electrical Engineering, 2025.

---

## 9. License

© 2025 Team PPT.  
This code and data are part of the TUK Graduation Project and are not licensed for redistribution.  
For academic or educational use, please contact the author.

---

## 10. Repository

GitHub: https://github.com/sepgom/Microgrid_TUK￼
