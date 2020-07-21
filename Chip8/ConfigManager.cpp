#include "ConfigManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

bool ConfigManager::loadConfigFile() {
    std::ifstream ifs{ "config.ini" };
    
    if (!ifs) {
        std::cerr << "Config file not found!" << '\n';
        return false;
    }

    // If all of them found in config file return true;
    bool isRom = false;
    bool isCycleRate = false;
    bool isRed = false;
    bool isGreen = false;
    bool isBlue = false;
    bool isScale = false;

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
                isRom = true;
            }
            else if (tempString == "REFRESHRATE") {
                lineStream >> CycleRefreshRate;
                isCycleRate = true;
            }
            else if (tempString == "RED") {
                 lineStream >> rgbRed;
                 isRed = true;
            }
            else if (tempString == "GREEN") {
                    lineStream >> rgbGreen;
                    isGreen = true;
            }
            else if (tempString == "BLUE") {
                    lineStream >> rgbBlue;
                    isBlue = true;
            }
            else if (tempString == "SCALE") {
                lineStream >> scale;
                isScale = true;
            }
        }
    }

    if (isRom && isCycleRate && isRed && isGreen && isRed && isScale) {
        return true;
    }

    return false;
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

int ConfigManager::getScale() {
    return scale;
}
