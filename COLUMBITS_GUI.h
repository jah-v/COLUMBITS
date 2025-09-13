#ifndef COLUMBITS_GUI_H
#define COLUMBITS_GUI_H

#include "COLUMBITS.h"

#ifndef CUSTOM_WINDOWING
#include "COLUMBITS_WINDOW.h"
#endif

typedef struct {
  _TEXT text;
  uint16_t size;
  _COLOR color;
} _WIDGET;

typedef struct {
  _WIDGET* widgets;
  uint8_t style, shape;
} _WIDGET_BAR;

_WIDGET NEW_WIDGET(   _TEXT text,
                      uint16_t size,
                      _COLOR color    );

_WIDGET_BAR* BIND_WIDGET(
                      _WIDGET child,
                      _WIDGET_BAR* parent,
                      _EVENT event    );

_WIDGET_BAR* UNBIND_WIDGET(
                      _WIDGET child,
                      _WIDGET_BAR* parent,
                      _EVENT event    );

_WIDGET_BAR* NEW_WIDGET_BAR(
                      uint16_t w,
                      uint16_t h,
                      uint8_t style   );
                      
_BITMAP* DRAW_WIDGET_BAR(
                      _WIDGET_BAR* child,
                      _BITMAP* parent );

#endif // COLUMBITS_GUI_H
