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

    while (std::getline(ifs, line)) {
        std::stringstream lineStream{ line };

        if (lineStream >> tempString >> tempOperator) {
            
            // # character means it's comment line.
            if (tempString == "#") {
                continue;
            }
            if (tempOperator != "=") {
                continue;
            }

            else if (tempString == "ROM") {
                std::string token;
                std::getline(lineStream, token);

                auto startIndex = token.find('"');

                // Rom name must start with " character.
                if (startIndex == std::string::npos) {
                    continue;
                }

                auto lastIndex = token.find('"', startIndex + 1);

                // There must be another " characther which identifies end of the rom's name.
                if (lastIndex == std::string::npos) {
                    continue;
                }

                // There should not be another " character in line.
                if (token.find('"', lastIndex + 1) != std::string::npos) {
                    continue;
                }

                romName = token.substr(startIndex + 1, lastIndex - 2);
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
