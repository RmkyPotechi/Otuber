#include "opencv_face_tracker.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <algorithm>
#include <cmath>

namespace otuber::windows::tracking {

struct OpenCvFaceTracker::Impl {
    cv::CascadeClassifier face;
    bool ready = false;
};

OpenCvFaceTracker::OpenCvFaceTracker(std::string cascade_path)
    : impl_(std::make_unique<Impl>())
{
    impl_->ready = impl_->face.load(cascade_path);
}

OpenCvFaceTracker::~OpenCvFaceTracker() = default;

bool OpenCvFaceTracker::ready() const
{
    return impl_ && impl_->ready;
}

otuber::motion::FaceLandmarks OpenCvFaceTracker::detect(const otuber::motion::CameraFrame &frame)
{
    otuber::motion::FaceLandmarks result{};
    if (!ready() || frame.width <= 0 || frame.height <= 0 || frame.pixels == nullptr)
        return result;

    cv::Mat rgba(frame.height, frame.width, CV_8UC4,
                 const_cast<unsigned char *>(frame.pixels));
    cv::Mat gray;
    cv::cvtColor(rgba, gray, cv::COLOR_RGBA2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces;
    impl_->face.detectMultiScale(gray, faces, 1.1, 5, 0, cv::Size(80, 80));
    if (faces.empty())
        return result;

    const auto largest = std::max_element(faces.begin(), faces.end(),
        [](const cv::Rect &a, const cv::Rect &b) { return a.area() < b.area(); });
    const cv::Rect face = *largest;

    const float fx = static_cast<float>(face.x) / frame.width;
    const float fy = static_cast<float>(face.y) / frame.height;
    const float fw = static_cast<float>(face.width) / frame.width;
    const float fh = static_cast<float>(face.height) / frame.height;

    // Compact landmark approximation. This backend deliberately exposes the
    // same normalized FaceLandmarks contract as a future dense landmark model.
    result.detected = true;
    result.nose = {fx + fw * 0.50f, fy + fh * 0.56f};
    result.left_eye = {fx + fw * 0.31f, fy + fh * 0.39f};
    result.right_eye = {fx + fw * 0.69f, fy + fh * 0.39f};
    result.mouth_left = {fx + fw * 0.35f, fy + fh * 0.72f};
    result.mouth_right = {fx + fw * 0.65f, fy + fh * 0.72f};
    result.mouth_top = {fx + fw * 0.50f, fy + fh * 0.69f};
    result.mouth_bottom = {fx + fw * 0.50f, fy + fh * 0.78f};
    return result;
}

} // namespace otuber::windows::tracking
