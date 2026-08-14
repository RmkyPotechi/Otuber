#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <chrono>
#include "preview_ui.hpp"

namespace { otuber::preview::AppState app_state; std::chrono::steady_clock::time_point start_time;
LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
 switch(message) {
 case WM_CREATE: start_time=std::chrono::steady_clock::now(); SetTimer(hwnd,1,16,nullptr); return 0;
 case WM_TIMER: InvalidateRect(hwnd,nullptr,FALSE); return 0;
 case WM_KEYDOWN:
  if(wparam==VK_SPACE) app_state.camera_enabled=!app_state.camera_enabled;
  else if(wparam=='T') app_state.tracking_enabled=!app_state.tracking_enabled;
  InvalidateRect(hwnd,nullptr,FALSE); return 0;
 case WM_PAINT:{ PAINTSTRUCT ps{}; HDC dc=BeginPaint(hwnd,&ps); if(app_state.tracking_enabled){auto e=std::chrono::steady_clock::now()-start_time; otuber::preview::update_demo_motion(app_state,std::chrono::duration<float>(e).count());} RECT r{}; GetClientRect(hwnd,&r); otuber::preview::draw_preview(dc,r,app_state); EndPaint(hwnd,&ps); return 0; }
 case WM_DESTROY: KillTimer(hwnd,1); PostQuitMessage(0); return 0;
 default: return DefWindowProcW(hwnd,message,wparam,lparam); }
}
}
int WINAPI wWinMain(HINSTANCE instance,HINSTANCE, PWSTR,int show_command){
 const wchar_t name[]=L"OtuberPreviewWindow"; WNDCLASSW wc{}; wc.lpfnWndProc=window_proc; wc.hInstance=instance; wc.lpszClassName=name; wc.hCursor=LoadCursorW(nullptr,IDC_ARROW); wc.hbrBackground=static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); if(!RegisterClassW(&wc)) return 1;
 HWND w=CreateWindowExW(0,name,L"Otuber Preview",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1100,700,nullptr,nullptr,instance,nullptr); if(!w) return 1; ShowWindow(w,show_command); UpdateWindow(w); MSG m{}; while(GetMessageW(&m,nullptr,0,0)>0){TranslateMessage(&m);DispatchMessageW(&m);} return static_cast<int>(m.wParam);
}
