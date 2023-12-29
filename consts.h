/*
 Assignment 9 - Educational App - Leif's Garden

 Team: Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac
 Date:      11/29/2023
 Course:    CS 3505, University of Utah, School of Computing
 Copyright: CS 3505 and Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac - This work may not be copied for use in Academic Coursework.

 We, Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac, certify that we wrote this code from scratch and
 did not copy it in part or whole from another source. All references used in the completion of the assignments are cited in my README file.

 File Contents

    This file contains any constants that are used in the project. Used for
    easy access.
*/

#ifndef CONSTS_H
#define CONSTS_H

#include <QString>
#include <QSize>
#include <QRect>
#include <QPoint>

/* -------Screen Information ------ */
/// \brief The width of the screen
const int screenWidth = 800;
/// \brief The height of the screen
const int screenHeight =  500;
/// \brief Size of the screen
const QSize backgroundSize(800, 500);
/// \brief Lowest Y on screen
const int bottomY = screenHeight;

/* -------Plant Information ------ */
/// \brief Location of the plant offscreen to the right
const QPoint plantRightOffscreenPosition(1120,265);
/// \brief Location of the plant offscreen to the left
const QPoint plantLeftOffscreenPosition(-480,265);
/// \brief Location of the plant onscreen
const QPoint plantOnscreenPosition(320,265);
/// \brief Size of the plant
const QSize plantSize(170,170);

/* -------Box2D Intervals ------ */
/// \brief Interval of QTimer for world = 10
const int timerIntervalMS = 10;

/* -------End Game Information ------*/
/// \brief Animation duration for end game animations
const int endGameAnimationduration = 700;
/// \brief Location of the background offscreen to the left
const QPoint backgroundLeftOffscreenPosition(-800,0);
/// \brief Location of the background
const QPoint backgroundOnscreenPosition(0,0);

/* -------Death Screen Information------*/
/// \brief Animation duration for death animations
const int deathAnimationDuration = 800;
/// \brief Location of when plant falls during death screen
const QPoint plantFallingToDeathStartPosition(320,0);

/* -------Advance Day Information------*/
/// \brief Animation duration for advanceing to next day animations
const int advanceDayAnimationDuration = 700;
/// \brief X coordinate of the offscreen position for advance day (Right)
const int offscreenAdvanceDayXPositon =  850;
/// \brief Location of the background offscreen to the right
const QPoint backgroundRightOffscreenPosition(800,0);

/* -------Return Screen Information------*/
/// \brief X coordinate of the offscreen position for return screen (Left)
const int offscreenReturnWindowXPositon =  -750;

/* -------Notebook ------*/
/// \brief Animation duration for notebook animations
const int notebookAnimationduration = 1000;
/// \brief Location of the notebook offscreen to the right
const QPoint notebookOffsceenPosition(1000, 0);
/// \brief Location of the notebook oncsreen
const QPoint notebookOnsceenPosition(340, -45);
/// \brief Size of the notebook button
const QSize notebookButtonSize(60,60);
/// \brief Size of the open notebook
const QSize notebookSize(500, 600);
/// \brief Size of the closed notebook
const QSize notebookClosedSize(0, 0);
/// \brief Location of the notebook text box
const QPoint notebookTextBoxPosition(490, 35);
/// \brief Size of the notebook text box
const QSize notebookTextBoxSize(240, 440);
/// \brief The notebookTextBox dimensions
const QRect notebookTextBox(notebookTextBoxPosition, notebookTextBoxSize);

/* -------Tool Bar Menu Buttons ------*/
/// \brief X coordinate for tools
const int toolPostionX = 57;
const int offscreenToolXPositon =  -250;
/// \brief Size of the default tool button
const QSize toolButtonSize(50, 50);
/// \brief Size of the watering can tool button
const QSize wateringCanSize(60,60);
/// \brief Size of the sulfur tool button
const QSize sulfurSize(60,60);
/// \brief Size of the phMeter display
const QSize phMeterLabelSize(20,15);
/// \brief Y coordinate of the first tool in tool menu
const int firstToolPositonY = 45;
/// \brief Y spacing between tools
const int spacingBetweenToolsY = 57;
/// \brief Animation duration for incoming tool animations
const int toolMenuAnimationduration = 600;

/* -------Tool Names ------*/
/// \brief toolDirectory: holds string  tools images directory in resources
const QString toolsDirectory = ":/images/Images/Tools/";
/// \brief wateringCan const string used to represent watering can tool
const QString wateringCan = "wateringCan";
/// \brief fertilizer const string used to represent fertilizer tool
const QString fertilizer = "fertilizer";
/// \brief plantName const string used to represent the plant object
const QString plantName = "plant";
/// \brief pesticide const string used to represent the pesticide tool
const QString pesticide = "pesticide";
/// \brief sulfur const string used to represent the sulfur tool
const QString sulfur = "sulfur";
/// \brief limestone const string used to represent the limestone tool
const QString limestone = "limestone";
/// \brief phMeter const string used to represent the phMeter tool
const QString phMeter = "phMeter";
/// \brief tap const string used to represent the tap tool
const QString tap = "tap";
/// \brief thermostat const string used to represent the thermostat
const QString thermostat = "thermostat";

/* -------Balloons ------ */
/// \brief Force to exert on balloon going up
const int balloonSpeedUp = 8000;
/// \brief Width of BalloonImages
const int balloonWidth = 60;
/// \brief Path to first balloon image
const QString balloonImagePath1 = ":/images/Images/Balloons/balloonImage1.png";
/// \brief Path to second balloon image
const QString balloonImagePath2 = ":/images/Images/Balloons/balloonImage2.png";
/// \brief Path to third balloon image
const QString balloonImagePath3 = ":/images/Images/Balloons/balloonImage3.png";
/// \brief Path to fourth balloon image
const QString balloonImagePath4 = ":/images/Images/Balloons/balloonImage4.png";
/// \brief Total Number of Balloons in Box2D Animation
const int totalBalloons = 10;

/* -------Model ------ */
/// \brief totalGrowingStages - a constant that keeps track of how many growth stages the plant has
const int totalGrowingStages = 7;
#endif // CONSTS_H
