#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core/motion/landmark_mapper.hpp"
#include "platform/windows/camera/media_foundation_capture.hpp"
#include "platform/windows/tracking/opencv_face_tracker.hpp"

#include <cstdlib>
#include <cwchar>
#include <memory>
#include <sstream>
#include <string>

namespace {

otuber::windows::camera::MediaFoundationCapture camera;
std::unique_ptr<otuber::windows::tracking::OpenCvFaceTracker> tracker;
otuber::motion::LandmarkMapper mapper;
otuber::motion::CameraState state;
std::wstring status = L"Starting...";

std::string cascade_path()
{
    if (const char *value = std::getenv("OTUBER_FACE_CASCADE"))
        return value;
    return {};
}

void update_tracking(HWND hwnd)
{
    otuber::windows::camera::CapturedFrame frame;
    if (!camera.read(frame)) {
        status = L"Camera frame unavailable";
        InvalidateRect(hwnd, nullptr, FALSE);
        return;
    }

    otuber::motion::CameraFrame core_frame{
        frame.width,
        frame.height,
        frame.rgba.data()
    };

    const auto landmarks = tracker->detect(core_frame);
    state = mapper.map(landmarks);

    std::wostringstream text;
    text << L"Camera: " << frame.width << L"x" << frame.height
         << L"\nFace: " << (landmarks.detected ? L"detected" : L"not detected")
         << L"\nHead X: " << state.head_x
         << L"\nHead Y: " << state.head_y
         << L"\nRotation: " << state.head_rotation
         << L"\nMouth: " << state.mouth_open
         << L"\nEye L: " << state.eye_left_open
         << L"\nEye R: " << state.eye_right_open;
    status = text.str();
    InvalidateRect(hwnd, nullptr, FALSE);
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_CREATE: {
        if (!camera.initialize()) {
            status = L"Media Foundation initialization failed";
            return 0;
        }
        if (!camera.open(0)) {
            status = L"Could not open camera 0";
            return 0;
        }

        const std::string cascade = cascade_path();
        if (cascade.empty()) {
            status = L"Set OTUBER_FACE_CASCADE to haarcascade_frontalface_default.xml";
            return 0;
        }

        tracker = std::make_unique<otuber::windows::tracking::OpenCvFaceTracker>(cascade);
        if (!tracker->ready()) {
            status = L"Could not load OpenCV face cascade";
            return 0;
        }

        SetTimer(hwnd, 1, 33, nullptr);
        status = L"Tracking started";
        return 0;
    }
    case WM_TIMER:
        update_tracking(hwnd);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps{};
        HDC dc = BeginPaint(hwnd, &ps);
        RECT client{};
        GetClientRect(hwnd, &client);
        FillRect(dc, &client, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
        SetBkMode(dc, TRANSPARENT);

        TextOutW(dc, 32, 28, L"Otuber Camera Tracking", 21);
        int y = 70;
        std::wstring line;
        std::size_t start = 0;
        while (start <= status.size()) {
            const std::size_t end = status.find(L'\n', start);
            line = status.substr(start, end == std::wstring::npos ? end : end - start);
            TextOutW(dc, 32, y, line.c_str(), static_cast<int>(line.size()));
            y += 28;
            if (end == std::wstring::npos)
                break;
            start = end + 1;
        }
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        camera.shutdown();
        tracker.reset();
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd, message, wparam, lparam);
    }
}

} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show_command)
{
    const wchar_t class_name[] = L"OtuberTrackingWindow";
    WNDCLASSW wc{};
    wc.lpfnWndProc = window_proc;
    wc.hInstance = instance;
    wc.lpszClassName = class_name;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    if (!RegisterClassW(&wc))
        return 1;

    HWND window = CreateWindowExW(
        0, class_name, L"Otuber Camera Tracking",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        720, 520, nullptr, nullptr, instance, nullptr);
    if (!window)
        return 1;

    ShowWindow(window, show_command);
    UpdateWindow(window);

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}
