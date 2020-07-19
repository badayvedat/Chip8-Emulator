#pragma once
#include <string>

#define TOTAL_MEMORY 4096
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

//TODO Instruction class 

class Chip8
{
public:
	Chip8();
	~Chip8();

	bool loadGame(std::string fileName);
	void emulateCycle();

	bool getDrawFlag();
	void setDrawFlag(bool flag);

	unsigned char* getGfx();
	unsigned char* getKeys();

private:
	unsigned char memory[TOTAL_MEMORY];
	unsigned char gfx[SCREEN_HEIGHT * SCREEN_WIDTH];

	unsigned short opcode;
	unsigned char V[16];
	unsigned short I;
	unsigned short pc;

	bool drawFlag;

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[16];
	unsigned short sp;

	unsigned char key[16];
	const unsigned char fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	
};


