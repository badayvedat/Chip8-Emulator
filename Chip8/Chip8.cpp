#include <iostream>
#include <fstream>
#include <iomanip>

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

	// Clear keys
	std::fill(key, key + 16, 0);

	// Load fontset
	for (int i = 0; i < 80; i++) {
		memory[i] = fontset[i];
	}

	// Reset timers
	delay_timer = 0;
	sound_timer = 0;

	// Set draw flag
	drawFlag = 0;
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
			memory[i + 0x200] = buffer[i];
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
	std::cout << std::setfill('0') << std::setw(4) << std::hex << opcode << ' ' << pc << ' ' << I << '\n';
	
	pc += 2;

	// Decode opcode
	//AND opcode with 0xF000 to get first 4 bits.
	switch (opcode & 0xF000) {
		case 0x0000: {
			// AND opcode with 0x0FF to get last 8 bits
			switch (opcode & 0x0FF) {
				case 0x0000: {
					break;
				}

				// Clears the screen
				case 0x00E0: {
					std::fill(gfx, gfx + SCREEN_HEIGHT * SCREEN_WIDTH, 0);
					drawFlag = true;
					break;
				}

				// Returns from a subroutine
				case 0x00EE: {
					--sp;
					pc = stack[sp];
					break;
				}

				default: {
					std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
					break;
				}
			}
			break;
		}
		// Jumps to address NNN
		case 0x1000: {
			pc = (opcode & 0x0FFF);
			break;
		}

		// Calls subroutine at NNN
		case 0x2000: {
			stack[sp] = pc;
			sp++;
			pc = (opcode & 0x0FFF);
			break;
		}

		// Skips the next instruction if VX equals NN
		case 0x3000: {
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
				pc += 2;
			}
			break;
		}

		// Skips the next instruction if VX doesn't equal NN
		case 0x4000: {
			if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
				pc += 2;
			}
			break;
		}

		// Skips the next instruction if VX equals VY
		case 0x5000: {
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
				pc += 2;
			}
			break;
		}

		// Sets VX to NN
		case 0x6000: {
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
			break;
		}

		// Adds NN to VX (Carry flag is not changed)
		case 0x7000: {
			V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
			break;
		}

		case 0x8000: {
			switch (opcode & 0x000F) {
				// Sets VX to the value of VY
				case 0x0000: {
					unsigned char VY = V[(opcode & 0x00F0) >> 4];
					V[(opcode & 0x0F00) >> 8] = VY;
					break;
				}

				// Sets VX to VX or VY (Bitwise OR operation)
				case 0x0001: {
					unsigned char VX = V[(opcode & 0x0F00) >> 8];
					unsigned char VY = V[(opcode & 0x00F0) >> 4];
					V[(opcode & 0x0F00) >> 8] = (VX | VY);
					break;
				}

				// Sets VX to VX and VY (Bitwise AND operation)
				case 0x0002: {
					unsigned char VX = V[(opcode & 0x0F00) >> 8];
					unsigned char VY = V[(opcode & 0x00F0) >> 4];
					V[(opcode & 0x0F00) >> 8] = (VX & VY);
					break;
				}

				// Sets VX to VX xor VY
				case 0x0003: {
					unsigned char VX = V[(opcode & 0x0F00) >> 8];
					unsigned char VY = V[(opcode & 0x00F0) >> 4];
					V[(opcode & 0x0F00) >> 8] = (VX ^ VY);
					break;
				}

				// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
				case 0x0004: {
					unsigned char VX = V[(opcode & 0x0F00) >> 8];
					unsigned char VY = V[(opcode & 0x00F0) >> 4];
					if (VX > 0xFF - VY) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}
					V[(opcode & 0x0F00) >> 8] += VY;
					break;
				}

				// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				case 0x0005: {
					unsigned char VX = V[(opcode & 0x0F00) >> 8];
					unsigned char VY = V[(opcode & 0x00F0) >> 4];
					if (VX < VY) {
						V[0xF] = 0;
					} else {
						V[0xF] = 1;
					}
					V[(opcode & 0x0F00) >> 8] -= VY;
					break;
				}

				// Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
				case 0x0006: {
					unsigned char VX = V[(opcode & 0x0F00) >> 8];
					V[0xF] = (VX & 0x1);
					V[(opcode & 0x0F00) >> 8] >>= 1;
					break;
				}

				// Sets VX to VY minux VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				case 0x0007: {
					unsigned char VX = V[(opcode & 0x0F00) >> 8];
					unsigned char VY = V[(opcode & 0x00F0) >> 4];
					if (VX > VY) {
						V[0xF] = 0;
					} else {
						V[0xF] = 1;
					}
					V[(opcode & 0x0F00) >> 8] = VY - VX;
					break;
				}

				// Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
				case 0x000E: {
					unsigned char VX = V[(opcode & 0x0F00) >> 8];
					V[0xF] = (VX & 0x80) >> 7;
					V[(opcode & 0x0F00) >> 8] <<= 1;
					break;
				}

				default: {
					std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
					break;
				}
			}
			break;
		}

		// Skips the next instruction if VX doesn't equal VY.
		case 0x9000: {
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
				pc += 2;
			}
			break;
		}

		// Sets I to the address NNNN.
		case 0xA000: {
			I = (opcode & 0x0FFF);
			break;
		}

		// Jump to the address NNN plus V0.
		case 0xB000: {
			pc = ((opcode & 0x0FFF) + V[0x000]);
			break;
		}

		// Sets VX to the result of a bitwise and operation on a random number.
		case 0xC000: {
			V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
			break;
		}

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
		*/
		case 0xD000: {
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

					if (currentPixel) {
						if (gfx[xpos + (ypos * SCREEN_WIDTH)] == 1) {
							V[0xF] = 1;
						}
						gfx[(xpos + (ypos * SCREEN_WIDTH))] ^= 1;
					}
				}
			}

			drawFlag = true;
			break;
		}

		case 0xE000: {
			switch (opcode & 0x000F) {
				// Skips the next instruction if the key stored in VX is pressed.
				case 0x000E: {
					if (key[V[(opcode & 0x0F00) >> 8]] != 0) {
						pc += 2;
					}
					break;
				}

				// Skips the next instruction if the key stored in VX isn't pressed.
				case 0x0001: {
					if (key[V[(opcode & 0x0F00) >> 8]] == 0) {
						pc += 2;
					}
					break;
				}

				default: {
					std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
					break;
				}
			}
			break;
		}

		case 0xF000: {
			switch (opcode & 0x00FF) {
				// Sets VX to the value of the delay timer.
				case 0x0007: {
					V[(opcode & 0x0F00) >> 8] = delay_timer;
					break;
				}

				// A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event).
				case 0x000A: {
					bool key_pressed = false;
					for (int i = 0; i < 16; i++) {
						if (key[i] != 0) {
							key_pressed = true;
							V[(opcode & 0x0F00) >> 8] = i;
							break;
						}
					}

					if (!key_pressed) {
						pc -= 2;
					}

					break;
				}

				// Sets the delay timer to VX.
				case 0x0015: {
					delay_timer = V[(opcode & 0x0F00) >> 8];
					break;
				}

				// Sets the sound timer to VX.
				case 0x0018: {
					sound_timer = V[(opcode & 0x0F00) >> 8];
					break;
				}

				/*
				Adds VX to I. VF is not affected.
				Most CHIP-8 interpreters' FX1E instructions do not affect VF, with one exception:
				The CHIP-8 interpreter for the Commodore Amiga sets VF to 1 when there is a range overflow (I+VX>0xFFF),
				and to 0 when there isn't. The only known game that depends on this behavior is Spacefight 2091!
				While at least one game, Animal Race, depends on VF not being affected.
				*/
				case 0x001E: {
					I += V[(opcode & 0x0F00) >> 8];
					break;
				}

				// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
				case 0x0029: {
					unsigned short VX = V[(opcode & 0x0F00) >> 8];
					I = VX * 5;
					break;
				}

				/*
				Stores the binary-coded decimal representation of VX,
				with the most significant of three digits at the address in I,
				the middle digit at I plus 1, and the least significant digit at I plus 2.
				(In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I,
				the tens digit at location I+1, and the ones digit at location I+2.)
				*/
				case 0x0033: {
					unsigned char VX = V[(opcode & 0x0F00) >> 8];
					memory[I] = (VX / 100) % 10;
					memory[I + 1] = (VX / 10) % 10;
					memory[I + 2] = VX % 10;
					break;
				}

				// Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
				case 0x0055: {
					int x = (opcode & 0x0F00) >> 8;

					for (int i = 0; i <= x; i++) {
						memory[I + i] = V[i];
					}

					break;
				}

				// Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
				case 0x0065: {
					int x = (opcode & 0x0F00) >> 8;

					for (int i = 0; i <= x; i++) {
						V[i] = memory[I + i];
					}

					break;
				}

				default: {
					std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
					break;
				}
			}
			break;
		}

		default: {
			std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
			break;
		}
	}
}

bool Chip8::getDrawFlag() {
	return drawFlag;
}

void Chip8::setDrawFlag(bool flag) {
	drawFlag = flag;
}

unsigned char* Chip8::getGfx() {
	return gfx;
}
