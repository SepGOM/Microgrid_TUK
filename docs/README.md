# Microgrid_TUK Documentation

이 폴더는 **DC 독립형 마이크로그리드 제어 시스템** 연구를 위한  
문서 및 참고 자료를 포함하고 있습니다.

---

## 구성

| 파일명 | 설명 |
|--------|------|
| `Control and Operation Analysis of a DC Standalone Microgrid_20251106` | 논문 (Introduction, Method, Results, Discussion 등 포함) |
| `figures/` | 분석 그래프 / 다이얼로그 이미지 |
| `README.md` | 이 문서 폴더의 인덱스 (현재 파일) |

---

## 주요 그래프

- **Fig. 1** PV 전압·전류 곡선  
- **Fig. 2** 배터리 SOC 및 충방전 특성  
- **Fig. 3** 버스 전압 안정성 비교  
- **Fig. 4** 부하 대응성 검증  

모든 그래프는 `analysis/` 폴더에서 생성된 결과를 `docs/figures/`로 복사하여 관리합니다.

---

## 관련 데이터

| CSV 파일 | 설명 |
|-----------|------|
| `microgrid_log_20250928.csv` | ESP32 실험 로그 데이터 |
| `microgrid_weather_subset.csv` | KMA 날씨 데이터 |
| `microgrid_merged.csv` | 병합된 분석용 데이터셋 |

---

## 데이터셋 구조

---

## 작성자 및 버전
- 작성자: **Su Min Lee (Energy & Electrical Engineering, TUK)**, 
        **Kyung Mo Park (Energy & Electrical Engineering, TUK)**
        **Byung Hoon Lee  (Energy & Electrical Engineering, TUK)**
        **Hun Hui Kim (Energy & Electrical Engineering, TUK)**
- 지도교수: **Prof. Paul Jang**
- 버전: v1.0 (2025-11-06)

---
> 본 문서는 GitHub 저장소의 `/docs/` 폴더 내에서  
> 프로젝트 관련 문서를 체계적으로 관리하기 위한 인덱스 파일입니다.