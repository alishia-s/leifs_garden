#include "plant.h"

void Plant::updateStage()
{
    currentStage++;
}

bool Plant::checkTemp()
{
    // Check if the temperature is outside of range
    if (currentTemp > preferredTempHigh || currentTemp < preferredTempLow)
    {
        return false;
    }

    return true;
}

bool Plant::checkPH()
{
    // Checks if current ph levels are within range
    if (currentPH > preferredPHHigh || currentPH < preferredPHLow)
    {
        return false;
    }

    return true;
}

void Plant::setHasBugs()
{
    hasBugs = true;
}

void Plant::setPHLevel(float PH)
{
    currentPH = PH;
}

void Plant::addWater()
{
    waterAmount++;
}

void Plant::fertilize()
{
    fertilized = true;
}

void Plant::changeTemp(int newTemp)
{
    currentTemp = newTemp;
}

void Plant::addPestilient()
{
    hasBugs = false;
}

int Plant::getTemp()
{
    return currentTemp;
}

int Plant::getPlantStage()
{
    return currentStage;
}

float Plant::getPH()
{
    return currentPH;
}

float Plant::getPreferredLowPh()
{
    return preferredPHLow;
}

float Plant::getPreferredHighPh()
{
    return preferredPHHigh;
}

QString Plant::getInstructionsPath()
{
    return instructionFilePath;
}

int Plant::getPreferredHighTemp()
{
    return preferredTempHigh;
}

int Plant::getPreferredLowTemp()
{
    return preferredTempLow;
}
