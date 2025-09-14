#include "COLUMBITS_WINDOW.h"
#include "COLUMBITS.h"

int main() {
    COLUMBITS_INIT();
    _WINDOW_HANDLE window = NEW_WINDOW("Sample #1", 800, 600);
    if (!window) {
        REPORT_ERROR();
    };
    _COLOR black = {0, 0, 0, 0};
    _COLOR red = { 255, 0, 0, 0 };
    _COLOR blue = { 0, 100, 255, 0 };
    _COLOR green = { 0, 255, 100, 0 };

    _BITMAP canvas = NEW_BITMAP(800, 600, black);
    

    DRAW_RECTANGLE(100, 100, 300, 200, &canvas, red);
    DRAW_CIRCLE(400, 300, 50, &canvas, blue);
    DRAW_ROUNDTANGLE(50, 350, 250, 450, 128, &canvas, green);

    SHOW_WINDOW(window);

    _EVT event;
    bool running = true;
    while (running) {
        while(POLL_EVENT(window, &event)) {
            switch (event.type) {
                case EVENT_QUIT:
                running = false;
                break;
        }
        PRESENT_BITMAP(window, &canvas);
        }
    }

    free(canvas.pixels);
    DESTROY_WINDOW(window);
    SHUTDOWN_PLATFORM();

    return 0;
}