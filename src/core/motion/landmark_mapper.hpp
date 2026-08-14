#pragma once

#include "camera_state.hpp"
#include "landmarks.hpp"

namespace otuber::motion {

class LandmarkMapper {
public:
    CameraState map(const FaceLandmarks &landmarks) const;
};

} // namespace otuber::motion
