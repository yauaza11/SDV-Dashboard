#include "video_streamer.hpp"
#include <iostream>
#include <cstdlib>

// 클래스 명시(VideoStreamer::)가 빠지면 링커가 못 찾습니다!
VideoStreamer::VideoStreamer(const std::string& ip, int port) 
    : target_ip(ip), target_port(port) {}

void VideoStreamer::start() {
    std::string command = "ffmpeg -f v4l2 -i /dev/video0 "
                          "-s 480x320 -r 60 "
                          "-vcodec libx264 -preset ultrafast -tune zerolatency "
                          "-b:v 1500k -maxrate 1500k -bufsize 750k "
                          "-g 60 "
                          "-f mpegts \"udp://" + target_ip + ":" + std::to_string(target_port) + "?pkt_size=1316\"";

    std::cout << "Starting Video Streaming..." << std::endl;
    std::system(command.c_str());
}