/*
 Assignment 9 - Educational App - Leif's Garden

 Team: Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac
 Date:      11/29/2023
 Course:    CS 3505, University of Utah, School of Computing
 Copyright: CS 3505 and Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac - This work may not be copied for use in Academic Coursework.

 We, Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac, certify that we wrote this code from scratch and
 did not copy it in part or whole from another source. All references used in the completion of the assignments are cited in my README file.

 File Contents

    This file sets the logic and attributes of a succulent plant object
*/
#ifndef SUCCULENT_H
#define SUCCULENT_H

#include "plant.h"

#include <QtPlugin>

/// \brief The succulent class - represents a succulent object and tracks the task and conditions need for the succulent to grow.
///
/// \note QObject needs to be first to avoid static meta object error
/// see: https://medium.com/@yakupcengiz/multiple-inheritance-requires-qobject-to-be-first-628faa7
class Succulent : public QObject, public Plant
{
    Q_OBJECT

    /// \brief checkFirstDayTasks: checks if the first day tasks are completed. Different from other days' tasks.
    void checkFirstDayTasks();

public:

    /// \brief Succulent - Constructs a Succulent Object
    /// \param parent - Unused
    explicit Succulent(QObject *parent = nullptr);

    /// \brief ~Succulent - Destructor
    ~Succulent();

    /// \brief checkStage - used to check if tasks are done and changes the succulent's current stage
    /// \param stage - the stage to check
    /// \return A vector of incomplete or missed tasks
    QVector<QString> checkStage (int stage) override;

    /// \brief getPlantImageName - gets the file name of the image that corresponds to the current stage of the plant
    /// \return A String filename
    QString getPlantImageName() override;

    /// \brief endOfDaySuccess - called if the user has successfully completed tasks needed to grow and care for the succulent.
    ///  Used to reset certain tasks so they can be completed again
    void endOfDaySuccess() override;

    /// \brief checkBasicTasks: checks if the basic required tasks are completed.
    void checkBasicTasks() override;
};
#endif // SUCCULENT_H
