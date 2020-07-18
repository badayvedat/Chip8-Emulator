#include "Chip8.h"

int main() {
	// setupGraphics()
	// setupInput()
	Chip8 chip8;
	chip8.initialize();
	chip8.loadGame("test_opcode.ch8");
	for (int i = 0; i < 100; i++) {
		chip8.emulateCycle();
	}
	
	/*
		// Set up render system and register input callbacks
		setupGraphics();
		setupInput();

		// Initialize the Chip8 system and load the game into the memory  
		myChip8.initialize();
		myChip8.loadGame("pong");

		// Emulation loop
		for (;;) {
			// Emulate one cycle
			myChip8.emulateCycle();

			// If the draw flag is set, update the screen
			if (myChip8.drawFlag)
				drawGraphics();

			// Store key press state (Press and Release)
			myChip8.setKeys();
		}

		return 0;
	*/
}