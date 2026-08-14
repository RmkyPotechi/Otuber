#include "preview_ui.hpp"

#include <algorithm>
#include <string>

namespace otuber::preview {
namespace {

void text(HDC dc, int x, int y, const std::wstring &value)
{
    TextOutW(dc, x, y, value.c_str(), static_cast<int>(value.size()));
}

void panel(HDC dc, RECT r)
{
    FillRect(dc, &r, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
    FrameRect(dc, &r, static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)));
}

void bar(HDC dc, int x, int y, int width, float value)
{
    RECT bg{x, y, x + width, y + 12};
    FrameRect(dc, &bg, static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)));
    value = std::clamp(value, 0.0f, 1.0f);
    RECT fill{x + 1, y + 1, x + 1 + static_cast<int>((width - 2) * value), y + 11};
    FillRect(dc, &fill, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
}

void draw_rgba(HDC dc, const RECT &dst, const otuber::windows::camera::CapturedFrame &frame)
{
    if (frame.rgba.empty() || frame.width <= 0 || frame.height <= 0)
        return;
    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = frame.width;
    bmi.bmiHeader.biHeight = -frame.height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    StretchDIBits(dc, dst.left, dst.top, dst.right - dst.left, dst.bottom - dst.top,
                  0, 0, frame.width, frame.height, frame.rgba.data(), &bmi,
                  DIB_RGB_COLORS, SRCCOPY);
}

void draw_avatar_image(HDC dc, const RECT &dst, const AvatarImage &image)
{
    if (image.empty()) return;
    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = image.width;
    bmi.bmiHeader.biHeight = -image.height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    StretchDIBits(dc, dst.left, dst.top, dst.right - dst.left, dst.bottom - dst.top,
                  0, 0, image.width, image.height, image.rgba.data(), &bmi,
                  DIB_RGB_COLORS, SRCCOPY);
}

} // namespace

void draw_preview(HDC dc, const RECT &client, const Runtime &runtime)
{
    const auto &state = runtime.state();
    FillRect(dc, &client, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
    SetBkMode(dc, TRANSPARENT);

    text(dc, 24, 18, L"Otuber Preview");
    text(dc, 24, 42, state.camera_enabled ? L"Camera: ON" : L"Camera: OFF");
    text(dc, 150, 42, state.tracking_enabled ? L"Tracking: ON" : L"Tracking: OFF");
    text(dc, 300, 42, L"Space: camera   Ctrl+O: load PNG");

    RECT camera{24, 78, 500, client.bottom - 24};
    panel(dc, camera);
    text(dc, camera.left + 12, camera.top + 12, L"Camera");
    RECT camera_image{camera.left + 10, camera.top + 38, camera.right - 10, camera.bottom - 10};
    draw_rgba(dc, camera_image, runtime.camera_frame());

    RECT avatar{520, 78, client.right - 220, client.bottom - 24};
    panel(dc, avatar);
    text(dc, avatar.left + 12, avatar.top + 12, L"Avatar");

    const auto &m = state.motion;
    RECT target{avatar.left + 30 + static_cast<int>(m.head_x * 35.0f),
                avatar.top + 50 + static_cast<int>(m.head_y * 30.0f),
                avatar.right - 30 + static_cast<int>(m.head_x * 35.0f),
                avatar.bottom - 20 + static_cast<int>(m.head_y * 30.0f)};
    if (!runtime.avatar_image().empty()) {
        draw_avatar_image(dc, target, runtime.avatar_image());
    } else {
        const int cx = (target.left + target.right) / 2;
        const int cy = (target.top + target.bottom) / 2;
        const int radius = 95;
        Ellipse(dc, cx - radius, cy - radius, cx + radius, cy + radius);
        const int eye_h = std::max(2, static_cast<int>(18.0f * m.eye_left_open));
        Ellipse(dc, cx - 46, cy - 30 - eye_h / 2, cx - 30, cy - 30 + eye_h / 2);
        Ellipse(dc, cx + 30, cy - 30 - eye_h / 2, cx + 46, cy - 30 + eye_h / 2);
        const int mouth_h = 4 + static_cast<int>(42.0f * std::clamp(m.mouth_open, 0.0f, 1.0f));
        RECT mouth{cx - 35, cy + 30, cx + 35, cy + 30 + mouth_h};
        FillRect(dc, &mouth, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
    }

    RECT stats{client.right - 196, 78, client.right - 24, client.bottom - 24};
    panel(dc, stats);
    text(dc, stats.left + 12, stats.top + 12, L"Tracking");
    int y = stats.top + 48;
    const int x = stats.left + 12;
    text(dc, x, y, L"Head X"); bar(dc, x + 54, y + 2, 95, (m.head_x + 1.0f) * .5f); y += 38;
    text(dc, x, y, L"Head Y"); bar(dc, x + 54, y + 2, 95, (m.head_y + 1.0f) * .5f); y += 38;
    text(dc, x, y, L"Rotate"); bar(dc, x + 54, y + 2, 95, (m.head_rotation + 1.0f) * .5f); y += 38;
    text(dc, x, y, L"Eye L");  bar(dc, x + 54, y + 2, 95, m.eye_left_open); y += 38;
    text(dc, x, y, L"Eye R");  bar(dc, x + 54, y + 2, 95, m.eye_right_open); y += 38;
    text(dc, x, y, L"Mouth");  bar(dc, x + 54, y + 2, 95, m.mouth_open);
}

} // namespace otuber::preview
