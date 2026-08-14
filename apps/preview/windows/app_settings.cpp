#include "app_settings.hpp"

#include <fstream>

namespace otuber::preview {

namespace {
constexpr const char *settings_file = "otuber_preview.cfg";
}

// Keep persistence deliberately tiny and human-readable for the first release.
// The UI can later expose these fields without changing the core.
void save_settings(const AppSettings &settings)
{
    std::ofstream out(settings_file, std::ios::trunc);
    if (!out) return;
    out << settings.camera_index << '\n'
        << settings.mirror_camera << '\n'
        << settings.show_tracking << '\n'
        << settings.show_mesh << '\n'
        << settings.smoothing << '\n';
}

bool load_settings(AppSettings &settings)
{
    std::ifstream in(settings_file);
    if (!in) return false;
    return static_cast<bool>(in >> settings.camera_index
                                >> settings.mirror_camera
                                >> settings.show_tracking
                                >> settings.show_mesh
                                >> settings.smoothing);
}

} // namespace otuber::preview
