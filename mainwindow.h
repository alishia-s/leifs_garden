/*
 Assignment 9 - Educational App - Leif's Garden

 Team: Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac
 Date:      11/29/2023
 Course:    CS 3505, University of Utah, School of Computing
 Copyright: CS 3505 and Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac - This work may not be copied for use in Academic Coursework.

 We, Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac, certify that we wrote this code from scratch and
 did not copy it in part or whole from another source. All references used in the completion of the assignments are cited in my README file.

 File Contents

 This file contains the class defintion of the MainWindow class, representing the view of the Garden Game using MVC framework. This class contains methods to be interfaced with by the model,
 including changing Leif's face, playing sounds, and loading dialogue from a file.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "model.h"
#include "helpmenu.h"
#include "consts.h"

#include <QMainWindow>
#include <QResizeEvent>
#include <QtMultimedia>
#include <QScreen>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QMouseEvent>
#include <QString>
#include <QGraphicsScene>
#include <Box2D/Box2D.h>
#include <QApplication>
#include <QProcess>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/// \brief The MainWindow class - represents the view of the Garden Game in MVC framework.
class MainWindow : public QMainWindow
{
    Q_OBJECT

    /// \brief wateringCan - const string used to represent watering can tool
    const QString wateringCan = "wateringCan";

    /// \brief fertilizer - const string used to represent fertilizer tool
    const QString fertilizer = "fertilizer";

    /// \brief plantName - const string used to represent the plant
    const QString plantName = "plant";

    /// \brief pesticide - const string used to represent the pesticide
    const QString pesticide = "pesticide";

    /// \brief phMeter - const string used to represent the sulfure
    const QString sulfur = "sulfur";

    /// \brief limestone - const string used to represent the limestone
    const QString limestone = "limestone";

    /// \brief tap - const string used to represent the tap
    const QString tap = "tap";

    /// \brief thermostat - const string used to represent the thermostat
    const QString thermostat = "thermostat";

    /// \brief currentTool - represents the currentTool selected
    QString currentTool = "";

    /// \brief model - pointer to model
    Model * model;

    /// \brief helpMenu - creates a new window for help menu
    HelpMenu helpMenu;
    
    /// \brief toolDirectory - holds string to tools images directory in resources
    const QString toolsDirectory = ":/images/Images/Tools/";

public:

    /// \brief MainWindow - Contructs a main window object.
    /// \param parent - null, unused
    MainWindow(Model& model, QWidget *parent = nullptr);

    /// \brief mousePressEvent - The method ran when then mouse is clicked.
    /// \param clicked - The mouse clicked event containing information about the mouse click.
    void mousePressEvent(QMouseEvent* clicked) override;

    /// \brief showEvent - The method ran when the program is loaded.
    /// \param event - unused
    void showEvent(QShowEvent* event) override;

    /// \brief playMusic - Plays sound on the music channel.
    /// \param soundPath - The file path of the sound to play.
    void playMusic(QString soundPath);

    /// \brief playAmbience - Plays sound on the ambience channel.
    /// \param soundPath - The file path of the sound to play.
    void playAmbience(QString soundPath);

    /// \brief ~MainWindow() - Destructs the MainWindow object.
    ~MainWindow();

private slots:

    /// \brief setUpUI - Sets up the Garden Game UI once the start button is clicked.
    void setUpUI();

    /// \brief displayLeif - Animates Leif onto the screen.
    void displayLeif();

    /// \brief hideLeif - Animates Leif off of the screen.
    void hideLeif();

    /// \brief displayTools - Animates the tools view onto the screen.
    void displayTools();

    /// \brief displayPlantSelection - Displays or hides the plant selection view.
    /// \param visible - Displays if true, hides if false.
    void displayPlantSelection(bool visible);

    /// \brief playSoundEffect - Plays a sound effect.
    /// \param soundPath - The path of the sound file you would like to play.
    /// \param infinite - True if you want the sound to loop continuously.
    void playSoundEffect(QString soundPath, bool infinite);

    /// \brief changeFace - Changes the face of Leif
    /// \param imagePath - The path of the image file to display
    void changeFace(QString imagePath);

    /// \brief displayDialogue - Reads dialogue from a text file and displays it in a speech bubble.
    /// \param filePath - The path of the text file to read text from.
    void displayDialogue(QString filePath);

    /// \brief displayNotebook - Reads instructions from a text file and displays it in the notebook.
    /// \param instructions - The instructions which will be displayed on notebook.
    void displayNotebook(QString instructions);

    /// \brief toolButtonClicked - Processes when one of the tools have been clicked and updates the cursor icon.
    void toolButtonClicked();

    /// \brief plantClicked - Processes whether a plant's click was in relevant context (if tool had been clicked).
    void plantClicked();
    
    /// \brief endDialogue - Hides all dialogue.
    void endDialogue();

    /// \brief showStartButton - Animates the start button in the intro sequence
    void showStartButton();

    /// \brief showNextDayButton - Sets nextDayButton's visibility to true
    void showNextDayButton();

    /// \brief advanceDay - Transitions from the playable screen to the "nighttime" screen, and emits incrementDayCount().
    void advanceDay();

    /// \brief displayPlant - displays the chosen plant
    /// \param plantImage - the file name of the plant image
    void displayPlant(QString plantImage);

    /// \brief returnWindow - Brings the playable screen back after showing the night screen.
    void returnWindow();

    /// \brief plantDeath - changes the UI to reflect the dead plant
    void plantDeath();

    /// \brief endGame - changes the UI to reflect the end of the game
    void endGame();

    /// \brief newEvent - changes the UI to reflect an event that could affect plant growth/care
    /// \param randomEvent - a random event from Model's enum event
    void newEvent(Model::event randomEvent);

    /// \brief enablingButtons - enables all the buttons
    void enablingButtons();

    /// \brief upateNotebookNextButton - Enables the next page button on the notebook.
    void upateNotebookNextButton(bool enable);

    /// \brief upateNotebookBackButton - Enables the previous page button on the notebook.
    void upateNotebookBackButton(bool enable);

signals:

    /// \brief playSound - Sends a request to play a sound effect. Should usually be called from a single shot.
    /// \param soundPath - The path of the sound file to play.
    void playSound(QString soundPath, bool infinte);

    /// \brief changeLeifFace - Sends a request to change Leif's face.
    /// \param imagePath - The path of the image file to display.
    void changeLeifFace(QString imagePath);

    /// \brief getDialogue - Sends a dialogue request to the model.
    void getDialogue();

    /// \brief requestLeif - Sends a request for Leif to be displayed.
    void requestLeif();

    /// \brief dismissLeif - Sends a request for Leif to leave the screen.
    void dismissLeif();

    /// \brief requestTools - Sends a request for the Tools View to be displayed.
    void requestTools();

    /// \brief requestShowStartButton - Send a request to show the startButtonAnimation
    void requestShowStartButton();

    /// \brief progressDialogue - Sends a progress dialogue request to the model.
    void progressDialogue();

    /// \brief incrementDay - Requests that the model increments the variable representing the days that have passed.
    void incrementDay();

    /// \brief sendTask - Sends a signal to Model with information on what type of tool the user is using on the plant.
    void sendTask(Model::task);

    /// \brief bringBackWindow - Requests the playable screen after advancing to the next day.
    void bringBackWindow();

private:

    /// \brief effect - The sound effect to be played
    QSoundEffect effect;

    /// \brief music - The music channel.
    QSoundEffect music;

    /// \brief ambience - The ambient sound channel.
    QSoundEffect ambience;

    /// \brief notebookAnime - Animation carried out when notebook is to be opened or closed.
    QPropertyAnimation* notebookAnime;

    /// \brief isNotebookClosing - Keeps track whether the user requested that the notebook be closed
    bool isNotebookClosing = false;

    bool isNotebookOpen = false;

    /// \brief textDisplayed - True if text is currently being displayed.
    bool dialogueDisplayed;

    /// \brief hasBugs - used to display bugs if there are any
    bool hasBugs;

    /// \brief balloonImages - collection of all possible images in the ballon
    QVector<QPixmap *> balloonImages;

    /// \brief ui - The main window ui.
    Ui::MainWindow *ui;

    /// \brief scene - the scene for water droplet animation
    QGraphicsScene* scene;

    /// \brief timer
    QTimer timer;
    
    /// \brief plantDeathScreen - animates the plant death screen
    void animatePlantDeathScreen();

    /// \brief celebrationScreen - animates the win screen
    void celebrationScreen();

    /// \brief animateSkull - animates the death skull representing a dead plant
    void animateSkull();

    /// \brief displayRestartButton - displays the reset button
    void displayRestartButton();

    /// \brief restartWorld - restarts the game
    void restartWorld();

    /// \brief showTitleCard - Shows the title card sequence.
    void showTitleCard();

    /// \brief notebookAnimation - Carries out opening and closing notebook.
    /// \param openNotebook - whether the notebook needs to be opened(true) or closed(false)
    /// \param instructions - text to be displayed onto notebook
    void notebookAnimation(bool openNotebook, QString instructions);

    /// \brief setNotebookText - Sets the notebook text
    /// \param text - text to be displayed onto notebook
    void setNotebookText(QString text);

    /// \brief resetButtonVisibility - resets the tools buttons to reappear
    void resetButtonVisibility();

    /// \brief createToolMenu - creates and displays the tool menu
    void createToolMenu();

    /// \brief createPlantButton - creates and displays the plant
    void createPlantButton();

    /// \brief createNotebook - creates the notebook that holds plant details
    void createNotebook();

    /// \brief createHelpMenuButton - creates the UI help menu button
    void createHelpMenuButton();

    /// \brief playWaterAnimation - starts the water droplet animation when watering a plant
    void playWaterAnimation();

    /// \brief playWaterDropletAnimation - creates animation for water droplets
    void playWaterDropletAnimation();

    /// \brief drawBalloons- Uses box2D to draw balloons floating up
    void drawBalloons();

    /// \brief populateBalloonWorld - populates the world with consts.totalBalloons worth of bodies
    /// and assigns them an index to the balloonImages vector
    QVector<int> populateBalloonWorld(b2World* world, b2Vec2 gravity);

    /// \briefs Applies an upward force to everything in world and draws them as balloons.
    void balloonBox2DWorldStep(b2World* world, QVector<int> balloonIndexes);

    /// \brief populateBalloons - populates the balloonImages vector with QPixmaps of the balloon images we have.
    void populateBalloons();

    /// \brief disableNotebookButtons - Disables notebook buttons upon its closing.
    void disableNotebookButtons();

    /// \brief setUpAnimation - Helper method to setup a geometry animation.
    /// \param animator - pointer to QPropertyAnimation object
    /// \param animeDuration - how long animation should last
    /// \param startPoints - the start position of the animator
    /// \param startSize - the intial size of the animator
    /// \param endPoints - the end position of the animator
    /// \param endSize - the final size of the animator
    void setUpAnimation(QPropertyAnimation* animator, int animeDuration, QPoint startPoints, QSize startSize,  QPoint endPoints, QSize endSize);

    /// \brief createToolButton - Helper method to create tool buttons for tool menu.
    /// \param button - pointer to a button object
    /// \param toolName - name of associated tool for button
    /// \param buttonSize - desired size of the button
    void createToolButton(QPushButton* button, QString toolName, QSize buttonSize);

};
#endif // MAINWINDOW_H
