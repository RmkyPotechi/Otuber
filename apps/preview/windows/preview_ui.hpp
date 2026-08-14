#pragma once

#include <windows.h>

#include "app_state.hpp"

namespace otuber::preview {

void draw_preview(HDC dc, const RECT &client, const AppState &state);
void update_demo_motion(AppState &state, float time_seconds);

} // namespace otuber::preview
