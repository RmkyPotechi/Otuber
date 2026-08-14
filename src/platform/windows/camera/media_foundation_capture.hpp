#pragma once

#include "core/motion/camera_tracker.hpp"

#include <cstdint>
#include <string>
#include <vector>

#ifdef _WIN32
struct IMFSourceReader;
struct IMFMediaSource;
#endif

namespace otuber::windows::camera {

struct CapturedFrame {
    int width = 0;
    int height = 0;
    std::vector<std::uint8_t> rgba;
};

class MediaFoundationCapture {
public:
    ~MediaFoundationCapture();
    bool initialize();
    bool open(int device_index = 0);
    void shutdown();
    bool read(CapturedFrame &frame);
    bool is_open() const { return opened_; }
    const std::string &last_error() const { return last_error_; }

private:
    void set_error(const char *message);
#ifdef _WIN32
    void set_hresult_error(const char *operation, long hr);
#endif

    bool initialized_ = false;
    bool opened_ = false;
#ifdef _WIN32
    bool com_initialized_ = false;
    IMFSourceReader *reader_ = nullptr;
    IMFMediaSource *source_ = nullptr;
#endif
    int width_ = 0;
    int height_ = 0;
    std::string last_error_;
};

} // namespace otuber::windows::camera
