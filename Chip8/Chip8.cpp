#include "Chip8.h"

#include <iostream>
#include <fstream>
#include <algorithm>

void Chip8::initialize() {
	pc = 0x200;		// Program counter starts at 0x200
	I = 0;			// Reset current opcode
	sp = 0;			// Reset index register
	opcode = 0;		// Reset stack pointer

	// Clear memory
	std::fill(memory, memory + TOTAL_MEMORY, 0);
	
	// Clear stack
	std::fill(stack, stack + 16, 0);
	
	// Clear display
	std::fill(gfx, gfx + (SCREEN_HEIGHT * SCREEN_WIDTH), 0);
	
	// Clear registers
	std::fill(V, V + 16, 0);

	// Load fontset
	for (int i = 0; i < 80; i++) {
		memory[i] = fontset[i];
	}

	// Reset timers
	delay_timer = 0;
	sound_timer = 0;
}

bool Chip8::loadGame(std::string fileName) {
	std::ifstream ifs(fileName, std::ios::binary);
	if (ifs) {
		ifs.seekg(0, ifs.end);
		int bufferSize = static_cast<int>(ifs.tellg());
		ifs.seekg(0, ifs.beg);

		char* buffer = new char[bufferSize];
		ifs.read(buffer, bufferSize);

		// Free Memory of Chip-8 starts from 0x200
		for (int i = 0; i < bufferSize; i++) {
			memory[i + 0x200] = buffer[i];;
		}
		
		delete[] buffer;

		if (ifs) {
			return true;
		} else {
			std::cout << "Unable to read contents of " << fileName << '\n';
			return false;
		}
	} else {
		std::cout << "Unable to open " << fileName << '\n';
		return false;
	}
}
void Chip8::emulateCycle() {
	// Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];
	
	// Decode opcode
	switch (opcode & 0xF000) {
	case 0xA000:
		I = opcode & 0x0FFF;
		pc += 2;
		break;
	default:
		std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
	}
}
