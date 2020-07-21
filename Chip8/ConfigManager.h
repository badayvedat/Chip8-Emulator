#pragma once
#include <string>

class ConfigManager {
public:
	// Returns false if config.ini file not found.
	bool loadConfigFile();

	int getCycleRefreshRate();

	std::string getRomName();

	int getRed();
	int getGreen();
	int getBlue();

	int getScale();

private:
	int scale;

	// Cycle refresh rate is in Hertz. 
	int CycleRefreshRate;

	std::string romName;

	// RGB values;
	int rgbRed;
	int rgbGreen;
	int rgbBlue;

	// Scale value which will be used to Multiply Height and Width.
	int scale;
};

