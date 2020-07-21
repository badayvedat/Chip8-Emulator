#include "Platform.h"
#include <iostream>

Platform::Platform() {
	window = nullptr;
	renderer = nullptr;

	beepSound = nullptr;

	RedValue = 238;
	GreenValue = 169;
	BlueValue = 144;
}

Platform::~Platform() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	Mix_FreeChunk(beepSound);
	Mix_Quit();

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
	SDL_SetRenderDrawColor(renderer, RedValue, GreenValue, BlueValue, 255);
	
	return true;
}

bool Platform::setAudio(const char* fileName) {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "SDL_mixer could not be initalized" << '\n';
		return false;
	}

	beepSound = Mix_LoadWAV(fileName);
	if (beepSound == nullptr) {
		std::cerr << "Failed to load sound file!" << '\n';
		return false;
	}

	beepSound->volume = 4;

	return true;
}

void Platform::handleAudio(unsigned short soundTimer) {
	if (soundTimer > 0) {
		Mix_PlayChannel(-1, beepSound, 0);
	}
}

void Platform::setColors(int Red, int Green, int Blue) {
	RedValue = Red;
	GreenValue = Green;
	BlueValue = Blue;
}

void Platform::drawGraphics(unsigned char* display, const int width, const int height) {
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = 1;
		rect.h = 1;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, RedValue, GreenValue, BlueValue, 0);

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

bool Platform::handleInput(unsigned char* key) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		} 
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: {
					return false;
				}
				case SDLK_1: {
					key[0x1] = 1;
					break;
				}
				case SDLK_2: {
					key[0x2] = 1;
					break;
				}
				case SDLK_3: {
					key[0x3] = 1;
					break;
				}
				case SDLK_4: {
					key[0xC] = 1;
					break;
				}
				case SDLK_q: {
					key[0x4] = 1;
					break;
				}
				case SDLK_w: {
					key[0x5] = 1;
					break;
				}
				case SDLK_e: {
					key[0x6] = 1;
					break;
				}
				case SDLK_r: {
					key[0xD] = 1;
					break;
				}
				case SDLK_a: {
					key[0x7] = 1;
					break;
				}
				case SDLK_s: {
					key[0x8] = 1;
					break;
				}
				case SDLK_d: {
					key[0x9] = 1;
					break;
				}
				case SDLK_f: {
					key[0xE] = 1;
					break;
				}
				case SDLK_z: {
					key[0xA] = 1;
					break;
				}
				case SDLK_x: {
					key[0x0] = 1;
					break;
				}
				case SDLK_c: {
					key[0xB] = 1;
					break;
				}
				case SDLK_v: {
					key[0xF] = 1;
					break;
				}
			}
			break;
		}
		else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
				case SDLK_1: {
					key[0x1] = 0;
					break;
				}
				case SDLK_2: {
					key[0x2] = 0;
					break;
				}
				case SDLK_3: {
					key[0x3] = 0;
					break;
				}
				case SDLK_4: {
					key[0xC] = 0;
					break;
				}
				case SDLK_q: {
					key[0x4] = 0;
					break;
				}
				case SDLK_w: {
					key[0x5] = 0;
					break;
				}
				case SDLK_e: {
					key[0x6] = 0;
					break;
				}
				case SDLK_r: {
					key[0xD] = 0;
					break;
				}
				case SDLK_a: {
					key[0x7] = 0;
					break;
				}
				case SDLK_s: {
					key[0x8] = 0;
					break;
				}
				case SDLK_d: {
					key[0x9] = 0;
					break;
				}
				case SDLK_f: {
					key[0xE] = 0;
					break;
				}
				case SDLK_z: {
					key[0xA] = 0;
					break;
				}
				case SDLK_x: {
					key[0x0] = 0;
					break;
				}
				case SDLK_c: {
					key[0xB] = 0;
					break;
				}
				case SDLK_v: {
					key[0xF] = 0;
					break;
				}
			}
			break;
		}
	}

	return true;
}

