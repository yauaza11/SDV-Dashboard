cmake_minimum_required(VERSION 3.10)
project(SDV_2026_soo)

# 1. C++ 표준 및 Qt 자동화 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)

# Qt 전용 코드 생성 도구 활성화 (Qt의 핵심!)
set(CMAKE_AUTOMOC ON) # Signal & Slot 처리를 위해 필수
set(CMAKE_AUTOUIC ON) # .ui 파일을 코드로 바꾸기 위해 필수
set(CMAKE_AUTORCC ON) # 이미지 등 리소스를 위해 필수


# file(GLOB_RECURSE SOURCES "src/*.cpp")
set(SOURCES 
    src/main.cpp    
    src/motor_tx.cpp
    src/motor_rx.cpp
    src/video_streamer.cpp
)

add_executable(sdv_core ${SOURCES})

target_link_libraries(sdv_core Qt5::Widgets ${OpenCV_LIBRARIES})

add_custom_target(setup
    COMMAND sudo ip link set can0 up type can bitrate 500000
    COMMAND sudo ifconfig eth0 192.168.137.2 netmask 255.255.255.0 up
)

# 빌드가 성공하면 자동으로 안내 메시지 출력
add_custom_command(TARGET sdv_core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "빌드 완료! 실행 전 'make setup'으로 네트워크를 설정하세요."
)