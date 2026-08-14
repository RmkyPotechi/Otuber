#pragma once

#include "camera_tracker.hpp"
#include "landmarks.hpp"

namespace otuber::motion {

class FaceTracker {
public:
    virtual ~FaceTracker() = default;
    virtual FaceLandmarks detect(const CameraFrame &frame) = 0;
};

} // namespace otuber::motion
