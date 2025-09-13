#ifndef COLUMBITS_WINDOW_H
#define COLUMBITS_WINDOW_H

#include "COLUMBITS.h"
#include <stdbool.h>

typedef void* _WINDOW_HANDLE;

typedef enum {
    EVENT_NONE = 0,
    EVENT_QUIT,
    EVENT_RESIZE,
    EVENT_KEY_DOWN,
    EVENT_KEY_UP,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_DOWN,
    EVENT_MOUSE_UP,
    EVENT_REDRAW    
} _EVT_TYPE;

typedef struct {
    _EVT_TYPE type;
    union {
        struct { uint16_t w, h; } resize;
        struct { uint32_t keycode; } key;
        struct { int16_t x, y; uint8_t button; } mouse;
    } data;
} _EVT;

typedef struct {
    _WINDOW_HANDLE (*create_window)(const char* title, uint16_t width, uint16_t height);
    void (*destroy_window)(_WINDOW_HANDLE window);
    void (*show_window)(_WINDOW_HANDLE window);
    void (*hide_window)(_WINDOW_HANDLE window);
    
    // Event handling
    bool (*poll_event)(_WINDOW_HANDLE window, _EVT* event);
    void (*wait_event)(_WINDOW_HANDLE window, _EVT* event);
    
    // Rendering
    void (*present_bitmap)(_WINDOW_HANDLE window, _BITMAP* bitmap);
    void (*get_window_size)(_WINDOW_HANDLE window, uint16_t* width, uint16_t* height);
    
    // Platform specific
    void (*init_platform)(void);
    void (*shutdown_platform)(void);
} _WINDOW_PLATFORM

extern _WINDOW_PLATFORM* g_platform;

// Convenience macros for cleaner API
#define CREATE_WINDOW(title, w, h) g_platform->create_window(title, w, h)
#define DESTROY_WINDOW(window) g_platform->destroy_window(window)
#define SHOW_WINDOW(window) g_platform->show_window(window)
#define POLL_EVENT(window, event) g_platform->poll_event(window, event)
#define PRESENT_BITMAP(window, bitmap) g_platform->present_bitmap(window, bitmap)
#define INIT_PLATFORM() g_platform->init_platform()
#define SHUTDOWN_PLATFORM() g_platform->shutdown_platform()

#endif // COLUMBITS_WINDOW_H