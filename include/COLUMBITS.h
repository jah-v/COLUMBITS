#ifndef COLUMBITS_H
#define COLUMBITS_H

#include <stdlib.h>
#include <stdint.h>
#include <cmath>

typedef struct {    uint8_t r, g, b, flags;     } _COLOR;
typedef struct {    uint8_t r, g, b, a;         } _PIXEL;
typedef struct {    _PIXEL* pixels;
                    uint16_t w, h;              } _BITMAP;
typedef struct {    uint16_t a_x, a_y, b_x, b_y;
                    _COLOR color;               } _RECTANGLE;
typedef struct {    char* c; 
                    uint16_t char_length;
                    uint16_t font_size;
                    uint8_t font, style;        } _TEXT;

uint16_t larger_of(
                    uint16_t a, uint16_t b
);

uint16_t smaller_of(
                    uint16_t a, uint16_t b
);

uint32_t distance_from(
                    uint16_t a_x, uint16_t a_y,
                    uint16_t b_x, uint16_t b_y
);

_BITMAP CREATE_BITMAP(
                    uint16_t w, uint16_t h,
                    _COLOR color
);

_BITMAP* SCALE_BITMAP(
                    _BITMAP* bitmap,
                    float scale,
                    uint8_t flags
);

_BITMAP* MERGE_BITMAPS(
                    _BITMAP* interior,
                    _BITMAP* exterior,
                    uint8_t flags
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
                    _COLOR color
);

_TEXT* NEW_TEXT(
                    char* c,
                    uint16_t length,
                    uint16_t font_size,
                    uint8_t font,
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
                    _COLOR color
) {
    c_x = smaller_of(a_x, b_x);
    c_y = smaller_of(a_y, b_y);
    d_x = larger_of(a_x, b_x);
    d_y = larger_of(a_y, b_y);

    uint16_t radius = (d_y - cy) / 256) * (roundness / 2);

    DRAW_RECTANGLE( c_x + radius, c_y + radius,
                    d_x - radius, d_y - radius,
                    bitmap, color);

    if (roundness = 255) {
        DRAW_CIRCLE(c_x + radius, c_y + radius, 
                    radius, bitmap, color);
        DRAW_CIRCLE(d_x - radius, c_y + radius, 
                    radius, bitmap, color);
    } else {
        DRAW_CIRCLE(c_x + radius, c_y + radius, 
                    radius, bitmap, color);
        DRAW_CIRCLE(d_x - radius, c_y + radius, 
                    radius, bitmap, color);
        DRAW_CIRCLE(c_x + radius, d_y - radius, 
                    radius, bitmap, color);
        DRAW_CIRCLE(d_x - radius, d_y - radius, 
                    radius, bitmap, color);
    }
    return bitmap;
}


_BITMAP* DRAW_CIRCLE(
                    uint16_t x, uint16_t y,
                    uint16_t radius,
                    _BITMAP* bitmap, _COLOR color
) {
    for (int b = (y - radius); b < (y + radius); ++b) {
        for (int a = (x - radius); a < (x + radius); ++a) {
            if (distance_from(a, b, x, y) < radius) {
                bitmap->pixels[(b * bitmap->w) + a].r = color.r;
                bitmap->pixels[(b * bitmap->w) + a].g = color.g;
                bitmap->pixels[(b * bitmap->w) + a].b = color.b;
                bitmap->pixels[(b * bitmap->w) + a].a = 255;
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
            bitmap->pixels[(y*bitmap->w + x)].a = 256;
        } 
    };
    _RECTANGLE rect = {a_x, a_y, b_x, b_y, color};
    return rect;
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

    };

    return bmp;
} 

uint32_t distance_from(
                    uint16_t a_x, uint16_t a_y,
                    uint16_t b_x, uint16_t b_y
) {
    uint32_t diff = larger_of(a_x, b_x) - smaller_of(a_x, b_x);
    uint32_t distance = (uint32_t)sqrt(diff*diff);
    return distance;
}

uint16_t larger_of(
                    uint16_t a, uint16_t b
) {
    if (a < b) { return b; }
    else if (a > b) { return a; }
    else if (a == b) { return 0; }
}

uint16_t smaller_of(
                    uint16_t a, uint16_t b
) {
    if (a < b) { return a; }
    else if (a > b) { return b; }
    else if (a == b) { return 0; }
}

#endif // COLUMBITS_H