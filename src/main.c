#include "display.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "vector.h"

const int N_POINTS = 9 * 9 * 9;

bool is_running = false;
float fov_scale = 250.0f;
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

void setup_cube_points(void) {
	float step = 2 / (float)9;
	size_t index = 0;

	for (size_t x = 0; x < 9; x++) {
		for (size_t y = 0; y < 9; y++) {
			for (size_t z = 0; z < 9; z++) {
				vec3_t point;
				point.x = (x * step) - 1;
				point.y = (y * step) - 1;
				point.z = (z * step) - 1;
				cube_points[index] = point;
				index++;
			}
		}
	}
}

void setup(void) {
	// Allocate memory for collor buffer
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	
	// Allocate rendering buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	// Fill the cube_points array with points in the range [-1, 1]
	setup_cube_points();
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type) {
		case SDL_EVENT_QUIT:
			is_running = false;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.key == SDLK_ESCAPE) {
				is_running = false;
			}
			break;
	}
}

vec2_t project(vec3_t point) {
	vec2_t projected_point = { point.x, point.y };
	return projected_point;
}

void update(void) {
	for (size_t i = 0; i < N_POINTS; i++)
	{
		vec3_t point = cube_points[i];
		vec2_t projected_point = project(point);
		projected_point.x = (projected_point.x * fov_scale) + (window_width / 2);
		projected_point.y = (projected_point.y * fov_scale) + (window_height / 2);
		projected_points[i] = projected_point;
	}
}

void render(void) {
	draw_grid(0x0000FFFF);

	for (size_t i = 0; i < N_POINTS; i++)
	{
		vec2_t point = projected_points[i];
		draw_rect(point.x, point.y, 4, 4, 0xFFFF00FF);
	}

	render_color_buffer();
	clear_color_buffer(0x000000FF);

	SDL_RenderPresent(renderer);
}

int main(void) {
	// Initialization
	is_running = initialize_window();
	setup();

	// Game Loop
	while(is_running) {
		process_input();
		update();
		render(); 
	}	

	destroy_window();
	return 0;
}
