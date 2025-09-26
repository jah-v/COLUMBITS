#ifndef COLUMBITS2_H
#define COLUMBITS2_H

#ifdef NO_STDIO
#else
#include <stdio.h>
#endif

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*          Types           */

    #ifdef THREE_DIMENSIONAL
    typedef struct { 
        uint16_t x, y, z; 
    } point;
    #else 
    typedef struct {
        uint16_t x, y, rx, ry;
    } point;
    #endif

    typedef struct {
        point pos;
        ray* next;
        uint8_t curve_t;
    } ray;

    typedef struct {
        ray* lines;
        uint16_t count;
    } shape;

    typedef struct {
        uint8_t* a;
        uint16_t width, height;
    } texture;

    typedef struct {
        uint8_t r, g, b, a; // "a" will also serve as an enum for flags
    } rgba;

    typedef struct {
        rgba* pixels;
        uint16_t width, height;
    } bitmap;

/*                          */

/*         Functions        */

    /* Bitmap */

    bitmap NEW_BITMAP(
        uint16_t width, uint16_t height,
        rgba color
    );
    bitmap MERGE_BITMAP(
        bitmap* parent, bitmap* child,
        point p
    );
    bitmap SCALE_BITMAP(
        bitmap* parent, 
        point scale_f
    );
    bitmap BITMAP_TO_FILE(
        bitmap* parent, char* file_path
    );
    bitmap BITMAP_FROM_FILE(
        char* file_path
    );
    void KILL_BITMAP(
        bitmap* parent
    );


    /* Texture */

    texture NEW_TEXTURE(
        uint16_t width, uint16_t height,
        rgba color
    );
    texture MERGE_TEXTURE(
        texture* parent, texture* child, 
        point p
    );
    texture SCALE_TEXTURE(
        texture* parent,
        point scale_f
    );
    texture KILL_TEXTURE(
        texture* parent
    );
    bitmap MULTIPLTY_TEXTURE(
        texture* a,
        texture* b
        color a,
        color b,
        uint8_t multiply_t
    );

    /* Drawing */

    bitmap DRAW_TEXTURE(
        bitmap* parent, 
        point p, 
        rgba color
    );
    bitmap DRAW_RECTANGLE(
        bitmap* parent,
        point a, point b,
        rgba color
    );
    bitmap DRAW_CIRCLE(
        bitmap* parent,
        point p,
        uint16_t radius,
        rgba color
    );
    bitmap DRAW_ROUNDTANGLE(
        bitmap* parent,
        point a, point b,
        uint8_t rotundity,
        rgba color
    );
    bitmap DRAW_TRIANGLE(
        bitmap* parent, 
        point a, point b, point c,
        rgba color
    );
    bitmap DRAW_SHAPE(
        bitmap* parent, 
        shape shape,
        rgba color
    );

    /* Other */

    float linear_from(
        point a, point b, point compare, 
        uint8_t transform_t;
    );
    float quadratic_from(
        point a, point b, point compare, 
        uint8_t transform_t;
    );
    float cubic_from(
        point a, point b, point compare, 
        uint8_t transform_t;
    );
    float sqrt_from(
        point a, point b, point compare, 
        uint8_t transform_t;
    );
    float inverse_sqrt_from(
        point a, point b, point compare, 
        uint8_t transform_t;
    );

/*      Implementation      */

    bitmap MERGE_BITMAP(
        bitmap* parent, bitmap* child, point p
    ) {
        for (uint16_t y = p.y; y < height; ++y) {
            for (uint16_t x = p.x; x < width; ++x) {
                parent->pixels[(y * parent->width) + x] = child->pixels[((y-p.y) * child->width) + (x-p.x)];
            }
        }
        return *parent;
    }

    bitmap NEW_BITMAP(
        uint16_t width, uint16_t height, 
        rgba color
    ) {
        bitmap BMP = {width, height, nullptr};
        BMP.pixels = (rgba*)malloc(width*height*sizeof(rgba));
        if (!BMP.pixels) { return; }
        for (uint16_t y = 0; y < height; ++y) {
            for (uint16_t x = 0; x < width; ++x) {
                BMP.pixels[(y * width) + x] = color;
            }
        }
        return BMP;
    }






#endif // COLUMBITS2_H