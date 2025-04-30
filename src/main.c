#include "display.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

bool is_running = false;

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

void update(void) {

}

void render(void) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_grid(0x0000FFFF);
	draw_rect(100, 100, 200, 100, 0x00FF00FF);

	const int rect_pos_x = 80;
	const int rect_pos_y = 80;

	const int rect_width = 40;
	const int rect_height = 40;

	for (int i = rect_pos_x; i < rect_pos_x + rect_width; i++)
	{
		for (int j = rect_pos_y; j < rect_pos_y + rect_height; j++)
		{
			draw_pixel(i, j, 0xFF00FFFF);
		}
	}

	render_color_buffer();
	clear_color_buffer(0xFFFF00FF);

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
