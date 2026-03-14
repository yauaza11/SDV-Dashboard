# SDV 통합 제어 플랫폼 (SDV-Centralized-Control-Platform)
하드웨어가 소프트웨어에 의해 정의되는 SDV(Software Defined Vehicle) 구조를 기반으로 사용자 애플리케이션(Qt) - 게이트웨이(Raspberry Pi) - 구동 제어기(STM32)를 통합한 중앙 집중형 제어 시스템입니다.

## 1. 프로젝트 개요 (Overview)
본 프로젝트는 현대 자동차 산업의 중앙 집중형 아키텍처를 모사하여, 상위 레이어의 소프트웨어 명령이 이기종 네트워크를 거쳐 실제 하드웨어 구동까지 전달되는 전 과정을 구현했습니다. 단순히 모터를 구동하는 것에 그치지 않고, 실시간 영상 스트리밍과 주행 제어 데이터가 공존하는 환경에서의 통신 신뢰성을 검증하는 데 목적이 있습니다.

## 2. 주요 기능 (Features)
중앙 집중형 통합 제어: Qt 앱을 통한 실시간 가속/제동 제어 및 차량 상태 모니터링

멀티 프로토콜 게이트웨이: Ethernet(UDP/IP)과 CAN 간의 실시간 프로토콜 라우팅 최적화

정밀 속도 제어: 엔코더 피드백 기반 PID 알고리즘 구현 (목표 RPM 대비 오차율 5% 이내)

실시간 데이터 로깅: 주행 데이터를 CSV 형식으로 저장하여 제어 성능 정량 분석

영상 관제: CSI 카메라를 이용한 저지연 UDP 영상 스트리밍

## 3. 시스템 아키텍처 (System Architecture)
Application Layer (PC): C++/Qt 기반 GUI, 멀티쓰레드 데이터 처리

Middleware Layer (Raspberry Pi 4): Linux 기반 SocketCAN-UDP 데이터 중계 (Gateway)

Control Layer (STM32): HAL 라이브러리 기반 모터 제어 및 센서 데이터 수집

## 4. 기술 스택 (Tech Stack)
언어: C, C++

프레임워크: Qt, STM32 HAL

통신: CAN, UDP/IP, I2C, UART, ADC

하드웨어: STM32F103, Raspberry Pi 4, CSI Camera, L298N Motor Driver

## 5. 핵심 성과 및 문제 해결 (Key Achievements)
통신 실시간성 확보: 고대역폭 영상 스트리밍 환경에서도 제어 신호의 지연 없는 전송로 설계

데이터 기반 제어 최적화: 실시간 로그 분석을 통해 PID 게인을 정밀 튜닝하여 제어 결정성 확보

풀스택 통합 역량: 로우레벨 HW 인터페이스부터 하이레벨 앱까지 전체 시스템 파이프라인 구축
