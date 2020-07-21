#include "Chip8.h"
#include "Platform.h"
#include "Scheduler.h"
#include "ConfigManager.h"

#include <iostream>

int main(int argc, char* argv[]) {
	/*
	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <RomPath> <CycleRefreshRate> <Scale>\n"
			<< "For Most Roms 500 would be nice cycle refresh rate, i prefer 1000.\n"
			<< "Chip8's original width is 64 and height is 32. Based on your Scale value it will be width * scale and height * scale.\n";
		return -1;
	}
	*/

	Chip8 chip8;
	Platform platform;

	ConfigManager configManager;
	if (!configManager.loadConfigFile()) {
		std::cerr << "Error occured while parsing config file" << '\n';
		return -1;
	}
	/*
	std::string fileName = argv[1];
	int cycleRefreshRate = std::stoi(argv[2]);
	int scale = std::stoi(argv[3]);
	*/

	if (!chip8.loadGame(configManager.getRomName())) {
		std::cerr << "Could not load file " << configManager.getRomName() << '\n';
		return -1;
	}

	// Original height of CHIP8 is equal to 32 and width is equal to 64.
	if (!platform.init("CHIP-8 EMULATOR", 64 * configManager.getScale(), 32 * configManager.getScale())) {
		std::cerr << "An error occured while initalizing Platform" << '\n';
		return -1;
	}

	if (!platform.setAudio("SFX/sfx1.wav")) {
		std::cerr << "Could not set Audio File!" << '\n';
		return -1;
	}

	/*
	For most of Chip-8 games 500Hz is a good delay;
	But i choose to play with 1000Hz, feels smoother.
	*/
	Scheduler cycleScheduler(configManager.getCycleRefreshRate());
	
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