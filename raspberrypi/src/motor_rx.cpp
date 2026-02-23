#include "motor_rx.hpp"
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>

MotorReceiver::MotorReceiver(const std::string& ip, int port) {
    memset(&qt_addr, 0, sizeof(qt_addr));
    qt_addr.sin_family = AF_INET;
    qt_addr.sin_port = htons(port);
    qt_addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

MotorReceiver::~MotorReceiver() {
    close(can_socket);
    close(udp_socket);
}

bool MotorReceiver::init() {
    // 1. CAN 소켓 설정
    struct ifreq ifr;
    if((can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("CAN Socket Error");
        return false;
    }

    strcpy(ifr.ifr_name, "can0");
    ioctl(can_socket, SIOCGIFINDEX, &ifr);
    can_addr.can_family = AF_CAN;
    can_addr.can_ifindex = ifr.ifr_ifindex;
    
    if(bind(can_socket, (struct sockaddr *)&can_addr, sizeof(can_addr)) < 0) {
        perror("CAN Bind Error");
        return false;
    }

    // 2. UDP 소켓 설정
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    return (udp_socket >= 0);
}

void MotorReceiver::run() {
    std::cout << "Bridge Start: CAN(0x201) -> UDP" << std::endl;
    
    while(true) {
        int nbytes = read(can_socket, &frame, sizeof(struct can_frame));
        
        if(nbytes > 0 && frame.can_id == 0x201) {
            // 비트 연산으로 데이터 복원
            int16_t raw_rpm = (frame.data[0] << 8) | frame.data[1];
            int16_t raw_target = (frame.data[2] << 8) | frame.data[3];

            float current_rpm = (float)raw_rpm / 10.0f;
            float target_rpm = (float)raw_target / 10.0f;
            
            char buffer[64];
            
            sprintf(buffer, "RPM:%.1f,TRG:%.1f", current_rpm, target_rpm);

            sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&qt_addr, sizeof(qt_addr));
            printf("Forwarded to Qt : %s\n", buffer);
        }
    }
}