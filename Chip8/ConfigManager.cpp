#include "ConfigManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

bool ConfigManager::loadConfigFile() {
    std::ifstream ifs{ "config.ini" };
    
    if (!ifs) {
        std::cerr << "Config file not found!" << '\n';
        return -1;
    }

    // If all of them found in config file return true;
    bool rom = false;
    bool cycleRate = false;
    bool red = false;
    bool green = false;
    bool blue = false;

    std::string line;

    /* 
    Dummy strings initialized.
    tempString will be used to check identifier.
    tempOperator will be used to check if equality("=") operator comes after the identifier.
    */
    std::string tempString, tempOperator;

    while (ifs) {
        std::getline(ifs, line);
        std::stringstream lineStream{ line };
        
        while (lineStream) {
            lineStream >> tempString >> tempOperator;
            
            if (tempString == "#") break;
            if (tempOperator != "=") break;

            else if (tempString == "ROM") {
                lineStream >> romName;
                rom = true;
            }
            else if (tempString == "REFRESHRATE") {
                lineStream >> CycleRefreshRate;
                cycleRate = true;
            }
            else if (tempString == "RED") {
                 lineStream >> rgbRed;
                 red = true;
            }
            else if (tempString == "GREEN") {
                    lineStream >> rgbGreen;
                    green = true;
            }
            else if (tempString == "BLUE") {
                    lineStream >> rgbBlue;
                    blue = true;
            }
        }
    }

    if (rom && cycleRate && rgbRed && rgbGreen && rgbRed) {
        return 1;
    }

    return -1;
}

int ConfigManager::getCycleRefreshRate() {
    return CycleRefreshRate;
}

std::string ConfigManager::getRomName() {
    return romName;
}

int ConfigManager::getRed() {
    return rgbRed;
}

int ConfigManager::getGreen() {
    return rgbGreen;
}

int ConfigManager::getBlue() {
    return rgbBlue;
}
