/*
 Assignment 9 - Educational App - Leif's Garden

 Team: Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac
 Date:      11/29/2023
 Course:    CS 3505, University of Utah, School of Computing
 Copyright: CS 3505 and Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac - This work may not be copied for use in Academic Coursework.

 We, Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac, certify that we wrote this code from scratch and
 did not copy it in part or whole from another source. All references used in the completion of the assignments are cited in my README file.

 File Contents

    This header inherits from both QObject and Plant abstract class to create a Snake Plant object that will be used when
    the user decides to plant a snake plant.

    It defines the virtual methods from the Plant class and has other methods to help the virtual methods.
*/

#ifndef SNAKE_H
#define SNAKE_H

#include "plant.h"

#include <QtPlugin>

/// \brief The SnakePlant class class represents a SnakePlant object and tracks the tasks and conditions needed for the snake plant to grow
///
/// \note QObject needs to be first to avoid static meta object error
/// see: https://medium.com/@yakupcengiz/multiple-inheritance-requires-qobject-to-be-first-628faa719140
class SnakePlant : public QObject, public Plant
{
    Q_OBJECT

    /// \brief checkFirstDayTasks - checks if the first day tasks are completed. Different from other days' tasks
    void checkFirstDayTasks();

public:

    /// \brief SnakePlant - constructor for snake plant object
    /// \param parent - QObject parent. Null if not specified
    explicit SnakePlant(QObject *parent = nullptr);

    ///\brief ~SnakePlant() - Deconstructor
    ~SnakePlant();

    /// \brief checkStage - Used to check if tasks are done and changes the snake plant's current stage
    /// \return A vector of incomplete or missed tasks
    QVector<QString> checkStage(int stage) override;

    /// \brief getPlantImageName - gets the file name of the image that corresponds to the current stage of the plant
    /// \return A string filename
    QString getPlantImageName() override;

    /// \brief endOfDaySuccess - called if the user has successfully completed tasks needed to grow and care for the snake plant
    ///        Used to reset certain tasks so they can be completed again
    void endOfDaySuccess() override;

    /// \brief checkTasks - checks if the basic required tasks are completed
    void checkBasicTasks() override;
};
#endif // SNAKE_H
