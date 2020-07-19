#pragma once
#include <SDL.h>

class Platform {
public:
	Platform();
	~Platform();

	bool init(const char* title, int width, int height);
	void drawGraphics(unsigned char* display, const int width, const int height) {
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = 1;
		rect.h = 1;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (display[(y * width) + x]) {
					rect.x = x;
					rect.y = y;

					SDL_RenderFillRect(renderer, &rect);
				}
			}
		}
		SDL_RenderPresent(renderer);
		// set drawFlag to false

	}

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};

