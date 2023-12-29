/*
 Assignment 9 - Educational App - Leif's Garden

 Team: Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac
 Date:      11/29/2023
 Course:    CS 3505, University of Utah, School of Computing
 Copyright: CS 3505 and Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac - This work may not be copied for use in Academic Coursework.

 We, Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac, certify that we wrote this code from scratch and
 did not copy it in part or whole from another source. All references used in the completion of the assignments are cited in my README file.

 File Contents

    This abstract class is used to create the following plant classes for the game
    Succulent, Snake, Tomato.
*/
#ifndef PLANT_H
#define PLANT_H

#include <QVector>
#include <QString>
#include <QImage>
#include <QObject>

/// \brief The Plant class - contains logic of how the plant object will behave
class Plant
{
protected:

    /// \brief plantName - current plant name
    QString plantName;

    /// \brief currentStage - the current stage of the plant
    int currentStage;

    /// \brief waterAmount - The amount of water the plant has
    float waterAmount;

    /// \brief preferredTempLow - The lowest temperature the plant prefers
    int preferredTempLow;

    /// \brief preferredTempHigh - The highest temperature the plant prefers
    int preferredTempHigh;

    /// \brief currentTemp - The current temperature of the room the plant is in
    int currentTemp;
  
    /// \brief preferredPHLow - The lowest PH level the plant prefers
    float preferredPHLow;

    /// \brief preferredPHHigh - the highest PH level the plant prefers
    float preferredPHHigh;

    /// \brief currentPH - The current PH level of the plant
    float currentPH;

    /// \brief fertilized - Whether the plant has been fertalized or not
    bool fertilized;

    /// \brief hasBugs - Whether the plant has bugs or not
    bool hasBugs;

    /// \brief missedTasks - list of its missed tasks
    QVector<QString> missedTasks;

    /// \brief insturctionFilePath - file path to the plant insturctions
    QString instructionFilePath;

public:

    /// \brief ~Plant - virtual deconstructor
    virtual ~Plant() = default;

    /// \brief checkTemp - checks if the current temperature of the game is within the acceptable temp range
    /// \return True if current temperature is within range, false otherwise
    bool checkTemp();

    /// \brief checkPH - checks if the current PH level of the soil is within the acceptable ph range for the plant
    /// \return True if current PH levels are within range, false otherwise
    bool checkPH();

    /// \brief setHadBugs - sets true if there are bugs on the plant
    void setHasBugs();

    /// \brief setPHLevel - sets the current ph level of the soil
    void setPHLevel(float PH);

    /// \brief addWater - Adds water to the plants current water level
    void addWater();

    /// \brief fertilize - Fertilizes the plant
    void fertilize();

    /// \brief changeTemp - Changes the temperature of the room the plant is in.
    /// \param newTemp - The new temperature of the room.
    void changeTemp(int newTemp);

    /// \brief addedPestilient - sets hasBugs false
    void addPestilient();

    /// \brief updateStage - updates the plant's growth stage
    void updateStage();

    /// \brief getTemp - Gets the current temperature of the room
    /// \return The current temperature of the room
    int getTemp();
  
    /// \brief getPH - Gets the current PH of the plant
    /// \return the current PH of the plant
    float getPH();

    /// \brief getPreferredLowPh - Gets the preferred low PH of the plant
    /// \return the preferred low PH of the plant
    float getPreferredLowPh();

    /// \brief getPreferredHighPh - Gets the preferred high PH of the plant
    /// \return the preferred high PH of the plant
    float getPreferredHighPh();

    /// \brief getPlantStage - Gets the current stage of the plant. Used to check if the plant is growing or not
    /// \return the current stage of the plant
    int getPlantStage();

    /// \brief getInstructionsPath - Gets the file path of for plant instructions
    /// \return filePath of the associated plant insructions file
    QString getInstructionsPath();

    /// \brief getPreferredHighTemp - returns the highest temp preferred by the plant
    /// \return the highest temp preferred by the plant
    int getPreferredHighTemp();

    /// \brief getPreferredLowTemp - returns the lowest temp preferred by the plant
    /// \return the lowest temp preferred by the plant
    int getPreferredLowTemp();

    /// \brief checkStage - Returns the tasks that have not been completed for that stage
    /// \param stage - The stage of which the tasks are being checked
    virtual QVector<QString> checkStage(int stage) = 0;

    /// \brief endOfDaySuccess - called if the user has successfully completed tasks needed to grow and care for the snake plant
    ///        Used to reset certain tasks so they can be completed again
    virtual void endOfDaySuccess() = 0;

    /// \brief checkTasks - checks if the basic required tasks are completed
    virtual void checkBasicTasks() = 0;

    /// \brief getPlantImage - Gets the current stages plant image.
    /// \return the current stages plant image
    virtual QString getPlantImageName() = 0;
};
//force include snake plant child class header
//https://stackoverflow.com/questions/31665600/including-header-files-of-base-classes-and-sub-classes-derived-classes-correct
#include "snake.h"
#include "tomato.h"
#include "succulent.h"
#endif // PLANT_H
