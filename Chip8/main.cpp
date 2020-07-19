#include "Chip8.h"
#include "Platform.h"

int main(int argc, char* argv[]) {
	Chip8 chip8;
	chip8.initialize();
	chip8.loadGame("test_opcode.ch8");

	Platform platform;
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