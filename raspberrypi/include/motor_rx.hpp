#ifndef MOTOR_RX_HPP
#define MOTOR_RX_HPP

#include <linux/can.h>
#include <netinet/in.h>
#include <string>

class MotorReceiver {
public:
    // 생성자에서 IP와 Port를 받도록 설정
    MotorReceiver(const std::string& ip, int port);
    ~MotorReceiver();

    bool init();   // CAN 및 UDP 소켓 초기화 (기존 main의 설정 부분)
    void run();    // 무한 루프 실행 (기존 main의 while 부분)

private:
    // CAN 관련
    int can_socket;
    struct sockaddr_can can_addr;
    struct can_frame frame;

    // UDP 관련
    int udp_socket;
    struct sockaddr_in qt_addr;
};

#endif