// STM32의 데이터를 받아서
// Qt로 쏴주기 UDP로

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <arpa/inet.h>


int main()
{
    // 1. CAN 소켓 설정
    int can_socket;
    struct sockaddr_can can_addr;
    struct ifreq ifr;
    struct can_frame frame;

    if((can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW))<0){
        perror("CAN Socket Error");
        return 1;
    }

    strcpy(ifr.ifr_name, "can0");
    ioctl(can_socket, SIOCGIFINDEX, &ifr);

    can_addr.can_family = AF_CAN;
    can_addr.can_ifindex = ifr.ifr_ifindex;
    bind(can_socket, (struct sockaddr *)&can_addr, sizeof(can_addr));

    //2 2. UDP 소켓 설정(Qt 전송용)
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in qt_addr;
    memset(&qt_addr, 0, sizeof(qt_addr));
    qt_addr.sin_family = AF_INET;
    qt_addr.sin_port = htons(12345);
    qt_addr.sin_addr.s_addr = inet_addr("192.168.137.1"); // 이거 PC 번호로 바꿔야할듯

    std::cout << "Bridge Start: CAN(0x201) -> UDP(12345)" << std::endl;
    
    while(true){
        // CAN 수신대기
        int nbytes = read(can_socket, &frame, sizeof(struct can_frame));
        // STM32에서 8비트씩 쪼개 보낸것을 다시 합쳐
        if(nbytes > 0 && frame.can_id == 0x201){
            int16_t raw_rpm = (frame.data[0]<<8) | frame.data[1];
            int16_t raw_target = (frame.data[2]<<8) | frame.data[3];

            float current_rpm = (float)raw_rpm/10.0f;
            float target_rpm = (float)raw_target/10.0f;
            
            // UDP 메시지 조립
            char buffer[64];
            sprintf(buffer, "RPM:%.1f, TRG:%.1f", current_rpm, target_rpm);

            // Qt로 전송
            sendto(udp_socket, buffer, strlen(buffer),0, (struct sockaddr*)&qt_addr, sizeof(qt_addr));

            printf("Forwarded to Qt : %s\n", buffer);
        }
    }
    close(can_socket);
    close(udp_socket);
    return 0;
}