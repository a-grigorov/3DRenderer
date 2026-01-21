#ifndef LINE_H
#define LINE_H
#include "stdlib.h"

typedef enum LineAlgorithm {
    BRESENHAM,
    WU
} LineAlgorithm;

void draw_line_bresenham(int x0, int y0, int x1, int y1, uint32_t color);
void draw_line_wu(int x0, int y0, int x1, int y1, uint32_t color);

#endif