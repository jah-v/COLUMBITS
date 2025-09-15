#ifndef COLUMBITS_2D_H
#define COLUMBITS_2D_H

#include "COLUMBITS.h"

#ifdef HD
typedef struct { double a_x, a_y, b_x, b_y; uint8_t curve_type; } _LINE;
typedef struct { _LINE* lines; uint8_t num_of_lines; } _SHAPE;
#else
typedef struct { float a_x, a_y, b_x, b_y; uint8_t curve_type; } _LINE;
typedef struct { _LINE* lines; uint8_t num_of_lines; } _SHAPE;
#endif

_BITMAP* DRAW_LINE();
_BITMAP* DRAW_CURVE();
_BITMAP* DRAW_TRIANGLE();
_BITMAP* DRAW_SHAPE();

_SHAPE* NEW_LINE();
_SHAPE* NEW_CURVE();
_SHAPE* NEW_TRIANGLE();
_SHAPE* NEW_SHAPE_FROM();

_SHAPE* 2D_TRANSLATE();
_SHAPE* 2D_ROTATE();
_SHAPE* 2D_SCALE();

#endif // COLUMBITS_2D_H