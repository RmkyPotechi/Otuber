#pragma once

#include <windows.h>

#include "app_state.hpp"
#include "avatar_image.hpp"
#include "runtime.hpp"

namespace otuber::preview {

void draw_preview(HDC dc, const RECT &client, const Runtime &runtime);

} // namespace otuber::preview
