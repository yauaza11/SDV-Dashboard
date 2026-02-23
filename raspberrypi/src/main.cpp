#include <thread>
#include <vector>
#include "motor_rx.hpp"
#include "motor_tx.hpp"
#include "video_streamer.hpp"

int main() {
    // 1. 객체 생성
    MotorReceiver rx("192.168.137.1", 12345); // CAN -> UDP(PC)
    MotorTransmitter tx(8888);               // UDP(PC) -> CAN
    VideoStreamer video("192.168.137.1", 5000); // Camera -> UDP(PC)

    // 2. 초기화 확인
    if (!rx.init() || !tx.init()) {
        return -1;
    }

    // 3. 각 모듈을 쓰레드로 분리해서 실행 (동시 실행!)
    // Video는 system()을 호출하므로 반드시 별도 쓰레드 필요
    std::thread v_thread(&VideoStreamer::start, &video);
    
    // RX (데이터 수신 및 전달)
    std::thread rx_thread(&MotorReceiver::run, &rx);
    
    // TX (명령어 수신 및 전달) - 메인 쓰레드에서 실행하거나 별도 쓰레드
    std::thread tx_thread(&MotorTransmitter::run, &tx);

    // 쓰레드가 종료될 때까지 대기
    v_thread.join();
    rx_thread.join();
    tx_thread.join();

    return 0;
}