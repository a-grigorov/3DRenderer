#include "stdlib.h"
#include "display.h"

void draw_line_bresenham_h(int x0, int y0, int x1, int y1, uint32_t color) {
	if (x0 > x1)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;

		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = 2 * dy - dx;
	int y = y0;
	
	int sy = (y0 < y1) ? 1 : -1;
	dy *= sy;

	for (size_t i = x0; i < x1; i++)
	{
		draw_pixel(i, y, color);
		if (d > 0)
		{
			y = y + sy;
			d = d - 2 * dx;
		}

		d = d + 2 * dy;
	}
}

void draw_line_bresenham_v(int x0, int y0, int x1, int y1, uint32_t color) {
	if (y0 > y1)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;

		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = 2 * dy - dx;
	int x = x0;
	
	int sx = (x0 < x1) ? 1 : -1;
	dx *= sx;

	for (size_t i = y0; i < y1; i++)
	{
		draw_pixel(x, i, color);
		if (d > 0)
		{
			x = x + sx;
			d = d - 2 * dy;
		}

		d = d + 2 * dx;
	}
}

void draw_line_wu_h(int x0, int y0, int x1, int y1, uint32_t color) {
    if(x1 < x0)
    {
		int temp = x0;
		x0 = x1;
		x1 = temp;

		temp = y0;
		y0 = y1;
		y1 = temp;
    }

	int dx = x1 - x0;
	int dy = y1 - y0;
    float m = dx != 0 ? dy / (float)dx : 1;

    float overlap = 1 - (x0 + 0.5f) - (int)(x0 + 0.5f);
    float dist_start = y0 - (int)y0;

    uint32_t color_top = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)((1.0f - dist_start) * overlap * 255);
    uint32_t color_bottom = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)(dist_start * overlap * 255);

    draw_pixel((int)(x0 + 0.5f), y0, color_top);
    draw_pixel((int)(x0 + 0.5f), y0 + 1, color_bottom);

    float overlap_end = (x1 - 0.5f) - (int)(x1 + 0.5f);
    float dist_end = y1 - (int)y1;

    color_top = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)((1.0f - dist_end) * overlap_end * 255);
    color_bottom = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)(dist_end * overlap_end * 255);

    draw_pixel((int)(x1 + 0.5f), y1, color_top);
    draw_pixel((int)(x1 + 0.5f), y1 + 1, color_bottom);

    for (size_t i = 1; i < (int)(dx + 0.5f) + 1; i++)
    {
        float x = x0 + i;
        float y = y0 + m * i;

        int ix = (int)x;
        int iy = (int)y;

        float dist = y - iy;

        uint32_t color_top = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)((1.0f - dist) * 255);
        uint32_t color_bottom = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)(dist * 255);

        draw_pixel(ix, iy, color_top);
        draw_pixel(ix, iy + 1, color_bottom);
    }
}

void draw_line_wu_v(int x0, int y0, int x1, int y1, uint32_t color) {
    if(y1 < y0)
    {
		int temp = x0;
		x0 = x1;
		x1 = temp;

		temp = y0;
		y0 = y1;
		y1 = temp;
    }

	int dx = x1 - x0;
	int dy = y1 - y0;
    float m = dy != 0 ? dx / (float)dy : 1;

    float overlap = 1 - (y0 + 0.5f) - (int)(y0 + 0.5f);
    float dist_start = y0 - (int)y0;

    uint32_t color_top = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)((1.0f - dist_start) * overlap * 255);
    uint32_t color_bottom = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)(dist_start * overlap * 255);

    draw_pixel((int)(x0 + 0.5f), y0, color_top);
    draw_pixel((int)(x0 + 0.5f), y0 + 1, color_bottom);

    float overlap_end = (y1 - 0.5f) - (int)(y1 - 0.5f);
    float dist_end = y1 - (int)y1;

    color_top = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)((1.0f - dist_end) * overlap_end * 255);
    color_bottom = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)(dist_end * overlap_end * 255);

    draw_pixel(x1, (int)(y1 + 0.5f), color_top);
    draw_pixel(x1 + 1, (int)(y1 + 0.5f), color_bottom);

    for (size_t i = 0; i < dy + 1; i++)
    {
        float x = x0 + m * i;
        float y = y0 + i;

        int ix = (int)x;
        int iy = (int)y;

        float dist = x - ix;

        uint32_t color_top = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)((1.0f - dist) * 255);
        uint32_t color_bottom = color >> 24 << 24 | color >> 16 << 16 | color >> 8 << 8 | (uint8_t)(dist * 255);

        draw_pixel(ix, iy, color_top);
        draw_pixel(ix + 1, iy, color_bottom);
    }
}

void draw_line_bresenham(int x0, int y0, int x1, int y1, uint32_t color) {
	if (abs(y1 - y0) < abs(x1 - x0))
		draw_line_bresenham_h(x0, y0, x1, y1, color);
	else
		draw_line_bresenham_v(x0, y0, x1, y1, color);
}

void draw_line_wu(int x0, int y0, int x1, int y1, uint32_t color) {
    if (abs(y1 - y0) < abs(x1 - x0))
        draw_line_wu_h(x0, y0, x1, y1, color);
    else
        draw_line_wu_v(x0, y0, x1, y1, color);
}