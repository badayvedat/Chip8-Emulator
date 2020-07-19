#pragma once
#include <SDL.h>

class Platform {
public:
	Platform();
	~Platform();

	bool init(const char* title, int width, int height);
	void drawGraphics(unsigned char* display, const int width, const int height);
	bool handleInput(unsigned char* key);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
};

