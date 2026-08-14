#include "preview_ui.hpp"

#include <algorithm>
#include <cmath>
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

} // namespace

void draw_preview(HDC dc, const RECT &client, const AppState &state)
{
    FillRect(dc, &client, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
    SetBkMode(dc, TRANSPARENT);

    text(dc, 24, 18, L"Otuber Preview");
    text(dc, 24, 42, state.camera_enabled ? L"Camera: ON" : L"Camera: OFF");
    text(dc, 150, 42, state.tracking_enabled ? L"Tracking: ON" : L"Tracking: OFF");

    RECT avatar{24, 78, client.right - 250, client.bottom - 24};
    panel(dc, avatar);
    text(dc, avatar.left + 16, avatar.top + 14, L"Avatar");

    const int cx = (avatar.left + avatar.right) / 2 +
                   static_cast<int>(state.motion.head_x * 55.0f);
    const int cy = (avatar.top + avatar.bottom) / 2 +
                   static_cast<int>(state.motion.head_y * 45.0f);

    // Lightweight diagnostic avatar: the same motion values used by the core
    // are visualized without introducing a renderer dependency into the core.
    const int radius = 95;
    Ellipse(dc, cx - radius, cy - radius, cx + radius, cy + radius);

    const float rot = state.motion.head_rotation;
    const int eye_y = cy - 25;
    const int eye_dx = 38;
    const int eye_h_l = std::max(2, static_cast<int>(18.0f * state.motion.eye_left_open));
    const int eye_h_r = std::max(2, static_cast<int>(18.0f * state.motion.eye_right_open));
    Ellipse(dc, cx - eye_dx - 8, eye_y - eye_h_l / 2, cx - eye_dx + 8, eye_y + eye_h_l / 2);
    Ellipse(dc, cx + eye_dx - 8, eye_y - eye_h_r / 2, cx + eye_dx + 8, eye_y + eye_h_r / 2);

    const int mouth_h = 4 + static_cast<int>(42.0f * std::clamp(state.motion.mouth_open, 0.0f, 1.0f));
    RECT mouth{cx - 35, cy + 38, cx + 35, cy + 38 + mouth_h};
    FillRect(dc, &mouth, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

    // Rotation indicator.
    const int line_x = cx + static_cast<int>(std::sin(rot) * 70.0f);
    const int line_y = cy - static_cast<int>(std::cos(rot) * 70.0f);
    MoveToEx(dc, cx, cy, nullptr);
    LineTo(dc, line_x, line_y);

    RECT stats{client.right - 220, 78, client.right - 24, client.bottom - 24};
    panel(dc, stats);
    text(dc, stats.left + 14, stats.top + 14, L"Tracking");

    const int x = stats.left + 14;
    int y = stats.top + 52;
    text(dc, x, y, L"Head X"); bar(dc, x + 62, y + 2, 105, (state.motion.head_x + 1.0f) * 0.5f); y += 42;
    text(dc, x, y, L"Head Y"); bar(dc, x + 62, y + 2, 105, (state.motion.head_y + 1.0f) * 0.5f); y += 42;
    text(dc, x, y, L"Rotate"); bar(dc, x + 62, y + 2, 105, (state.motion.head_rotation + 1.0f) * 0.5f); y += 42;
    text(dc, x, y, L"Eye L");  bar(dc, x + 62, y + 2, 105, state.motion.eye_left_open); y += 42;
    text(dc, x, y, L"Eye R");  bar(dc, x + 62, y + 2, 105, state.motion.eye_right_open); y += 42;
    text(dc, x, y, L"Mouth");  bar(dc, x + 62, y + 2, 105, state.motion.mouth_open);
}

void update_demo_motion(AppState &state, float time_seconds)
{
    // Deterministic fallback motion keeps the preview useful when a camera
    // backend is unavailable. The same CameraState path is used by real input.
    state.motion.connected = state.camera_enabled;
    state.motion.head_x = std::sin(time_seconds * 0.8f) * 0.35f;
    state.motion.head_y = std::cos(time_seconds * 0.55f) * 0.12f;
    state.motion.head_rotation = std::sin(time_seconds * 0.7f) * 0.25f;
    state.motion.eye_left_open = 0.85f + 0.15f * std::sin(time_seconds * 0.4f);
    state.motion.eye_right_open = 0.85f + 0.15f * std::sin(time_seconds * 0.4f);
    state.motion.mouth_open = 0.15f + 0.15f * (0.5f + 0.5f * std::sin(time_seconds * 1.2f));
}

} // namespace otuber::preview
