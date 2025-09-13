#ifndef WINDOW_WIN32_H
#define WINDOW_WIN32_H

#ifdef _WIN32

#include "COLUMBITS_WINDOW.h"
#include <windows.h>
#include <stdlib.h>

typedef struct {
    HWND hwnd;
    HDC hdc;
    BITMAPINFO bmi;
    _BITMAP* current_bitmap;
    bool should_close;
} WIN32_WINDOW;

static LRESULT CALLBACK Win32_WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WIN32_WINDOW* window = (WIN32_WINDOW*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    
    switch (msg) {
        case WM_CLOSE:
            if (window) window->should_close = true;
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (window && window->current_bitmap) {
                // Present the bitmap
                StretchDIBits(hdc,
                    0, 0, window->current_bitmap->w, window->current_bitmap->h,
                    0, 0, window->current_bitmap->w, window->current_bitmap->h,
                    window->current_bitmap->pixels,
                    &window->bmi,
                    DIB_RGB_COLORS,
                    SRCCOPY);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

static WINDOW_HANDLE win32_create_window(const char* title, uint16_t width, uint16_t height) {
    WIN32_WINDOW* window = (WIN32_WINDOW*)calloc(1, sizeof(WIN32_WINDOW));
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = Win32_WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "BitmapWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&wc);
    
    window->hwnd = CreateWindowEx(
        0,
        "BitmapWindow",
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL,
        GetModuleHandle(NULL),
        NULL
    );
    
    SetWindowLongPtr(window->hwnd, GWLP_USERDATA, (LONG_PTR)window);
    window->hdc = GetDC(window->hwnd);
    
    // Setup bitmap info
    window->bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    window->bmi.bmiHeader.biWidth = width;
    window->bmi.bmiHeader.biHeight = -height; // Top-down bitmap
    window->bmi.bmiHeader.biPlanes = 1;
    window->bmi.bmiHeader.biBitCount = 32;
    window->bmi.bmiHeader.biCompression = BI_RGB;
    
    return (WINDOW_HANDLE)window;
}

static void win32_destroy_window(WINDOW_HANDLE handle) {
    WIN32_WINDOW* window = (WIN32_WINDOW*)handle;
    if (window) {
        ReleaseDC(window->hwnd, window->hdc);
        DestroyWindow(window->hwnd);
        free(window);
    }
}

static void win32_show_window(WINDOW_HANDLE handle) {
    WIN32_WINDOW* window = (WIN32_WINDOW*)handle;
    ShowWindow(window->hwnd, SW_SHOW);
    UpdateWindow(window->hwnd);
}

static void win32_hide_window(WINDOW_HANDLE handle) {
    WIN32_WINDOW* window = (WIN32_WINDOW*)handle;
    ShowWindow(window->hwnd, SW_HIDE);
}

static bool win32_poll_event(WINDOW_HANDLE handle, WINDOW_EVENT* event) {
    WIN32_WINDOW* window = (WIN32_WINDOW*)handle;
    MSG msg;
    
    event->type = EVENT_NONE;
    
    if (window->should_close) {
        event->type = EVENT_QUIT;
        return true;
    }
    
    if (PeekMessage(&msg, window->hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        switch (msg.message) {
            case WM_QUIT:
                event->type = EVENT_QUIT;
                break;
            case WM_SIZE:
                event->type = EVENT_RESIZE;
                event->data.resize.width = LOWORD(msg.lParam);
                event->data.resize.height = HIWORD(msg.lParam);
                // Update bitmap info
                window->bmi.bmiHeader.biWidth = event->data.resize.width;
                window->bmi.bmiHeader.biHeight = -event->data.resize.height;
                break;
            case WM_KEYDOWN:
                event->type = EVENT_KEY_DOWN;
                event->data.key.keycode = msg.wParam;
                break;
            case WM_KEYUP:
                event->type = EVENT_KEY_UP;
                event->data.key.keycode = msg.wParam;
                break;
            case WM_MOUSEMOVE:
                event->type = EVENT_MOUSE_MOVE;
                event->data.mouse.x = LOWORD(msg.lParam);
                event->data.mouse.y = HIWORD(msg.lParam);
                break;
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
                event->type = EVENT_MOUSE_DOWN;
                event->data.mouse.x = LOWORD(msg.lParam);
                event->data.mouse.y = HIWORD(msg.lParam);
                event->data.mouse.button = (msg.message == WM_LBUTTONDOWN) ? 1 : 
                                          (msg.message == WM_RBUTTONDOWN) ? 2 : 3;
                break;
        }
        return true;
    }
    return false;
}

static void win32_wait_event(WINDOW_HANDLE handle, WINDOW_EVENT* event) {
    WIN32_WINDOW* window = (WIN32_WINDOW*)handle;
    MSG msg;
    
    WaitMessage();
    win32_poll_event(handle, event);
}

static void win32_present_bitmap(WINDOW_HANDLE handle, _BITMAP* bitmap) {
    WIN32_WINDOW* window = (WIN32_WINDOW*)handle;
    window->current_bitmap = bitmap;
    
    // Convert RGBA to BGRA for Windows
    for (uint32_t i = 0; i < (bitmap->w * bitmap->h); i++) {
        uint8_t temp = bitmap->pixels[i].r;
        bitmap->pixels[i].r = bitmap->pixels[i].b;
        bitmap->pixels[i].b = temp;
    }
    
    StretchDIBits(window->hdc,
        0, 0, bitmap->w, bitmap->h,
        0, 0, bitmap->w, bitmap->h,
        bitmap->pixels,
        &window->bmi,
        DIB_RGB_COLORS,
        SRCCOPY);
    
    // Convert back
    for (uint32_t i = 0; i < (bitmap->w * bitmap->h); i++) {
        uint8_t temp = bitmap->pixels[i].r;
        bitmap->pixels[i].r = bitmap->pixels[i].b;
        bitmap->pixels[i].b = temp;
    }
}

static void win32_get_window_size(WINDOW_HANDLE handle, uint16_t* width, uint16_t* height) {
    WIN32_WINDOW* window = (WIN32_WINDOW*)handle;
    RECT rect;
    GetClientRect(window->hwnd, &rect);
    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

static void win32_init_platform(void) {
    // Nothing specific needed for Win32
}

static void win32_shutdown_platform(void) {
    // Nothing specific needed for Win32
}

// Platform implementation
static WINDOW_PLATFORM win32_platform = {
    .create_window = win32_create_window,
    .destroy_window = win32_destroy_window,
    .show_window = win32_show_window,
    .hide_window = win32_hide_window,
    .poll_event = win32_poll_event,
    .wait_event = win32_wait_event,
    .present_bitmap = win32_present_bitmap,
    .get_window_size = win32_get_window_size,
    .init_platform = win32_init_platform,
    .shutdown_platform = win32_shutdown_platform
};

// Set the global platform pointer
WINDOW_PLATFORM* g_platform = &win32_platform;

#endif // _WIN32
#endif // COLUMBITS_WINAPI_H