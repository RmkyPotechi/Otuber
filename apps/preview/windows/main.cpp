#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

namespace {

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps{};
        HDC dc = BeginPaint(hwnd, &ps);

        RECT client{};
        GetClientRect(hwnd, &client);
        FillRect(dc, &client, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

        SetBkMode(dc, TRANSPARENT);
        const std::wstring title = L"Otuber Windows Preview";
        const std::wstring status = L"Windows GUI backend: OK";
        const std::wstring camera = L"Camera backend: ready for capture integration";

        TextOutW(dc, 32, 32, title.c_str(), static_cast<int>(title.size()));
        TextOutW(dc, 32, 72, status.c_str(), static_cast<int>(status.size()));
        TextOutW(dc, 32, 104, camera.c_str(), static_cast<int>(camera.size()));

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd, message, wparam, lparam);
    }
}

} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show_command)
{
    const wchar_t class_name[] = L"OtuberPreviewWindow";

    WNDCLASSW window_class{};
    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = instance;
    window_class.lpszClassName = class_name;
    window_class.hCursor = LoadCursorW(nullptr, IDC_ARROW);

    if (!RegisterClassW(&window_class))
        return 1;

    HWND window = CreateWindowExW(
        0,
        class_name,
        L"Otuber Preview",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        960,
        640,
        nullptr,
        nullptr,
        instance,
        nullptr);

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
