#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL3/SDL.h>

extern int window_width;
extern int window_height;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

bool initialize_window(void);
void setup(void);
void destroy_window(void);
void draw_grid(uint32_t lineColor);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int pos_x, int pos_y, int width, int height, uint32_t color);
void render_color_buffer();
void clear_color_buffer(uint32_t color);

#endif