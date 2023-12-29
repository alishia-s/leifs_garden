/*
 Assignment 9 - Educational App - Leif's Garden

 Team: Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac
 Date:      11/29/2023
 Course:    CS 3505, University of Utah, School of Computing
 Copyright: CS 3505 and Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac - This work may not be copied for use in Academic Coursework.

 We, Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac, certify that we wrote this code from scratch and
 did not copy it in part or whole from another source. All references used in the completion of the assignments are cited in my README file.

 File Contents

    This is the model for Leif's Garden Educational Game of the MVC.
*/
#ifndef MODEL_H
#define MODEL_H

#include "plant.h"
#include "consts.h"

#include <QObject>
#include <QQueue>
#include <QSet>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>

/// \brief The Model class - contains game logic
class Model : public QObject
{
    Q_OBJECT

private:
    /// \brief currentDay - The current day.
    int currentDay;

    /// \brief leifDialog - Contains dialog to display
    QQueue<QString>* leifDialogue;

    /// \brief loadDialogueLineFromFile - Loads a line of dialogue from a text file.
    /// \param filePath - The path of the file to laod from.
    /// \param line - The line to read from the file.
    /// \return The line of text from the specified file.
    QQueue<QString>* getDialogueFromFile(QString filePath);

    /// \brief getFunFactsFromFile - Loads a line of dialogue from a text file.
    /// \param filePath - The path of the file to laod from.
    /// \return Vector of strings
    QVector<QString>* getFunFactsFromFile(QString filepath);

    /// \brief funFactsDialogue - Contains dialogue to be added to leifDialogue.
    QVector<QString>* funFactsDialogue;

    /// \brief usedFunFacts - The associated used facts from unFactsDialogue.
    QSet<int>* usedFunFacts;

    /// \brief notebookPages - Array holding all the notebook pages for a plant
    QVector<QString>* notebookPages;

    /// \brief currentPage - File path to the current page of the notebook
    QString currentPage;

    /// \brief currentPageIndex - Index of the current page to accessed from notebookPages.
    int currentPageIndex = 0;

    /// \brief currentPlant - The plant selected by the user.
    Plant* currentPlant;

    /// \brief plantTemperature - the environment's temperature
    int plantTemperature;

    /// \brief plantPHValue - the plant soil's ph value
    float plantPHValue;

public:
    /// \brief Constructor
    explicit Model(QObject *parent = nullptr);

    /// \brief totalGrowingStages - a constant that keeps track of how many growth stages the plant has
    const int totalGrowingStages = 7;

    /// \brief task - enum of type of tasks
    enum task {wateringCan, fertilizer, thermostat, sulfur, pesticide, limestone, phMeter};

    /// \brief event - enum of types of events that could affect plant growth/care
    enum event {bugs, phChanged, temperatureChanged};

    /// \brief ~Model() - Destructor
    ~Model();

signals:

    /// \brief sendDisplayDialogue - Sends dialogue to the view to display.
    /// \param dialogue - The dialogue to display
    void sendDisplayDialogue(QString dialogue);

    /// \brief sendDisplayInstructions - Sends instructions to the notebook to display.
    /// \param instructions - The instructions to display
    void sendDisplayInstructions(QString instructions);

    /// \brief endDialogue - Tells the main window to end dialogue mode.
    void endDialogue();

    /// \brief showPlantSelection - Tells the main window to open or close plant selection.
    /// \param visible - True to open the selection, false to close it.
    void showPlantSelection(bool visible);

    /// \brief showNextDayButton - Tells MainWindow to make nextDayButton visible
    void showNextDayButton();

    /// \brief sendCurrentTemp - sends the current temperature of the environment
    /// \param temp - the current temperature
    void sendCurrentTemp(int temp);

    /// \brief sendCurrentTemp - sends the current temperature of the environment
    /// \param temp - the current temperature
    void sendCurrentPH(QString ph);

    /// \brief endGame - sent when the user finishes growing their plant
    void endGame();

    /// \brief plantDied - sent when the user kills their plant
    void plantDied();

    /// \brief randomEvent - decides a random event that could affect the plant's growth
    /// \param e - a random event fromm the enum event
    void randomEvent(event e);

    /// \brief startNextDay - starts the next day
    void startNextDay();

    /// \brief displayPlant - Tells mainWindow to display the plant.
    void displayPlant(QString plantImage);

    /// \brief enableAllButtons - enables all game buttons when the user selects a plant
    void enableAllButtons();

    /// \brief handleNotebookNextButton - Enable the next button depending on enable
    /// \param enable - boolean determing whether next button be enabled
    void handleNotebookNextButton(bool enable);

    /// \brief handleNotebookBackButton - Enable the back button depending on enable
    /// \param enable - boolean determing whether back button be enabled
    void handleNotebookBackButton(bool enable);

public slots:

    /// \brief sendDialogue - Sends a signal to display dialogue to the main winow.
    void sendDialogue();

    /// \brief sendNotebookInstructions - Sends a signal to display instructions to the main winow.
    void sendNotebookInstructions();

    /// \brief progressDialogue - Sends the next line of dialogue to the main window.
    void progressDialogue();

    /// \brief plantSelected - Run when a plant is selected in the plant selection screen.
    void plantSelected();

    /// \brief updateCurrentDay - Increments currentDay by one when advancing to the next day.
    void updateCurrentDay();

    /// \brief processTaskDone - takes the task that the user has done and applies the changes to the plant object
    /// \param taskPerformed - the type of task the user did
    void processTaskDone(task taskPerformed);

    /// \brief changeTemp - updates the environment's temperature value that the model and plant hold
    /// \param temp - the temperature the user set it as
    void changeTemp(int temp);

    /// \brief progressNextDay - checks if the user has complete the correct tasks to progress into the next day
    /// If they miss tasks, then the game ends
    /// If the plant is fully grown, the game ends
    /// Else, the game continues
    void progressNextDay();

    /// \brief addRandomEvents - creates a random event that will affect the growth/care of the plant
    void addRandomEvents();

    /// \brief notebookNextPage - sets the current page of the Notebook to be the next page
    void notebookNextPage();

    /// \brief notebookPreviousPage - sets the current page of the Notebook to be the previous page
    void notebookPreviousPage();
};
#endif // MODEL_H
