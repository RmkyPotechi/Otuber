#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>

#include "runtime.hpp"
#include "preview_ui.hpp"

namespace {
otuber::preview::Runtime runtime;

void load_avatar(HWND hwnd)
{
    wchar_t path[MAX_PATH]{};
    OPENFILENAMEW dialog{};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = hwnd;
    dialog.lpstrFilter = L"PNG image\0*.png\0All files\0*.*\0";
    dialog.lpstrFile = path;
    dialog.nMaxFile = MAX_PATH;
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileNameW(&dialog))
        runtime.load_avatar(path);
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_CREATE:
        SetTimer(hwnd, 1, 16, nullptr);
        runtime.start(0);
        return 0;
    case WM_TIMER:
        runtime.tick();
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;
    case WM_KEYDOWN:
        if (wparam == VK_SPACE) {
            if (runtime.camera_open()) runtime.stop();
            else runtime.start(0);
            InvalidateRect(hwnd, nullptr, FALSE);
        } else if (wparam == 'O' && (GetKeyState(VK_CONTROL) & 0x8000)) {
            load_avatar(hwnd);
            InvalidateRect(hwnd, nullptr, FALSE);
        }
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps{};
        HDC dc = BeginPaint(hwnd, &ps);
        RECT client{};
        GetClientRect(hwnd, &client);
        otuber::preview::draw_preview(dc, client, runtime);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        runtime.stop();
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd, message, wparam, lparam);
    }
}
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show_command)
{
    const wchar_t name[] = L"OtuberPreviewWindow";
    WNDCLASSW wc{};
    wc.lpfnWndProc = window_proc;
    wc.hInstance = instance;
    wc.lpszClassName = name;
    wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    if (!RegisterClassW(&wc)) return 1;

    HWND window = CreateWindowExW(0, name, L"Otuber Preview",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 720,
        nullptr, nullptr, instance, nullptr);
    if (!window) return 1;

    ShowWindow(window, show_command);
    UpdateWindow(window);

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}
