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
					break;

				default:
					std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
					break;
			}
		
		// Jumps to address NNN
		case 0x1000:
			pc = (opcode & 0x0FFF);
			break;

		// Calls subroutine at NNN
		case 0x2000:
			stack[sp] = pc;
			sp++;
			pc = (opcode & 0x0FFF);
			break;

		// Skips the next instruction if VX equals NN
		case 0x3000:
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;

		// Skips the next instruction if VX doesn't equal NN
		case 0x4000:
			if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;

		// Skips the next instruction if VX equals VY
		case 0x5000:
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;

		// Sets VX to NN
		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
			pc += 2;
			break;

		// Adds NN to VX (Carry flag is not changed)
		case 0x7000:
			V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
			pc += 2;
			break;

		case 0x8000:
			unsigned char VX = V[(opcode & 0x0F00) >> 8];
			unsigned char VY = V[(opcode & 0x00F0) >> 4];
			switch (opcode & 0x000F) {
				// Sets VX to the value of VY
				case 0x0000:
					V[(opcode & 0x0F00) >> 8] = VY;
					pc += 2;
					break;

				// Sets VX to VX or VY (Bitwise OR operation)
				case 0x0001:
					V[(opcode & 0x0F00) >> 8] = (VX | VY);
					pc += 2;
					break;

				// Sets VX to VX and VY (Bitwise AND operation)
				case 0x0002:
					V[(opcode & 0x0F00) >> 8] = (VX & VY);
					pc += 2;
					break;

				// Sets VX to VX xor VY
				case 0x0003:
					V[(opcode & 0x0F00) >> 8] = (VX ^ VY);
					pc += 2;
					break;

				// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
				case 0x0004:
					if (VX > 0xFF - VY) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}
					V[(opcode & 0x0F00) >> 8] += VY;
					pc += 2;
					break;

				// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				case 0x0005:
					if (VX < VY) {
						V[0xF] = 0;
					} else {
						V[0xF] = 1;
					}
					V[(opcode & 0x0F00) >> 8] -= VY;
					pc += 2;
					break;

				// Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
				case 0x0006:
					V[0xF] = (VX & 0x1);
					V[(opcode & 0x0F00) >> 8] >>= 1;
					pc += 2;
					break;

				// Sets VX to VY minux VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				case 0x0007:
					if (VX > VY) {
						V[0xF] = 0;
					} else {
						V[0xF] = 1;
					}
					V[(opcode & 0x0F00) >> 8] = VY - VX;
					pc += 2;
					break;

				// Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
				case 0x000E:
					V[0xF] = (VX & 0x1);
					V[(opcode & 0x0F00) >> 8] <<= 1;
					pc += 2;
					break;

				default:
					std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
					break;
			}

		// Skips the next instruction if VX doesn't equal VY.
		case 0x9000:
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;

		// Sets I to the address NNNN.
		case 0xA000:
			I = (opcode & 0x0FFF);
			pc += 2;
			break;

		// Jump to the address NNN plus V0.
		case 0xB000:
			pc = ((opcode & 0x0FFF) + V[0x000]);
			break;

		// Sets VX to the result of a bitwise and operation on a random number.
		case 0xC000:
			V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
			pc += 2;
			break;

		/*
		Wikipedia's Chip-8 DXYN Explanation:
		Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
		Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. 
		As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen. 

		Cowgod's Chip-8 DXYN Explanation:
		The interpreter reads n bytes from memory, starting at the address stored in I. 
		These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). 
		Sprites are XORed onto the existing screen. 
		If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. 
		If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. 
		See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
		*/
		case 0xD000:
			unsigned char Y = V[(opcode & 0x0F00) >> 8];
			unsigned char X = V[(opcode & 0x00F0) >> 4];
			unsigned short N = (opcode & 0x000F);
			unsigned char currentPixel;
			unsigned short xpos;
			unsigned short ypos;

			V[0xF] = 0;

			// TODO test pong
			for (int yline = 0; yline < N; yline++) {
				currentPixel = memory[I + yline];
				ypos = (Y + yline) % SCREEN_HEIGHT;
				for (int xline = 0; xline < 8; xline++) {
					xpos = (X + xline) % SCREEN_WIDTH;

					if (gfx[xpos + (ypos * SCREEN_WIDTH)] == 1) {
						V[0xF] = 1;
					}
					gfx[(xpos + (ypos * SCREEN_WIDTH))] ^= 1;
				}
			}
			
			drawFlag = true;
			pc += 2;
			break;

		default:
			std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
			break;
	}
}
