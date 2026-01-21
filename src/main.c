#include "display.h"
#include <math.h>
#include "vector.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>

const int CUBE_SIZE = 9; // 3x3x3 grid
const int N_POINTS = CUBE_SIZE * CUBE_SIZE * CUBE_SIZE; // Total number of points in the cube
const int CUBE_FACES = 12; // Number of faces in a cube
const int CUBE_VERTICES = 8; // Number of vertices in a cube

typedef struct {
	int a;
	int b;
	int c;
} face_t;

bool is_running = false;
float fov_scale = 250.0f;
vec3_t camera_position = { 0, 0, -2 };
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
vec3_t cube_vertices[CUBE_VERTICES] = {
	{ -1, -1, -1 },
	{  1, -1, -1 },
	{  1,  1, -1 },
	{ -1,  1, -1 },
	{ -1, -1,  1 },
	{  1, -1,  1 },
	{  1,  1,  1 },
	{ -1,  1,  1 }
};

face_t cube_faces[CUBE_FACES] = {
	{ 0, 1, 2 }, { 0, 2, 3 }, // Back face
	{ 4, 5, 6 }, { 4, 6, 7 }, // Front face
	{ 0, 1, 5 }, { 0, 5, 4 }, // Bottom face
	{ 2, 3, 7 }, { 2, 7, 6 }, // Top face
	{ 1, 2, 6 }, { 1, 6, 5 }, // Right face
	{ 0, 3, 7 }, { 0, 7, 4 }  // Left face
};

void setup_cube_points(void) {
	float step = 2 / (float)(CUBE_SIZE - 1); // Step size to fill the range [-1, 1]
	size_t index = 0;

	for (size_t x = 0; x < CUBE_SIZE; x++) {
		for (size_t y = 0; y < CUBE_SIZE; y++) {
			for (size_t z = 0; z < CUBE_SIZE; z++) {
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

vec2_t orthographic_projection(vec3_t point) {
	vec2_t projected_point = { point.x * fov_scale, point.y * fov_scale };
	return projected_point;
}

vec2_t perspective_projection(vec3_t point) {
	vec2_t projected_point;
	projected_point.x = (point.x * fov_scale) / (point.z);
	projected_point.y = (point.y * fov_scale) / (point.z);
	return projected_point;
}

void update(void) {
	SDL_Delay(FRAME_TARGET_TIME);

	int current_frame = SDL_GetTicks();
	float angle = current_frame * 0.001f; // Rotate based on time

	for (size_t i = 0; i < CUBE_VERTICES; i++)
	{
		vec3_t point = cube_vertices[i];

		// Rotate object in 3 axis
		// point = rotate_around_x(point, angle);
		point = rotate_around_y(point, angle);
		// point = rotate_around_z(point, angle);

		point.x += camera_position.x;
		point.y += camera_position.y;
		point.z += camera_position.z;

		// Apply perspective projection
		vec2_t projected_point = perspective_projection(point);
		projected_point.x = projected_point.x + (window_width / 2);
		projected_point.y = projected_point.y + (window_height / 2);
		projected_points[i] = projected_point;
	}
}

void debug_draw_lines(void) {
	// float angle = SDL_GetTicks() * 0.001f;
	// angle = fmodf(angle, 2.0f * M_PI);

	for(u_int8_t i = 0; i < 8; i++) {
		float angle = i * M_PI / 4;
		int origin_x = 600;
		int origin_y = 600;
		int circle_pos_x = (400 - origin_x) * cos(angle) - (400 - origin_y) * sin(angle);
		int circle_pos_y = (400 - origin_y) * cos(angle) + (400 - origin_x) * sin(angle);
		circle_pos_x += origin_x;
		circle_pos_y += origin_y;

		draw_line(origin_x, origin_y, circle_pos_x, circle_pos_y, 0xFFFF00FF); // yellow line
	}
}

void render(void) {
	// draw_grid(0x0000FFFF);

	// for (size_t i = 0; i < N_POINTS; i++)
	// {
	// 	vec2_t point = projected_points[i];
	// 	draw_rect(point.x, point.y, 5, 5, 0xFFFF00FF);
	// }

	const int point_size = 4;

	for (size_t i = 0; i < CUBE_VERTICES; i++) {
		vec2_t p = projected_points[i];
		draw_rect(p.x - point_size, p.y - point_size, point_size * 2, point_size * 2, 0xFF00FFFF);
	}

	// debug_draw_lines();

	for (size_t i = 0; i < CUBE_FACES; i++) {
		face_t face = cube_faces[i];
		vec2_t p1 = projected_points[face.a];
		vec2_t p2 = projected_points[face.b];
		vec2_t p3 = projected_points[face.c];

		draw_line(p1.x, p1.y, p2.x, p2.y, 0xFF00FFFF);
		draw_line(p2.x, p2.y, p3.x, p3.y, 0xFF00FFFF);
		draw_line(p3.x, p3.y, p1.x, p1.y, 0xFF00FFFF);
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
