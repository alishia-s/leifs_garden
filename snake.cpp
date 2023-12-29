#include "snake.h"

SnakePlant::SnakePlant(QObject *parent):
    QObject(parent)
{
    plantName = "Snake";
    instructionFilePath = ":/images/Images/Instructions/Snake/snakeInstructions";
    currentStage = 1;
    waterAmount = 0.0;
    preferredTempLow = 65;
    preferredTempHigh = 80;
    preferredPHHigh = 8.5;
    preferredPHLow = 4.5;
    hasBugs = false;
    fertilized = false;
}

void SnakePlant::endOfDaySuccess()
{
    if (fertilized)
    {
        // Resets fertilized task
        fertilized = false;
    }

    // If successful, decrease water by 1/7 of a day (only watered once)
    waterAmount -= (float) 1/7;

    if (waterAmount < 0)
    {
        waterAmount = 0;
    }
}

void SnakePlant::checkFirstDayTasks()
{
    // Check if environment temp is within range
    if (!checkTemp())
    {
        missedTasks.append("wrongTemperature");
    }

    // Check ph level of soil
    if (!checkPH())
    {
        missedTasks.append("wrongPH");
    }

    // Check that it has no bugs
    if (hasBugs)
    {
        missedTasks.append("hasBugs");
    }

    // Check if it's been watered. Should only be watered when it's first planted
    if (waterAmount == 0)
    {
        missedTasks.append("notWatered");
    }

    // Check if it's been watered. Should only be watered when it's first planted
    if (waterAmount > 1)
    {
        missedTasks.append("overwatered");
    }

    // Check if it's been fertilized. Should only be fertilized when it's first planted
    if (!fertilized)
    {
        missedTasks.append("notFertilized");
    }
}

void SnakePlant::checkBasicTasks()
{
    //check if environment temp is within range
    if (!checkTemp())
    {
        missedTasks.append("wrongTemperature");
    }

    //check ph level of soil
    if (!checkPH())
    {
        missedTasks.append("wrongPH");
    }

    //make sure it hasn't been overwatered
    if (waterAmount > 1)
    {
        missedTasks.append("overwatered");
    }

    //make sure it has no bugs
    if (hasBugs)
    {
        missedTasks.append("hasBugs");
    }

    //make sure it hasn't been overfertilized
    if (fertilized)
    {
        missedTasks.append("overfertilized");
    }
}

QVector<QString> SnakePlant::checkStage(int stage)
{
    switch (stage)
    {
        case 1:
            checkFirstDayTasks();
            break;
        case 2 ... 7:
            checkBasicTasks();
            break;
    };

    if (missedTasks.size() == 0)
    {
        endOfDaySuccess();
    }

    return missedTasks;
}

QString SnakePlant::getPlantImageName()
{
    return plantName + "/" + plantName + QString::number(currentStage);
}

SnakePlant::~SnakePlant(){}
