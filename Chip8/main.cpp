#include "Chip8.h"
#include "Platform.h"

#include <iostream>

int main(int argc, char* argv[]) {
	Chip8 chip8;
	Platform platform;
	
	chip8.initialize();
	chip8.loadGame("IBM Logo.ch8");

	/*
	if (!chip8.loadGame(fileName)) {
		std::cerr << "Could not load file " << filename << '\n';
		return -1;
	}

	if (!platform.init("CHIP-8 EMULATOR", 640, 320)) {
		std::cerr << "An error occured while initalizing Platform" << '\n';
		return -1;
	}
	*/

	platform.init("TEST", 640, 320);

	while (true) {
		chip8.emulateCycle();
		
		if (chip8.getDrawFlag()) {
			platform.drawGraphics(chip8.getGfx(), 64, 32);
			chip8.setDrawFlag(false);
		}
		
		// setupInput()

	}
	return 0;
}