#ifndef MOTOR_TX_HPP
#define MOTOR_TX_HPP

#include <linux/can.h>
#include <netinet/in.h>
#include <string>

class MotorTransmitter {
public:
    MotorTransmitter(int port);
    ~MotorTransmitter();

    bool init();   // UDP 및 CAN 소켓 초기화
    void run();    // 명령 수신 및 CAN 전송 루프

private:
    // UDP 관련
    int udp_sock;
    struct sockaddr_in servaddr, cliaddr;
    int port;

    // CAN 관련
    int can_sock;
    struct sockaddr_can can_addr;
    struct can_frame frame;

    // 내부 처리 함수 (명령어 해석 및 전송)
    void handleCommand(const std::string& msg);
};

#endif