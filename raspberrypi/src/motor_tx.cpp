#include "motor_tx.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>

MotorTransmitter::MotorTransmitter(int port) : port(port) {
    udp_sock = -1;
    can_sock = -1;
}

MotorTransmitter::~MotorTransmitter() {
    if (udp_sock >= 0) close(udp_sock);
    if (can_sock >= 0) close(can_sock);
}

bool MotorTransmitter::init() {
    // 1. UDP 소켓 설정
    if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket creation failed");
        return false;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    if (bind(udp_sock, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("UDP bind failed");
        return false;
    }

    // 2. CAN 소켓 설정
    struct ifreq ifr;
    if ((can_sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("CAN Socket creation failed");
        return false;
    }

    std::strcpy(ifr.ifr_name, "can0");
    ioctl(can_sock, SIOCGIFINDEX, &ifr);

    can_addr.can_family = AF_CAN;
    can_addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(can_sock, (const struct sockaddr *)&can_addr, sizeof(can_addr)) < 0) {
        perror("CAN bind failed");
        return false;
    }

    frame.can_dlc = 1; // 데이터 길이는 1바이트 고정
    std::cout << "Motor Transmitter (Middleware) Ready on Port " << port << std::endl;
    return true;
}

void MotorTransmitter::handleCommand(const std::string& msg) {
    // 수형 님의 기존 제어 로직
    if (msg == "ACCEL_ON\n") {
        frame.can_id = 0x101;
        frame.data[0] = 0x01;
    } else if (msg == "ACCEL_OFF\n") {
        frame.can_id = 0x101;
        frame.data[0] = 0x00;
    } else if (msg == "BRAKE_ON\n") {
        frame.can_id = 0x102;
        frame.data[0] = 0x01;
    } else if (msg == "BRAKE_OFF\n") {
        frame.can_id = 0x102;
        frame.data[0] = 0x00;
    } else {
        return; // 정의되지 않은 명령은 무시
    }

    write(can_sock, &frame, sizeof(struct can_frame));
    std::cout << "Sent CAN ID: 0x" << std::hex << frame.can_id 
              << " Data: 0x" << (int)frame.data[0] << std::dec << std::endl;
}

void MotorTransmitter::run() {
    char buffer[1024];
    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(udp_sock, (char*)buffer, 1024, 0, (struct sockaddr *)&cliaddr, &len);
        if (n > 0) {
            buffer[n] = '\0';
            std::string msg(buffer);
            std::cout << "Received UDP: " << msg;
            handleCommand(msg);
        }
    }
}