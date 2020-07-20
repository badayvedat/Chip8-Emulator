#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

class Platform {
public:
	Platform();
	~Platform();

	bool init(const char* title, int width, int height);
	void drawGraphics(unsigned char* display, const int width, const int height);
	bool handleInput(unsigned char* key);

	bool setAudio(const char* fileName);
	void handleAudio(unsigned short soundTimer);


private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Mix_Chunk* beepSound;
};

