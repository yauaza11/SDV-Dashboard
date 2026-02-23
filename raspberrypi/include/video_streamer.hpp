#ifndef VIDEO_STREAMER_HPP
#define VIDEO_STREAMER_HPP

#include <string>

class VideoStreamer {
public:
    // 생성자에서 목적지 IP와 포트를 받도록 설정
    VideoStreamer(const std::string& ip, int port);
    
    // 스트리밍 시작 (기존 main의 로직)
    void start();

private:
    std::string target_ip;
    int target_port;
};

#endif