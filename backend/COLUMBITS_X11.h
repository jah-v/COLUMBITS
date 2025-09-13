#ifndef COLUMBITS_X11_H
#define COLUMBITS_X11_H

#if defined(__linux__) || defined(__unix__)

#include "COLUMBITS_WINDOW.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Display* display;
    Window window;
    GC gc;
    XImage* ximage;
    Visual* visual;
    int screen;
    Atom wm_delete;
    uint16_t width, height;
    bool should_close;
} X11_WINDOW;

static WINDOW_HANDLE x11_create_window(const char* title, uint16_t width, uint16_t height) {
    X11_WINDOW* window = (X11_WINDOW*)calloc(1, sizeof(X11_WINDOW));
    
    window->display = XOpenDisplay(NULL);
    if (!window->display) {
        free(window);
        return NULL;
    }
    
    window->screen = DefaultScreen(window->display);
    window->visual = DefaultVisual(window->display, window->screen);
    window->width = width;
    window->height = height;
    
    window->window = XCreateSimpleWindow(
        window->display,
        RootWindow(window->display, window->screen),
        0, 0, width, height, 1,
        BlackPixel(window->display, window->screen),
        WhitePixel(window->display, window->screen)
    );
    
    XStoreName(window->display, window->window, title);
    
    // Set up window close event
    window->wm_delete = XInternAtom(window->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(window->display, window->window, &window->wm_delete, 1);
    
    // Select input events
    XSelectInput(window->display, window->window,
        ExposureMask | KeyPressMask | KeyReleaseMask | 
        ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
        StructureNotifyMask);
    
    window->gc = XCreateGC(window->display, window->window, 0, NULL);
    
    return (WINDOW_HANDLE)window;
}

static void x11_destroy_window(WINDOW_HANDLE handle) {
    X11_WINDOW* window = (X11_WINDOW*)handle;
    if (window) {
        if (window->ximage) {
            XDestroyImage(window->ximage);
        }
        XFreeGC(window->display, window->gc);
        XDestroyWindow(window->display, window->window);
        XCloseDisplay(window->display);
        free(window);
    }
}

static void x11_show_window(WINDOW_HANDLE handle) {
    X11_WINDOW* window = (X11_WINDOW*)handle;
    XMapWindow(window->display, window->window);
    XFlush(window->display);
}

static void x11_hide_window(WINDOW_HANDLE handle) {
    X11_WINDOW* window = (X11_WINDOW*)handle;
    XUnmapWindow(window->display, window->window);
    XFlush(window->display);
}

static bool x11_poll_event(WINDOW_HANDLE handle, WINDOW_EVENT* event) {
    X11_WINDOW* window = (X11_WINDOW*)handle;
    XEvent xevent;
    
    event->type = EVENT_NONE;
    
    if (window->should_close) {
        event->type = EVENT_QUIT;
        return true;
    }
    
    if (XPending(window->display)) {
        XNextEvent(window->display, &xevent);
        
        switch (xevent.type) {
            case ClientMessage:
                if ((Atom)xevent.xclient.data.l[0] == window->wm_delete) {
                    event->type = EVENT_QUIT;
                    window->should_close = true;
                }
                break;
                
            case ConfigureNotify:
                if (xevent.xconfigure.width != window->width || 
                    xevent.xconfigure.height != window->height) {
                    window->width = xevent.xconfigure.width;
                    window->height = xevent.xconfigure.height;
                    event->type = EVENT_RESIZE;
                    event->data.resize.width = window->width;
                    event->data.resize.height = window->height;
                }
                break;
                
            case Expose:
                event->type = EVENT_REDRAW;
                break;
                
            case KeyPress:
                event->type = EVENT_KEY_DOWN;
                event->data.key.keycode = XLookupKeysym(&xevent.xkey, 0);
                break;
                
            case KeyRelease:
                event->type = EVENT_KEY_UP;
                event->data.key.keycode = XLookupKeysym(&xevent.xkey, 0);
                break;
                
            case MotionNotify:
                event->type = EVENT_MOUSE_MOVE;
                event->data.mouse.x = xevent.xmotion.x;
                event->data.mouse.y = xevent.xmotion.y;
                break;
                
            case ButtonPress:
                event->type = EVENT_MOUSE_DOWN;
                event->data.mouse.x = xevent.xbutton.x;
                event->data.mouse.y = xevent.xbutton.y;
                event->data.mouse.button = xevent.xbutton.button;
                break;
                
            case ButtonRelease:
                event->type = EVENT_MOUSE_UP;
                event->data.mouse.x = xevent.xbutton.x;
                event->data.mouse.y = xevent.xbutton.y;
                event->data.mouse.button = xevent.xbutton.button;
                break;
        }
        return true;
    }
    return false;
}

static void x11_wait_event(WINDOW_HANDLE handle, WINDOW_EVENT* event) {
    X11_WINDOW* window = (X11_WINDOW*)handle;
    XEvent xevent;
    
    XNextEvent(window->display, &xevent);
    XPutBackEvent(window->display, &xevent);
    x11_poll_event(handle, event);
}

static void x11_present_bitmap(WINDOW_HANDLE handle, _BITMAP* bitmap) {
    X11_WINDOW* window = (X11_WINDOW*)handle;
    
    // Convert RGBA to BGRA for X11 (assuming little-endian)
    uint32_t* pixel_data = (uint32_t*)malloc(bitmap->w * bitmap->h * sizeof(uint32_t));
    for (uint32_t i = 0; i < (bitmap->w * bitmap->h); i++) {
        pixel_data[i] = (bitmap->pixels[i].a << 24) |
                       (bitmap->pixels[i].r << 16) |
                       (bitmap->pixels[i].g << 8) |
                       (bitmap->pixels[i].b);
    }
    
    // Create or update XImage
    if (window->ximage) {
        XDestroyImage(window->ximage);
    }
    
    window->ximage = XCreateImage(
        window->display,
        window->visual,
        24,  // depth
        ZPixmap,
        0,
        (char*)pixel_data,
        bitmap->w,
        bitmap->h,
        32,  // bitmap_pad
        0    // bytes_per_line (0 = auto calculate)
    );
    
    XPutImage(window->display, window->window, window->gc, 
              window->ximage, 0, 0, 0, 0, bitmap->w, bitmap->h);
    XFlush(window->display);
}

static void x11_get_window_size(WINDOW_HANDLE handle, uint16_t* width, uint16_t* height) {
    X11_WINDOW* window = (X11_WINDOW*)handle;
    XWindowAttributes attrs;
    XGetWindowAttributes(window->display, window->window, &attrs);
    *width = attrs.width;
    *height = attrs.height;
}

static void x11_init_platform(void) {
    XInitThreads();  // Enable thread safety if needed
}

static void x11_shutdown_platform(void) {
    // Nothing specific needed for X11
}

// Platform implementation
static WINDOW_PLATFORM x11_platform = {
    .create_window = x11_create_window,
    .destroy_window = x11_destroy_window,
    .show_window = x11_show_window,
    .hide_window = x11_hide_window,
    .poll_event = x11_poll_event,
    .wait_event = x11_wait_event,
    .present_bitmap = x11_present_bitmap,
    .get_window_size = x11_get_window_size,
    .init_platform = x11_init_platform,
    .shutdown_platform = x11_shutdown_platform
};

// Set the global platform pointer
WINDOW_PLATFORM* g_platform = &x11_platform;

#endif // __linux__ || __unix__
#endif // COLUMBITS_X11_H