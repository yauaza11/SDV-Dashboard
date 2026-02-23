#include <cstdlib>
#include <iostream>

int main()
{
// 파이 송신 명령어 수정
std::string command = "ffmpeg -f v4l2 -i /dev/video0 "
                      "-s 480x320 -r 60 "
                      "-vcodec libx264 -preset ultrafast -tune zerolatency "
                      "-b:v 1500k -maxrate 1500k -bufsize 750k " // 1.5Mbps로 다이어트
                      "-g 60 " // 키프레임 간격을 조절해 데이터 구조 안정화
                      "-f mpegts \"udp://192.168.137.1:5000?pkt_size=1316\"";


    std::cout <<"Start Video Streaming" << std::endl;
    int result = std::system(command.c_str());
    return result;
}