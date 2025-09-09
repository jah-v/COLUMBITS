#ifndef COLUMBITS_GUI_H
#define COLUMBITS_GUI_H

#include "COLUMBITS.h"

typedef struct {
    _TEXT* text;
    _COLOR color;
    uint16_t span;
    uint8_t flags;
} _WIDGET;

typedef struct {
    _WIDGET* widgets;
    uint8_t widget_number;
    uint16_t size; 
    uint8_t shape, style; 
} _WIDGET_BAR;

_WIDGET_BAR NEW_WIDGET_BAR();

_WIDGET_BAR NEW_WIDGET();

_BITMAP* DRAW_WIDGET_BAR();

#endif // COLUMBITS_GUI_H