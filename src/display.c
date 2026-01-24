#include <stdlib.h>
#include <SDL3/SDL.h>
#include "line.h"

int window_width = 800;
int window_height = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
LineAlgorithm current_line_algorithm = BRESENHAM;

bool initialize_window(void) {
	fprintf(stdout, "Window Initialization");
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Error initializing SDL!");
		return false;
	}

	const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(1);
	window_width = display_mode->w;
	window_height = display_mode->h;
	fprintf(stdout, "Window size: %d x %d :: %d\n", display_mode->w, display_mode->h, display_mode->displayID);

	// Create SDL Window
	window = SDL_CreateWindow(
		"3d renderer",
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL window!");
		return false;
	}

	SDL_SetWindowFullscreen(
		window,
		SDL_WINDOW_FULLSCREEN
	);

	// Create SDL renderer
	renderer = SDL_CreateRenderer(
		window,
		NULL
		);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer!");
		return false;
	}

	return true;
}

void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t))	
	);

	SDL_RenderTexture(
		renderer,
		color_buffer_texture,
		NULL,
		NULL
	);
}

void clear_color_buffer(uint32_t color) {
	for(int y = 0; y < window_height; y++) {
		for(int x = 0; x < window_width; x++) {
			color_buffer[window_width * y + x] = color;
		}
	}
}

void draw_grid(uint32_t lineColor) {
	const int line_offset = 50;

	for (int y = 0; y < window_height;)
	{
		for (int x = 0; x < window_width; x++)
		{
			color_buffer[window_width * y + x] = lineColor;
		}
		
		y += line_offset;
		if (y >= window_height) {
			break;
		}
	}

	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width;)
		{
			color_buffer[window_width * y + x] = lineColor;
			x += line_offset;

			if (x >= window_width) {
				break;
			}
		}
	}
}

void draw_pixel(int x, int y, uint32_t color) {
	if (x < 0 || x >= window_width || y < 0 || y >= window_height) {
		return;
	}

	color_buffer[window_width * y + x] = color;
}

void draw_rect(int pos_x, int pos_y, int width, int height, uint32_t color) {
	width = pos_x + width;
	height = pos_y + height;

	if (width > window_width) {
		width = window_width;
	}
	if (height > window_height) {
		height = window_height;
	}

	for (int y = pos_y; y < height; y++) {
		for (int x = pos_x; x < width; x++) {
			draw_pixel(x, y, color);
		}
	}
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
	switch (current_line_algorithm)
	{
		default:
		case BRESENHAM:
			draw_line_bresenham(x0, y0, x1, y1, color);
			break;
		case WU:
			draw_line_wu(x0, y0, x1, y1, color);
			break;
	}
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}