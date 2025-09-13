#ifndef COLUMBITS_H
#define COLUMBITS_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct {    uint8_t r, g, b, flags;     } _COLOR;
typedef struct {    uint8_t r, g, b, a;         } _PIXEL;
typedef struct {    _PIXEL* pixels;
                    uint16_t w, h;              } _BITMAP;
typedef struct {    char* a;
                    uint16_t w, h;              } _TEXTURE;
typedef struct {    uint16_t a_x, a_y, b_x, b_y;
                    _COLOR color;               } _RECTANGLE;

typedef struct {    _TEXTURE* t;
                    uint16_t w, h;              } _FONT;
typedef struct {    char* str; 
                    uint16_t str_length, font_size;
                    _FONT* font;
                    _COLOR color;
                    uint8_t style;              } _TEXT;


inline uint16_t larger_of( uint16_t a, uint16_t b );
inline uint16_t smaller_of( uint16_t a, uint16_t b);
inline uint32_t distance_from(
                    uint16_t a_x, uint16_t a_y,
                    uint16_t b_x, uint16_t b_y
);

void REPORT_ERROR();

_BITMAP CREATE_BITMAP(
                    uint16_t w, uint16_t h,
                    _COLOR color
);

_BITMAP* SCALE_BITMAP(
                    _BITMAP* bitmap,
                    float scale,
                    uint8_t flags
);

_BITMAP* SWAP_BITMAPS(
                    _BITMAP* interior,
                    _BITMAP* exterior,
                    uint8_t flags
);

_BITMAP* MERGE_BITMAPS(
                    _BITMAP* parent,
                    _BITMAP* child,
                    uint16_t child_x,
                    uint16_t child_y
);

_RECTANGLE DRAW_RECTANGLE(
                    uint16_t a_x, uint16_t a_y,
                    uint16_t b_x, uint16_t b_y,
                    _BITMAP* bitmap, _COLOR color
);

_BITMAP* DRAW_CIRCLE(
                    uint16_t x, uint16_t y,
                    uint16_t radius,
                    _BITMAP* bitmap, _COLOR color
);

_BITMAP* DRAW_ROUNDTANGLE(
                    uint16_t a_x, uint16_t a_y,
                    uint16_t b_x, uint16_t b_y,
                    uint8_t roundness,
                    _BITMAP* bitmap,
                    _COLOR color
);



_TEXT* NEW_TEXT(
                    char* c,
                    uint16_t length,
                    uint16_t font_size,
                    _BITMAP* font,
                    uint8_t style
);

_BITMAP* DRAW_TEXT(
                    _TEXT* text,
                    _RECTANGLE parent
);

_BITMAP* DRAW_ROUNDTANGLE(
                    uint16_t a_x, uint16_t a_y,
                    uint16_t b_x, uint16_t b_y,
                    uint8_t roundness, // 255 is full roundness
                    _BITMAP* bitmap,
                    _COLOR color
) {
    uint16_t c_x = smaller_of(a_x, b_x);
    uint16_t c_y = smaller_of(a_y, b_y);
    uint16_t d_x = larger_of(a_x, b_x);
    uint16_t d_y = larger_of(a_y, b_y);

    uint16_t width = d_x - c_x;
    uint16_t height = d_y - c_y;
    uint16_t min_side = (width < height) ? width : height;

    // scale radius proportionally
    uint16_t radius = (min_side / 2) * roundness / 255;

    // center rectangle (without rounded corners)
    DRAW_RECTANGLE(c_x + radius, c_y,
        d_x - radius, d_y,
        bitmap, color);
    DRAW_RECTANGLE(c_x, c_y + radius,
        d_x, d_y - radius,
        bitmap, color);

    // 4 rounded corners
    if (radius > 0) {
        DRAW_CIRCLE(c_x + radius, c_y + radius, radius, bitmap, color); // top-left
        DRAW_CIRCLE(d_x - radius, c_y + radius, radius, bitmap, color); // top-right
        DRAW_CIRCLE(c_x + radius, d_y - radius, radius, bitmap, color); // bottom-left
        DRAW_CIRCLE(d_x - radius, d_y - radius, radius, bitmap, color); // bottom-right
    }

    return bitmap;
}


