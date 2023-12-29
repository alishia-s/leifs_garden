#include "succulent.h"

Succulent::Succulent(QObject *parent)
    : QObject{parent}
{
    plantName = "Succulent";
    instructionFilePath = ":/images/Images/Instructions/Succulent/succulentInstructions";
    currentStage = 1;
    waterAmount = 0.0;
    preferredTempLow = 40;
    preferredTempHigh = 80;
    preferredPHHigh = 6.5;
    preferredPHLow = 4;
    hasBugs = false;
    fertilized = false;
}

void Succulent::endOfDaySuccess()
{
    if (fertilized)
    {
        fertilized = false;
    }

    waterAmount -= 1.0/7.0;

    if (waterAmount < 0)
    {
        waterAmount = 0;
    }
}

void Succulent::checkFirstDayTasks()
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

    // Check if it's been overwatered.
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

void Succulent::checkBasicTasks()
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

    // Make sure it hasn't been overwatered
    if (waterAmount > 1)
    {
        missedTasks.append("overwatered");
    }

    // Make sure it has no bugs
    if (hasBugs)
    {
        missedTasks.append("hasBugs");
    }

    // Make sure it hasn't been overfertilized
    if (fertilized)
    {
        missedTasks.append("overfertilized");
    }
}

QVector<QString> Succulent::checkStage(int stage)
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

        return missedTasks;
    }

    QVector<QString> temp = missedTasks;
    missedTasks.clear();

    return temp;
}

QString Succulent::getPlantImageName()
{
    return plantName + "/" + plantName + QString::number(currentStage);
}

Succulent::~Succulent(){}
