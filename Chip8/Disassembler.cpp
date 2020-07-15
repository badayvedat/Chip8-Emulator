#include <iostream>
#include <string>
#include <fstream>

int main() {
	std::ifstream is("test_opcode.ch8", std::ios::binary);
	if (!is.is_open()) {
		std::cout << "Failed to open file!" << '\n';
	}
	else {
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);
		char* buffer = new char[length];
		std::cout << "Reading " << length << " characters\n";
		is.read(buffer, length);
		if (is) {
			std::cout << "All characters read successfully." << '\n';
			for (int i = 0; i < length; i++) {
				std::cout << buffer[i];
			}
		}
		else {
			std::cout << " error: only " << is.gcount() << " could be read";
		}
		is.close();
		delete[] buffer;
	}
	return 0;
}