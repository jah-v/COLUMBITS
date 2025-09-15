#ifndef COLUMBITS_3D_H
#define COLUMBITS_3D_H

#include "COLUMBITS.h"

#ifdef HD
typedef struct { double a_x, a_y, a_z, b_x, b_y, b_z; uint8_t curve_type; } _3D_LINE;
typedef struct { _3D_RAY* vertices; uint8_t num_of_vertices; } _MESH;
#else
typedef struct { float a_x, a_y, a_z, b_x, b_y, b_z; uint8_t curve_type; } _3D_RAY;
typedef struct { _3D_RAY* vertices; uint8_t num_of_vertices; } _MESH;
#endif

_BITMAP* DRAW_MESH();
_MESH* NEW_MESH();

_MESH* 3D_TRANSLATE();
_MESH* 3D_ROTATE();
_MESH* 3D_SCALE();

#endif // COLUMBITS_3D_H