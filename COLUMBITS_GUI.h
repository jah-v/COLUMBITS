#ifndef COLUMBITS_GUI_H
#define COLUMBITS_GUI_H

#include "COLUMBITS.h"

typedef struct {
  _TEXT text;
  uint16_t size;
  _COLOR color;
} _WIDGET;

typedef struct {
  _WIDGET* widgets;
  uint8_t style, shape;
} _WIDGET_BAR;

_WIDGET NEW_WIDGET();

_WIDGET_BAR* NEW_WIDGET_BAR();

_BITMAP* DRAW_WIDGET_BAR();

#endif // COLUMBITS_GUI_H