_BITMAP* DRAW_CIRCLE(
                    uint16_t x, uint16_t y,
                    uint16_t radius,
                    _BITMAP* bitmap, _COLOR color
) {
    for (int16_t b = (int16_t)(y - radius); b <= (int16_t)(y + radius); ++b) {
    for (int16_t a = (int16_t)(x - radius); a <= (int16_t)(x + radius); ++a) {
        // check bounds
        if (a >= 0 && a < bitmap->w && b >= 0 && b < bitmap->h) {
            // real Euclidean distance
            int16_t dx = a - x;
            int16_t dy = b - y;
            if (dx * dx + dy * dy <= radius * radius) {
                bitmap->pixels[b * bitmap->w + a].r = color.r;
                bitmap->pixels[b * bitmap->w + a].g = color.g;
                bitmap->pixels[b * bitmap->w + a].b = color.b;
                bitmap->pixels[b * bitmap->w + a].a = 255;
            }
        }
    }
}
    return bitmap;
}

_RECTANGLE DRAW_RECTANGLE(
                    uint16_t a_x, uint16_t a_y,
                    uint16_t b_x, uint16_t b_y,
                    _BITMAP* bitmap, _COLOR color
) {
    for (int y = smaller_of(a_y, b_y); y < larger_of(a_y, b_y); ++y) {
        for (int x = smaller_of(a_x, b_x); y < larger_of(a_x, b_x); ++x) {
            bitmap->pixels[(y*bitmap->w + x)].r = color.r;
            bitmap->pixels[(y*bitmap->w + x)].g = color.g;
            bitmap->pixels[(y*bitmap->w + x)].b = color.b;
            bitmap->pixels[(y*bitmap->w + x)].a = 255;
        } 
    };
    _RECTANGLE rect = {a_x, a_y, b_x, b_y, color};
    return rect;
}

_BITMAP* MERGE_BITMAPS(
                    _BITMAP* parent,
                    _BITMAP* child,
                    uint16_t child_x,
                    uint16_t child_y
) {
    if (child->w < parent->w && child->h < parent->h) { 
        for (int y = child_y; y < parent->h; ++y) {
            for (int x = child_x; x < parent->w; ++x) {
                parent->pixels[(y * parent->w) + x].r = child->pixels[(y * child->w)].r;
                parent->pixels[(y * parent->w) + x].g = child->pixels[(y * child->w)].g;
                parent->pixels[(y * parent->w) + x].b = child->pixels[(y * child->w)].b;
                parent->pixels[(y * parent->w) + x].a = child->pixels[(y * child->w)].a;
        }
    } 
} else {

    REPORT_ERROR();
    return parent;
}
}


_BITMAP CREATE_BITMAP(
                    uint16_t w, uint16_t h,
                    _COLOR color
) {
    _PIXEL* pixels = (_PIXEL*)malloc(w*h*sizeof(_PIXEL));
    _BITMAP bmp = {pixels, w, h};

    for (uint32_t i = 0; i < (w*h); ++i) {
        bmp.pixels[i].r = color.r;
        bmp.pixels[i].g = color.g;
        bmp.pixels[i].b = color.b;
        bmp.pixels[i].a = 255;

    }; return bmp;
} 

uint32_t distance_from(
                    uint16_t a_x, uint16_t a_y,
                    uint16_t b_x, uint16_t b_y
) {
    uint32_t diff = larger_of(a_x, b_x) - smaller_of(a_x, b_x);
    uint32_t distance = (uint32_t)sqrt(diff*diff);
    return distance;
}

inline uint16_t larger_of(uint16_t a, uint16_t b) { return (a > b) ? a : b; }
inline uint16_t smaller_of(uint16_t a, uint16_t b) { return (a < b) ? a : b; }

#endif // COLUMBITS_H
