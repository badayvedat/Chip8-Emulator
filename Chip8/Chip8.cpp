#include <iostream>
#include <fstream>
#include <algorithm>

#include "Chip8.h"

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
	//AND opcode with 0xF000 to get first 4 bits.
	switch (opcode & 0xF000) {
		case 0x0000:
			// AND opcode with 0x0FF to get last 8 bits
			switch (opcode & 0x0FF) {
				// Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN. Not necessary for most ROMs.
				case 0x0000:
					break;

				// Clears the screen
				case 0x00E0:
					for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
						gfx[i] = 0;
					}
					drawFlag = true;
					pc += 2;
					break;

				// Returns from a subroutine
				case 0x00EE:
					--sp;
					pc = stack[sp];

				default:
					std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
					break;
			}
		
		// Jumps to address NNN
		case 0x1000:
			pc = (opcode & 0x0FFF);

		// Calls subroutine at NNN
		case 0x2000:
			stack[sp] = pc;
			sp++;
			pc = (opcode & 0x0FFF);

		// Skips the next instruction if VX equals NN
		case 0x3000:
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
				pc += 4;
			} else {
				pc += 2;
			}

		default:
			std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
			break;
	}
}
