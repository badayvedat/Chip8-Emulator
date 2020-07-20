#include "Chip8.h"
#include "Platform.h"
#include "Scheduler.h"

#include <iostream>

int main(int argc, char* argv[]) {
	Chip8 chip8;
	Platform platform;
	
	std::string fileName = "snake.ch8";

	if (!chip8.loadGame(fileName)) {
		std::cerr << "Could not load file " << fileName << '\n';
		return -1;
	}

	if (!platform.init("CHIP-8 EMULATOR", 960, 480)) {
		std::cerr << "An error occured while initalizing Platform" << '\n';
		return -1;
	}

	if (!platform.setAudio("sfx1.wav")) {
		std::cerr << "Could not set Audio File!" << '\n';
		return -1;
	}

	// For most of Chip-8 games 500Hz is a good delay;
	Scheduler cycleScheduler(500);
	
	// Sound and delay timer for chip8 decreases at 60hz no matter what cycle refresh rate is.
	Scheduler delayScheduler(60);

	bool quit = false;

	while (!quit) {

		// Since it's delay value not used we choose an arbitrary integer.
		Scheduler currTime(-1);

		if (cycleScheduler.run(currTime)) {
			chip8.emulateCycle();

			if (chip8.getDrawFlag()) {
				platform.drawGraphics(chip8.getGfx(), 64, 32);
				chip8.setDrawFlag(false);
			}

			if (!platform.handleInput(chip8.getKeys())) {
				quit = true;
			}
			
			// Audio will be handled in every cycle
			platform.handleAudio(chip8.getSoundTimer());

			cycleScheduler.setTime(currTime.getTime());
		}

		if (delayScheduler.run(currTime)) {
			if (chip8.getSoundTimer() > 0) {
				chip8.decreaseSoundTimer();
			}

			if (chip8.getDelayTimer() > 0) {
				chip8.decreaseDelayTimer();
			}

			delayScheduler.setTime(currTime.getTime());
		}
	}

	return 0;
}