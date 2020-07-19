#include "Platform.h"
#include <iostream>

Platform::Platform() {
	window = nullptr;
	renderer = nullptr;
}

Platform::~Platform() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Platform::init(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not be initalized" << '\n';
		return false;
	}
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, 64, 32);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	return true;
}

void Platform::drawGraphics(unsigned char* display, const int width, const int height) {
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = 1;
		rect.h = 1;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 238, 169, 144, 0);

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
}

