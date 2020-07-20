#include "Chip8.h"
#include "Platform.h"

#include <iostream>

int main(int argc, char* argv[]) {
	Chip8 chip8;
	Platform platform;
	
	std::string fileName = "IBM Logo.ch8";

	if (!chip8.loadGame(fileName)) {
		std::cerr << "Could not load file " << fileName << '\n';
		return -1;
	}

	if (!platform.init("CHIP-8 EMULATOR", 640, 320)) {
		std::cerr << "An error occured while initalizing Platform" << '\n';
		return -1;
	}

	if (!platform.setAudio("sfx1.wav")) {
		std::cerr << "Could not set Audio File!" << '\n';
		return -1;
	}

	bool quit = false;

	while (!quit) {
		chip8.emulateCycle();
		
		if (chip8.getDrawFlag()) {
			platform.drawGraphics(chip8.getGfx(), 64, 32);
			chip8.setDrawFlag(false);
		}
		
		if (!platform.handleInput(chip8.getKeys())) {
			quit = true;
		}

		platform.handleAudio(chip8.getSoundTimer());

	}
	return 0;
}