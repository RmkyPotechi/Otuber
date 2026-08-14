#pragma once

#include "core/motion/face_tracker.hpp"

#include <memory>
#include <string>

namespace otuber::windows::tracking {

class OpenCvFaceTracker final : public otuber::motion::FaceTracker {
public:
    // cascade_path should point to OpenCV's haarcascade_frontalface_default.xml.
    explicit OpenCvFaceTracker(std::string cascade_path);
    ~OpenCvFaceTracker() override;

    bool ready() const;
    otuber::motion::FaceLandmarks detect(const otuber::motion::CameraFrame &frame) override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace otuber::windows::tracking
