#include "tomato.h"

Tomato::Tomato(QObject *parent) : QObject(parent)
{
    plantName = "Tomato";
    instructionFilePath = ":/images/Images/Instructions/Tomato/tomatoInstructions";
    currentStage = 1; 
    waterAmount = 0;
    preferredTempLow = 70;
    preferredTempHigh = 85;
    currentTemp = 0;
    preferredPHLow = 6;
    preferredPHHigh = 7;
    currentPH = 0;
    fertilized = false;
    hasBugs = false;
}

QVector<QString> Tomato::checkStage(int stage)
{
    switch (stage)
    {
        case 1:
            checkBasicTasks();
            break;
        case 2 ... 7:
            checkBasicTasks();
            break;
    }

    if (missedTasks.size() == 0)
    {
        endOfDaySuccess();
        return missedTasks;
    }

    QVector<QString> temp = missedTasks;
    missedTasks.clear();

    return temp;
}

void Tomato::endOfDaySuccess()
{
    waterAmount -= 1;

    if (waterAmount < 0)
    {
        waterAmount = 0;
    }
}

void Tomato::checkBasicTasks()
{
    if (currentTemp < preferredTempLow || currentTemp > preferredTempHigh)
    {
        missedTasks.append("wrongTemperature");
    }
    if (waterAmount < 1)
    {
        missedTasks.append("notWatered");
    }
    if (currentPH < preferredPHLow || currentPH > preferredTempHigh)
    {
        missedTasks.append("wrongPH");
    }
    if (!fertilized)
    {
        missedTasks.append("notFertilized");
    }
    if (hasBugs)
    {
        missedTasks.append("hasBugs");
    }
}

QString Tomato::getPlantImageName()
{
    return plantName + "/" + plantName + QString::number(currentStage);
}

Tomato::~Tomato(){}

