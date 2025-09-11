#ifndef COLUMBITS_GUI_H
#define COLUMBITS_GUI_H

#include "COLUMBITS.h"

#ifdef COLUMBITS_SDL
typedef struct {

} _EVENT;
#elif defined(COLUMBITS_WINAPI)
typedef struct {

} _EVENT;
#elif defined(COLUMBITS_MACAPI)
typedef struct {

} _EVENT;
#elif defined(COLUMBITS_LINAPI)
typedef struct {

} _EVENT;
#else
typedef struct {
  uint16_t flags;
} _EVENT;

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
