# STM32F746VG MCU를 사용한 임베디드 시스템 프로젝트

이 리포지토리에는 STM32F746VG 마이크로컨트롤러 유닛(MCU)을 사용하여 개발한 임베디드 시스템 프로젝트가 포함되어 있습니다. 
이 프로젝트는 특정 작업을 수행하기 위해 MCU의 다양한 기능과 주변 장치를 활용합니다.
해당 프로젝트에서는 서브 개발자로 참여하였으며, 이후 개발에 관리 및 유지 보수 담당하였습니다.
이 후 추가 되는 프로젝트 부트로더 기능 추가를 진행하였습니다.

## 기능

- ADC(아날로그-디지털 컨버터): 아날로그 센서 입력에 활용됩니다.
- DAC(디지털-아날로그 변환기): 아날로그 출력 신호 생성에 사용됩니다.
- SDCARD: 데이터 저장을 위해 SD 카드에서 읽고 쓸 수 있습니다.
- 이더넷: 네트워크 연결을 위한 이더넷 통신을 구현합니다. (외부 연결)
- GPIO(범용 입력/출력): 범용 디지털 입출력 작업에 사용됩니다. (스위치 입력에 사용)
- I2C(집적 회로): I2C 호환 장치와 통신할 수 있습니다. (입자 감지 센서와의 데이터 통신에 사용)
- ILI9341 LCD: 시각적 출력을 위해 ILI9341 LCD 디스플레이를 활용합니다. (외부 LCD로 시각적 화면 출력 320x240)
- RTC(실시간 시계): 정확한 타임키핑 기능을 제공합니다. (현재 시간 설정)
- 타이머: 타이밍 및 주기적 인터럽트 생성에 활용됩니다. (내부 MCU 타이머)
- UART(범용 비동기 수신기-송신기): 직렬 통신을 구현합니다.

## 시작하기

이 임베디드 시스템 프로젝트를 시작하려면 다음 단계를 따르세요:

1. 이 리포지토리를 로컬 컴퓨터에 복제합니다.
2. 필요한 툴체인과 IDE로 개발 환경을 설정합니다.
3. 개발 보드 또는 맞춤형 하드웨어 설정에 STM32F746VG MCU를 연결합니다.
4. 요구 사항에 따라 필요한 주변 장치와 핀을 구성합니다.
5. 프로젝트를 빌드하고 펌웨어를 MCU에 플래시합니다.
6. 임베디드 시스템을 실행하고 기능을 관찰합니다.

## 라이선스

이 프로젝트는 MIT 라이선스에 따라 라이선스가 부여됩니다. 자세한 내용은 [LICENSE](/LICENSE) 파일을 참조하세요.
