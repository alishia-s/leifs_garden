#include "mainwindow.h"
#include "qgraphicsscene.h"
#include "ui_mainwindow.h"

#include <QLayout>
#include <QGraphicsEllipseItem>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent),
    dialogueDisplayed(false),
    ui(new Ui::MainWindow)
{
    hasBugs = false;
    ui->setupUi(this);

    // CONNECTIONS WITHIN MAINWINDOW
    connect(this, &MainWindow::requestShowStartButton, this, &MainWindow::showStartButton);
    connect(this, &MainWindow::playSound, this, &MainWindow::playSoundEffect);
    connect(this, &MainWindow::changeLeifFace, this, &MainWindow::changeFace);
    connect(this, &MainWindow::requestLeif, this, &MainWindow::displayLeif);
    connect(this, &MainWindow::dismissLeif, this, &MainWindow::hideLeif);
    connect(this, &MainWindow::requestTools, this, &MainWindow::displayTools);
    connect(this, &MainWindow::bringBackWindow, this, &MainWindow::returnWindow);

    // CONNECTIONS TO MODEL
    connect(this, &MainWindow::getDialogue, &model, &Model::sendDialogue);
    connect(this, &MainWindow::progressDialogue, &model, &Model::progressDialogue);
    connect(this, &MainWindow::sendTask, &model, &Model::processTaskDone);
    connect(this, &MainWindow::incrementDay, &model, &Model::updateCurrentDay);
    connect(ui->notebookButton, &QPushButton::clicked, &model, &Model::sendNotebookInstructions);
    connect(ui->notebookBack, &QPushButton::clicked, &model, &Model::notebookPreviousPage);
    connect(ui->notebookForward, &QPushButton::clicked, &model, &Model::notebookNextPage);
    connect(ui->tomato, &QPushButton::clicked, &model, &Model::plantSelected);
    connect(ui->snakePlant, &QPushButton::clicked, &model, &Model::plantSelected);
    connect(ui->succulent, &QPushButton::clicked, &model, &Model::plantSelected);

    // CONNECTIONS FROM MODEL
    connect(&model, &Model::sendDisplayDialogue, this, &MainWindow::displayDialogue);
    connect(&model, &Model::endDialogue, this, &MainWindow::endDialogue);
    connect(&model, &Model::showPlantSelection, this, &MainWindow::displayPlantSelection);
    connect(&model, &Model::showNextDayButton, this, &MainWindow::showNextDayButton);
    connect(&model, &Model::sendDisplayInstructions, this, &MainWindow::displayNotebook);
    connect(&model, &Model::displayPlant, this, &MainWindow::displayPlant);
    connect(&model, &Model::sendCurrentTemp, ui->thermostatSpinBox, &QSpinBox::setValue);
    connect(&model, &Model::sendCurrentPH, ui->phMeterLabel, &QPushButton::setText);
    connect(&model, &Model::startNextDay, this, &MainWindow::returnWindow);
    connect(&model, &Model::enableAllButtons, this, &MainWindow::enablingButtons);
    connect(&model, &Model::handleNotebookNextButton, this, &MainWindow::upateNotebookNextButton);
    connect(&model, &Model::handleNotebookBackButton, this, &MainWindow::upateNotebookBackButton);

    // CONNECTIONS FOR NEXT DAY
    connect(&model, &Model::randomEvent, this, &MainWindow::newEvent);
    connect(&model, &Model::plantDied, this, &MainWindow::plantDeath);
    connect(&model, &Model::endGame, this, &MainWindow::endGame);
    connect(ui->nextDayButton, &QPushButton::clicked, this, &MainWindow::advanceDay);
    connect(ui->nextDayButton, &QPushButton::clicked, &model, &Model::progressNextDay);

    //CONNECTIONS FROM UI WITHIN MAINWINDOW
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::setUpUI);
    connect(ui->plantButton, &QPushButton::clicked, this, &MainWindow::plantClicked);
    connect(ui->restartButton, &QPushButton::clicked, this, &MainWindow::restartWorld);

    //CONNECTIONS FROM TOOLS WITHIN MAINWINDOW
    connect(ui->waterCanButton, &QPushButton::clicked, this, &MainWindow::toolButtonClicked);
    connect(ui->fertilizerButton, &QPushButton::clicked, this, &MainWindow::toolButtonClicked);
    connect(ui->pesticideButton, &QPushButton::clicked, this, &MainWindow::toolButtonClicked);
    connect(ui->phMeterButton, &QPushButton::clicked, this, &MainWindow::toolButtonClicked);
    connect(ui->phMeterLabel, &QPushButton::clicked, this, &MainWindow::toolButtonClicked);
    connect(ui->sulfurButton, &QPushButton::clicked, this, &MainWindow::toolButtonClicked);
    connect(ui->limestoneButton, &QPushButton::clicked, this, &MainWindow::toolButtonClicked);
    connect(ui->thermostatSpinBox, &QSpinBox::valueChanged, &model, &Model::changeTemp);
    connect(ui->tapButton, &QPushButton::clicked, this, &MainWindow::toolButtonClicked);
    connect(ui->helpMenuButton, &QPushButton::clicked, this,  [this]() {helpMenu.show();});

    this->setFixedSize(QSize(800, 500));
    this->setWindowIcon(QIcon(":/images/Images/programIcon.png"));
    this->setWindowTitle(QString("Leif's Garden"));

    ui->startButton->setVisible(false);
    ui->nextDayButton->setVisible(false);
    ui->snakePlant->setVisible(false);
    ui->tomato->setVisible(false);
    ui->succulent->setVisible(false);
    ui->selectPlantPrompt->setVisible(false);
    ui->waterAnimationLabel->setVisible(false);
    ui->graphicsView->setVisible(false);
    ui->plantButton->setVisible(false);
    ui->restartButton->setVisible(false);
    ui->graphicsView->setStyleSheet("background:transparent;");

    populateBalloons();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete notebookAnime;

    for(auto map: balloonImages)
    {
        delete map;
    }
}

void MainWindow::restartWorld()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::setUpAnimation(QPropertyAnimation* animator, int animeDuration, QPoint startPoints, QSize startSize,  QPoint endPoints, QSize endSize)
{
    animator->setDuration(animeDuration);
    animator->setStartValue(QRect(startPoints, startSize));
    animator->setEndValue(QRect(endPoints, endSize));
}

void MainWindow::newEvent(Model::event randomEvent)
{
    // Change the look of the plant
    if (randomEvent == Model::event::bugs)
    {
        hasBugs = true;
    }

    // Change hasBugs to false;
    if(randomEvent == Model::event::phChanged || randomEvent == Model::event::temperatureChanged)
    {
        hasBugs=false;
    }
}

void MainWindow::enablingButtons()
{
    ui->waterCanButton->setEnabled(true);
    ui->fertilizerButton->setEnabled(true);
    ui->pesticideButton->setEnabled(true);
    ui->phMeterLabel->setEnabled(true);
    ui->phMeterButton->setEnabled(true);
    ui->sulfurButton->setEnabled(true);
    ui->limestoneButton->setEnabled(true);
    ui->tapButton->setEnabled(true);
    ui->thermostatSpinBox->setEnabled(true);
    ui->notebookButton->setEnabled(true);
    ui->plantButton->setEnabled(true);
    ui->thermostatSpinBox->setEnabled(true);
}

void MainWindow::endGame()
{
    advanceDay();

    QTimer::singleShot(1000, this, [this](){celebrationScreen();});
}

void MainWindow::celebrationScreen()
{
    QPropertyAnimation* windowBackgroundAnim = new QPropertyAnimation(ui->windowBackground, "geometry", this);
    setUpAnimation(windowBackgroundAnim, endGameAnimationduration, backgroundLeftOffscreenPosition, backgroundSize, backgroundOnscreenPosition, backgroundSize);
    windowBackgroundAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* backgroundImageAnim = new QPropertyAnimation(ui->backgroundImage, "geometry", this);
    setUpAnimation(backgroundImageAnim, endGameAnimationduration, backgroundLeftOffscreenPosition, backgroundSize, backgroundOnscreenPosition, backgroundSize);
    backgroundImageAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* plantAnim = new QPropertyAnimation(ui->plantButton, "geometry", this);
    setUpAnimation(plantAnim, endGameAnimationduration, plantLeftOffscreenPosition, plantSize, plantOnscreenPosition, plantSize);
    plantAnim->start(QPropertyAnimation::DeleteWhenStopped);

    int leifWaitTime        = 1000;
    int celebrationWaitTime = 2000;
    QTimer::singleShot(leifWaitTime, this, [this](){emit requestLeif();});
    QTimer::singleShot(celebrationWaitTime, this, [this] () {emit getDialogue();});
    QTimer::singleShot(celebrationWaitTime, this, [this] () {drawBalloons();});
    QTimer::singleShot(celebrationWaitTime, [this](){this->displayRestartButton();});
}

void MainWindow::plantDeath()
{
    advanceDay();
    ambience.stop();

    QTimer::singleShot(1500, this, [this](){animatePlantDeathScreen();});
}

void MainWindow::animatePlantDeathScreen()
{
    ui->nextDayButton->setVisible(false);
    ui->restartButton->setVisible(true);
    ui->backgroundImage->setPixmap(QPixmap(":/images/Images/deathscreen.png"));

    // Animate plant death background
    QPropertyAnimation* backgroundAnim = new QPropertyAnimation(ui->backgroundImage, "geometry", this);
    setUpAnimation(backgroundAnim, deathAnimationDuration, backgroundLeftOffscreenPosition, backgroundSize, backgroundOnscreenPosition, backgroundSize);
    backgroundAnim->start(QPropertyAnimation::DeleteWhenStopped);

    // Animate the plant to appear from the top
    ui->plantButton->raise();
    QPropertyAnimation* plantAnim = new QPropertyAnimation(ui->plantButton, "geometry", this);
    setUpAnimation(plantAnim, deathAnimationDuration, plantFallingToDeathStartPosition, plantSize, plantOnscreenPosition, plantSize);
    connect(backgroundAnim, &QPropertyAnimation::finished, plantAnim, [=]{plantAnim->start(QPropertyAnimation::DeleteWhenStopped);});

    // Drop a skull with box2d
    QTimer::singleShot(2000, [this](){animateSkull();});
    connect(plantAnim, &QPropertyAnimation::finished, ui->plantButton, [=]{ui->plantButton->setEnabled(false);});
    QTimer::singleShot(2000, [this](){playSoundEffect(":/sounds/Sounds/plantDeathSound.wav", false);});

    // Show leif w/dialog
    QTimer::singleShot(6000, this, [this](){emit requestLeif();});
    QTimer::singleShot(7000, this, [this] () {emit getDialogue();});
    QTimer::singleShot(7000, [this](){changeFace(":/images/Images/Leif/confusedFaceLeft.png");});

    // Slide in restart button
    QTimer::singleShot(10000, [this](){displayRestartButton();});
}

void MainWindow::displayRestartButton()
{
    ui->restartButton->setIcon(QIcon(":/images/Images/restartButton.png"));
    ui->restartButton->setIconSize(QSize(160, 80));
    ui->restartButton->setGeometry(330, 200, 160, 80);
    ui->restartButton->setVisible(true);
    ui->restartButton->setFlat(true);
    ui->restartButton->raise();
}

void MainWindow::animateSkull()
{
    ui->skullLabel->setGeometry(QRect(365, 0, 80, 90));
    ui->skullLabel->setPixmap(QPixmap(":/images/Images/skull.png"));
    ui->skullLabel->setScaledContents(true);
    ui->skullLabel->raise();

    b2Vec2 gravity(0.0f, -10.0f);

    b2World* skullWorld = new b2World(gravity);

    b2BodyDef skullGroundDef;
    skullGroundDef.position.Set(0.0f, -10.0f);

    b2Body* skullGroundBody = skullWorld->CreateBody(&skullGroundDef);

    b2PolygonShape skullGroundBox;
    skullGroundBox.SetAsBox(50.0f, 10.0f);

    skullGroundBody->CreateFixture(&skullGroundBox, 0.0f);

    b2BodyDef skullDef;
    skullDef.type = b2_dynamicBody;
    skullDef.position.Set(0.0f, 4.0f);
    b2Body* skullBody = skullWorld->CreateBody(&skullDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef skullFixtureDef;
    skullFixtureDef.shape = &dynamicBox;
    skullFixtureDef.density=1.0f;
    skullFixtureDef.friction=0.1f;
    skullFixtureDef.restitution=4.0f;

    skullBody->CreateFixture(&skullFixtureDef);

    float32 timeStep = 1.0f/100.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 2;

    QPropertyAnimation* skullAnim = new QPropertyAnimation(ui->skullLabel, "pos", this);

    float32 time = 0.1;
    for (int i = 0; i < 100.0f; i++)
    {
        skullWorld->Step(timeStep, velocityIterations, positionIterations);

        b2Vec2 position = skullBody->GetPosition();

        if (position.y >= 2.5 && position.y <= 3.4){
            if(i % 10 == 0 && time <=1){
                skullAnim->setDuration(10000);
                skullAnim->setKeyValueAt(time, QPoint(365, position.y*90));
                time+=0.1;
            }
        }
    }
    skullAnim->setKeyValueAt(time, QPoint(365, 300));
    skullAnim->setEndValue(QPoint(365, 300));
    skullAnim->start(QPropertyAnimation::DeleteWhenStopped);

    skullWorld -> DestroyBody(skullGroundBody);
    skullWorld -> DestroyBody(skullBody);
    skullGroundBody = nullptr;
    skullBody = nullptr;
    delete skullWorld;
}

void MainWindow::setUpUI()
{
    playSoundEffect(":/sounds/Sounds/startJingle.wav", false);
    playAmbience(":/sounds/Sounds/birds.wav");
    QPropertyAnimation* titleCardOutroAnim = new QPropertyAnimation(ui->titleCard, "geometry", this);
    titleCardOutroAnim->setDuration(600);
    titleCardOutroAnim->setStartValue(ui->titleCard->geometry());
    titleCardOutroAnim->setEndValue(QRect(ui->titleCard->x(), ui->titleCard->y()-500, ui->titleCard->width(), ui->titleCard->height()));

    QPropertyAnimation* startButtonOutroAnim = new QPropertyAnimation(ui->startButton, "geometry", this);
    startButtonOutroAnim->setDuration(600);
    startButtonOutroAnim->setStartValue(ui->startButton->geometry());
    startButtonOutroAnim->setEndValue(QRect(ui->startButton->x(), ui->startButton->y()-500, ui->startButton->width(), ui->startButton->height()));

    startButtonOutroAnim->start(QPropertyAnimation::DeleteWhenStopped);
    titleCardOutroAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* decreaseMusicVolume = new QPropertyAnimation(&music, "volume", this);
    decreaseMusicVolume->setDuration(1000);
    decreaseMusicVolume->setStartValue(1.0);
    decreaseMusicVolume->setEndValue(0.0);
    decreaseMusicVolume->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* increaseAmbienceVolume = new QPropertyAnimation(&ambience, "volume", this);
    increaseAmbienceVolume->setDuration(1000);
    increaseAmbienceVolume->setStartValue(0.0);
    increaseAmbienceVolume->setEndValue(1.0);
    increaseAmbienceVolume->start(QPropertyAnimation::DeleteWhenStopped);

    ui->startButton->setDisabled(true);

    // Tool menu
    QTimer::singleShot(500, this, [this](){emit requestTools();});

    // Leif animations
    QTimer::singleShot(1500, this, [this](){emit requestLeif();});
    QTimer::singleShot(2500, this, [this](){emit changeLeifFace(":/images/Images/Leif/clickedFace.png");});
    QTimer::singleShot(2500, this, [this](){emit playSound(":/sounds/Sounds/intro.wav", false);});
    QTimer::singleShot(3500, this, [this](){emit changeLeifFace(":/images/Images/Leif/default.png");});
    QTimer::singleShot(4000, this, [this] () {emit getDialogue();});
}

void MainWindow::displayPlantSelection(bool visible)
{
    ui->snakePlant->setVisible(visible);
    ui->tomato->setVisible(visible);
    ui->succulent->setVisible(visible);
    ui->selectPlantPrompt->setVisible(visible);
}

void MainWindow::displayLeif()
{
    QPropertyAnimation* leifBodyIntroAnim = new QPropertyAnimation(ui->leif, "geometry", this);
    ui->leif->setScaledContents(true);
    ui->leif->setPixmap(QPixmap(":/images/Images/Leif/leifBase.png"));
    leifBodyIntroAnim->setDuration(600);
    leifBodyIntroAnim->setStartValue(QRect(600, 750, 250, 250));
    leifBodyIntroAnim->setEndValue(QRect(600, 250, 250, 250));
    leifBodyIntroAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* leifFaceIntroAnim = new QPropertyAnimation(ui->leifFace, "geometry", this);
    ui->leifFace->setScaledContents(true);
    ui->leifFace->setPixmap(QPixmap(":/images/Images/Leif/default.png"));
    leifFaceIntroAnim->setDuration(600);
    leifFaceIntroAnim->setStartValue(QRect(675, 830, 75, 75));
    leifFaceIntroAnim->setEndValue(QRect(675, 325, 75, 75));
    leifFaceIntroAnim->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::hideLeif()
{
    QPropertyAnimation* leifBodyOutroAnim = new QPropertyAnimation(ui->leif, "geometry", this);
    leifBodyOutroAnim->setDuration(600);
    leifBodyOutroAnim->setStartValue(QRect(600, 250, 250, 250));
    leifBodyOutroAnim->setEndValue(QRect(600, 750, 250, 250));
    leifBodyOutroAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* leifFaceOutroAnim = new QPropertyAnimation(ui->leifFace, "geometry", this);
    leifFaceOutroAnim->setDuration(600);
    leifFaceOutroAnim->setStartValue(QRect(675, 325, 75, 75));
    leifFaceOutroAnim->setEndValue(QRect(675, 830, 75, 75));
    leifFaceOutroAnim->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::displayTools()
{
    createPlantButton();
    createNotebook();
    createHelpMenuButton();
    createToolMenu();
}

void MainWindow::createPlantButton()
{
    ui->plantButton->setIcon(QIcon(toolsDirectory + fertilizer +".png"));
    ui->plantButton->setIconSize(QSize(300, 300));
    ui->plantButton->setFlat(true);
    ui->plantButton->setGeometry(325, 180, 150, 300);

    ui->plantButton->setObjectName(plantName);

    QPropertyAnimation* plantButtonAnim = new QPropertyAnimation(ui->plantButton, "geometry", this);
    plantButtonAnim->setDuration(600);
    plantButtonAnim->setStartValue(QRect(320, 500, 170, 170));
    plantButtonAnim->setEndValue(QRect(320, 265, 170, 170));
    plantButtonAnim->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::displayPlant(QString imagePath)
{
    ui->plantButton->setVisible(true);
    ui->plantButton->setIconSize(QSize(300,150));
    ui->plantButton->setIcon(QIcon(":/images/Images/Plants/"+imagePath +".png"));
}

void MainWindow::createNotebook()
{
    ui->notebookButton->setIcon(QIcon(":/images/Images/notebook.png"));
    ui->notebookButton->setIconSize(notebookButtonSize);
    ui->notebookButton->setFlat(true);
    ui->notebookButton->setGeometry(730, 10, 60, 60);

    ui->notebookBox->setVisible(false);
    ui->notebookBox->setScaledContents(true);
    ui->notebookBox->setPixmap(QPixmap(":/images/Images/notebook.png"));

    ui->notebookBack->setIcon(QIcon(":/images/Images/left-arrow.png"));
    ui->notebookBack->setIconSize(QSize(60, 20));
    ui->notebookBack->setFlat(true);
    ui->notebookBack->setVisible(false);
    ui->notebookBack->setGeometry(700, 15, 20, 40);
    ui->notebookForward->raise();

    ui->notebookForward->setIcon(QIcon(":/images/Images/right-arrow.png"));
    ui->notebookForward->setIconSize(QSize(60, 20));
    ui->notebookForward->setFlat(true);
    ui->notebookForward->setVisible(false);
    ui->notebookForward->setGeometry(720, 15, 20, 40);
    ui->notebookForward->raise();
}

void MainWindow::createHelpMenuButton()
{
    ui->helpMenuButton->setIcon(QIcon(":/images/Images/questionMarkIcon.png"));
    ui->helpMenuButton->setIconSize(QSize(60,60));
    ui->helpMenuButton->setFlat(true);
    ui->helpMenuButton->setGeometry(650, 10, 60,60);
}

void MainWindow::createToolButton(QPushButton* button, QString toolName, QSize buttonSize)
{
    button->setIcon(QIcon(toolsDirectory + toolName + ".png"));
    button->setIconSize(buttonSize);
    button->setFlat(true);
    button->setObjectName(toolName);
}

void MainWindow::createToolMenu()
{
    int buttonCounter = 0;
    int currentYPosition = 0;

    // ToolMenu Animation Setup
    ui->toolMenu->setScaledContents(true);
    ui->toolMenu->setPixmap(QPixmap(":/images/Images/tasklist.png"));
    QPropertyAnimation* toolMenuAnime = new QPropertyAnimation(ui->toolMenu, "geometry", this);
    setUpAnimation(toolMenuAnime, toolMenuAnimationduration, QPoint(-300, -30), QSize(160, 550), QPoint(0,-30), QSize(160, 550));

    // Watering Can Button & Animation Setup
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    createToolButton(ui->waterCanButton, wateringCan, wateringCanSize);
    QPropertyAnimation* wateringCanAnime = new QPropertyAnimation(ui->waterCanButton, "geometry", this);
    setUpAnimation(wateringCanAnime, toolMenuAnimationduration, QPoint(offscreenToolXPositon - 13, currentYPosition), wateringCanSize,
                                                                 QPoint(toolPostionX - 13, currentYPosition), wateringCanSize);
    // Fertilizer Button & Animation Setup
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    createToolButton(ui->fertilizerButton, fertilizer, toolButtonSize);
    QPropertyAnimation* fertilizerAnime = new QPropertyAnimation(ui->fertilizerButton, "geometry", this);
    setUpAnimation(fertilizerAnime, toolMenuAnimationduration, QPoint(offscreenToolXPositon, currentYPosition), toolButtonSize,
                                                                QPoint(toolPostionX, currentYPosition), toolButtonSize);

    // Pesticide Button & Animation Setup
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    createToolButton(ui->pesticideButton, pesticide, toolButtonSize);
    QPropertyAnimation* pesticideAnime = new QPropertyAnimation(ui->pesticideButton, "geometry", this);
    setUpAnimation(pesticideAnime, toolMenuAnimationduration, QPoint(offscreenToolXPositon, currentYPosition), toolButtonSize,
                                                               QPoint(toolPostionX, currentYPosition), toolButtonSize);

    // PhMeter Button & Animation Setup
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    ui->phMeterLabel->setText("7.0");
    ui->phMeterLabel->setStyleSheet(QString("QPushButton {background-color: white; color: red; border: none;}"));
    ui->phMeterLabel->setFlat(true);
    ui->phMeterLabel->setObjectName(phMeter);
    createToolButton(ui->phMeterButton, phMeter, toolButtonSize);
    QPropertyAnimation* phMeterAnime = new QPropertyAnimation(ui->phMeterButton, "geometry", this);
    setUpAnimation(phMeterAnime, toolMenuAnimationduration, QPoint(offscreenToolXPositon, currentYPosition), toolButtonSize,
                                                            QPoint(toolPostionX, currentYPosition), toolButtonSize);
    QPropertyAnimation* phMeterLabelAnime = new QPropertyAnimation(ui->phMeterLabel, "geometry", this);
    setUpAnimation(phMeterLabelAnime, toolMenuAnimationduration, QPoint(offscreenToolXPositon + 15, currentYPosition + 4), phMeterLabelSize,
                                                                 QPoint(toolPostionX + 15, currentYPosition + 4), phMeterLabelSize);

    // Limestone Button & Animation Setup
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    createToolButton(ui->limestoneButton, limestone, toolButtonSize);
    QPropertyAnimation* limestoneAnime = new QPropertyAnimation(ui->limestoneButton, "geometry", this);
    setUpAnimation(limestoneAnime, toolMenuAnimationduration, QPoint(offscreenToolXPositon, currentYPosition), toolButtonSize,
                                                               QPoint(toolPostionX, currentYPosition), toolButtonSize);

    // Sulfur Button & Animation Setup
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    createToolButton(ui->sulfurButton, sulfur, sulfurSize);
    QPropertyAnimation* sulfurAnime = new QPropertyAnimation(ui->sulfurButton, "geometry", this);
    setUpAnimation(sulfurAnime, toolMenuAnimationduration, QPoint(offscreenToolXPositon - 5, currentYPosition - 10), sulfurSize,
                                                            QPoint(toolPostionX - 5, currentYPosition - 10), sulfurSize);

    // Tap Button & Animation Setup
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    createToolButton(ui->tapButton, tap, toolButtonSize);
    QPropertyAnimation* tapAnime = new QPropertyAnimation(ui->tapButton, "geometry", this);
    setUpAnimation(tapAnime, toolMenuAnimationduration, QPoint(offscreenToolXPositon, currentYPosition), toolButtonSize,
                                                         QPoint(toolPostionX, currentYPosition), toolButtonSize);

    // Thermostat & Animation Setup
    ui->thermostatLabel->setPixmap(QPixmap(toolsDirectory + thermostat + ".png"));
    ui->thermostatLabel->setGeometry(680, 250, 200, 130);
    ui->thermostatLabel->setScaledContents(true);
    QPropertyAnimation* thermostatAnime = new QPropertyAnimation(ui->thermostatLabel, "geometry", this);
    setUpAnimation(thermostatAnime, toolMenuAnimationduration, QPoint(887, 225), QSize(200, 130), QPoint(587, 225), QSize(200, 130));
    ui->thermostatSpinBox->setObjectName(thermostat);
    QPropertyAnimation* thermostatSpinBoxAnime = new QPropertyAnimation(ui->thermostatSpinBox, "geometry", this);
    setUpAnimation(thermostatSpinBoxAnime, toolMenuAnimationduration, QPoint(950, 275), QSize(37, 20), QPoint(650, 269), QSize(37, 20));

    // Start Animations
    toolMenuAnime->start(QPropertyAnimation::DeleteWhenStopped);
    wateringCanAnime->start(QPropertyAnimation::DeleteWhenStopped);
    fertilizerAnime->start(QPropertyAnimation::DeleteWhenStopped);
    pesticideAnime->start(QPropertyAnimation::DeleteWhenStopped);
    phMeterAnime->start(QPropertyAnimation::DeleteWhenStopped);
    phMeterLabelAnime->start(QPropertyAnimation::DeleteWhenStopped);
    sulfurAnime->start(QPropertyAnimation::DeleteWhenStopped);
    limestoneAnime->start(QPropertyAnimation::DeleteWhenStopped);
    tapAnime->start(QPropertyAnimation::DeleteWhenStopped);
    thermostatAnime->start(QPropertyAnimation::DeleteWhenStopped);
    thermostatSpinBoxAnime->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::playSoundEffect(QString soundPath, bool infinite)
{
    effect.setSource(QUrl::fromLocalFile(soundPath));
    if(infinite)
    {
        effect.setLoopCount(QSoundEffect::Infinite);
    }
    else
    {
        effect.setLoopCount(1);
    }

    effect.setVolume(1.0);
    effect.play();
}

void MainWindow::playMusic(QString soundPath)
{
    music.setSource(QUrl::fromLocalFile(soundPath));
    music.setLoopCount(QSoundEffect::Infinite);
    music.setVolume(1.0);
    music.play();
}

void MainWindow::playAmbience(QString soundPath)
{
    ambience.setSource(QUrl::fromLocalFile(soundPath));
    ambience.setLoopCount(QSoundEffect::Infinite);
    ambience.setVolume(0.0);
    ambience.play();
}

void MainWindow::changeFace(QString imagePath)
{
    ui->leifFace->setPixmap(QPixmap(imagePath));
    ui->leifFace->setGeometry(675, 325, 75, 75);
}

void MainWindow::displayDialogue(QString dialogue)
{
    int id = QFontDatabase::addApplicationFont(":/font/Fonts/amber.otf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);

    font.setPointSize(24);

    QPixmap speechBubble = QPixmap(":/images/Images/speechbubble.png");
    ui->speechBubble->setScaledContents(true);
    ui->speechBubble->setPixmap(speechBubble);
    ui->speechBubble->setGeometry(QRect(180, 30, 500, 200));

    ui->dialogueBox->setFont(font);
    ui->dialogueBox->setWordWrap(true);
    ui->dialogueBox->setText(dialogue);
    ui->dialogueBox->setGeometry(QRect(200, 60, 450, 90));

    font.setPointSize(12);
    ui->continuePrompt->setFont(font);
    ui->continuePrompt->setGeometry(QRect(475, 150, 200, 20));

    ui->speechBubble->setVisible(true);
    ui->dialogueBox->setVisible(true);
    ui->continuePrompt->setVisible(true);

    dialogueDisplayed = true;
}

void MainWindow::mousePressEvent(QMouseEvent* clicked)
{
    if(dialogueDisplayed)
    {
        emit progressDialogue();
    }

    if(ui->notebookBox->isVisible() && !isNotebookClosing)
    {
        notebookAnimation(false, QString());
        QTimer::singleShot(notebookAnimationduration, this, [this] () {disableNotebookButtons();});
    }
}

void MainWindow::upateNotebookNextButton(bool enable)
{
    if(!ui->notebookBox->isVisible())
    {
        QTimer::singleShot(notebookAnimationduration, this, [=](){ui->notebookForward->setEnabled(enable);});
        return;
    }

    ui->notebookForward->setEnabled(enable);
}

void MainWindow::upateNotebookBackButton(bool enable)
{
    if(!ui->notebookBox->isVisible())
    {
        QTimer::singleShot(notebookAnimationduration, this, [=](){ui->notebookBack->setEnabled(enable);});
        return;
    }

    ui->notebookBack->setEnabled(enable);
}

void MainWindow::disableNotebookButtons()
{
    ui->notebookBox->setVisible(false);
    ui->notebookButton->setVisible(true);
}

void MainWindow::displayNotebook(QString instructions)
{
    if(isNotebookOpen)
    {
        setNotebookText(instructions);
        return;
    }

    notebookAnimation(true, instructions);
}

void MainWindow::notebookAnimation(bool openNotebook, QString instructions)
{
    ui->notebookButton->setVisible(false);
    QLabel *frame = ui->notebookBox;
    notebookAnime = new QPropertyAnimation(frame, "geometry", this);

    if(openNotebook)
    {
        isNotebookClosing = false;
        isNotebookOpen    = true;

        frame->setVisible(openNotebook);
        setUpAnimation(notebookAnime, notebookAnimationduration, notebookOffsceenPosition, notebookClosedSize, notebookOnsceenPosition, notebookSize);
        notebookAnime->start(QPropertyAnimation::DeleteWhenStopped);

        QTimer::singleShot(notebookAnimationduration, this, [=] () {setNotebookText(instructions);});
    }
    else
    {
        isNotebookClosing = true;
        isNotebookOpen    = false;

        ui->notebookText->setVisible(openNotebook);
        ui->notebookForward->setVisible(false);
        ui->notebookBack->setVisible(false);

        QPoint framePosition(frame->x(), frame->y());
        QSize frameSize(frame->width(), frame->height());
        setUpAnimation(notebookAnime, notebookAnimationduration, framePosition, frameSize , notebookOffsceenPosition, notebookClosedSize);
        notebookAnime->start(QPropertyAnimation::DeleteWhenStopped);
    }
}

void MainWindow::setNotebookText(QString text)
{
    QImage instructionImage = QImage(text);

    ui->notebookBack->setVisible(!isNotebookClosing);
    ui->notebookForward->setVisible(!isNotebookClosing);

    ui->notebookText->setPixmap(QPixmap::fromImage(instructionImage));
    ui->notebookText->setGeometry(notebookTextBox);
    ui->notebookText->setVisible(!isNotebookClosing);
}

void MainWindow::endDialogue()
{
    ui->speechBubble->setVisible(false);
    ui->dialogueBox->setVisible(false);
    ui->continuePrompt->setVisible(false);
    dialogueDisplayed = false;

    QTimer::singleShot(300, this, [this](){emit dismissLeif();});
}

void MainWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    showTitleCard();
}

void MainWindow::showTitleCard()
{
    playMusic(":/sounds/Sounds/music.wav");
    QPropertyAnimation* titleCardIntroAnim = new QPropertyAnimation(ui->titleCard, "geometry", this);
    QPropertyAnimation* backgroundIntroAnimation = new QPropertyAnimation(ui->backgroundImage, "geometry", this);
    QPropertyAnimation* startButtonIntroAnimation = new QPropertyAnimation(ui->startButton, "geometry", this);

    titleCardIntroAnim->setDuration(3500);
    titleCardIntroAnim->setStartValue(QRect(220, -550, 372, 300));
    titleCardIntroAnim->setEndValue(QRect(220,10, 372, 300 ));

    backgroundIntroAnimation->setDuration(3500);
    backgroundIntroAnimation->setStartValue(QRect(0, -500, 800, 500));
    backgroundIntroAnimation->setEndValue(QRect(0,0, 800, 500));

    startButtonIntroAnimation->setDuration(3500);
  
    backgroundIntroAnimation->start(QPropertyAnimation::DeleteWhenStopped);
    titleCardIntroAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QTimer::singleShot(3500, this, [this] () {emit requestShowStartButton();});
}

void MainWindow::showStartButton()
{
    ui->startButton->setVisible(true);
    ui->startButton->setGeometry(290,300, 230, 80);
    QPropertyAnimation* startButtonIntroIcon = new QPropertyAnimation(ui->startButton, "iconSize", this);
    startButtonIntroIcon->setDuration(600);
    startButtonIntroIcon->setStartValue(QSize(0,0));
    startButtonIntroIcon->setEndValue(QSize(225,75));
    startButtonIntroIcon->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::showNextDayButton()
{
    ui->nextDayButton->setIcon(QIcon(":/images/Images/nextDayButton.png"));
    ui->nextDayButton->setIconSize(QSize(160, 80));
    ui->nextDayButton->setFlat(true);
    ui->nextDayButton->setGeometry(642, 410, 160, 80);
    ui->nextDayButton->setVisible(true);
}

void MainWindow::advanceDay()
{
    int buttonCounter    = 0;
    int currentYPosition = 0;

    notebookAnimation(false, "");
    ui->notebookButton->setVisible(true);

    ui->nextDayButton->setVisible(false);
    QImage nightBackground = QImage(":/images/Images/nighttime.png").scaled(screenWidth, screenHeight);
    ui->nightBackgroundImage->setPixmap(QPixmap::fromImage(nightBackground));

    if (hasBugs)
    {
        QPropertyAnimation* bugsAnim = new QPropertyAnimation(ui->bugsLabel, "geometry", this);
        ui->bugsLabel->setPixmap(QPixmap(":/images/Images/bugs.png"));
        ui->bugsLabel->setGeometry(220, 265, 100, 60);
        ui->bugsLabel->setScaledContents(true);
        setUpAnimation(bugsAnim, advanceDayAnimationDuration, QPoint(350, 280), QSize(110, 40), QPoint(800, 280), QSize(110, 40));
        bugsAnim->start(QPropertyAnimation::DeleteWhenStopped);
        effect.setVolume(1.0);
    }

    QPropertyAnimation* windowBackgroundAnim = new QPropertyAnimation(ui->windowBackground, "geometry", this);
    setUpAnimation(windowBackgroundAnim, advanceDayAnimationDuration, backgroundOnscreenPosition, backgroundSize, backgroundRightOffscreenPosition, backgroundSize);
    windowBackgroundAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* backgroundImageAnim = new QPropertyAnimation(ui->backgroundImage, "geometry", this);
    setUpAnimation(backgroundImageAnim, advanceDayAnimationDuration, backgroundOnscreenPosition, backgroundSize, backgroundRightOffscreenPosition, backgroundSize);
    backgroundImageAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* notebookAnime = new QPropertyAnimation(ui->notebookButton, "geometry", this);
    setUpAnimation(notebookAnime, advanceDayAnimationDuration, QPoint(730, 10), notebookButtonSize, QPoint(730, -350), notebookButtonSize);

    QPropertyAnimation* helpButtonAnim = new QPropertyAnimation(ui->helpMenuButton, "geometry", this);
    setUpAnimation(helpButtonAnim, advanceDayAnimationDuration, QPoint(650, 10), QSize(60, 60), QPoint(650, -350), QSize(60, 60));

    QPropertyAnimation* plantAnim = new QPropertyAnimation(ui->plantButton, "geometry", this);
    setUpAnimation(plantAnim, advanceDayAnimationDuration, plantOnscreenPosition, plantSize, plantRightOffscreenPosition, plantSize);
    plantAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* notebookAnim = new QPropertyAnimation(ui->notebookButton, "geometry", this);
    setUpAnimation(notebookAnim, advanceDayAnimationDuration, QPoint(730,0), notebookButtonSize, QPoint(1530,0), notebookButtonSize);
    notebookAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* toolMenuAnim = new QPropertyAnimation(ui->toolMenu, "geometry", this);
    setUpAnimation(toolMenuAnim, advanceDayAnimationDuration, QPoint(0, -30), QSize(160, 550), QPoint(800, -30), QSize(160, 550));
    toolMenuAnim->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* wateringCanAnime = new QPropertyAnimation(ui->waterCanButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(wateringCanAnime, advanceDayAnimationDuration, QPoint(toolPostionX - 13, currentYPosition), wateringCanSize,
                                                                  QPoint(offscreenAdvanceDayXPositon - 13, currentYPosition), wateringCanSize);

    QPropertyAnimation* fertilizerAnime = new QPropertyAnimation(ui->fertilizerButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(fertilizerAnime, advanceDayAnimationDuration, QPoint(toolPostionX, currentYPosition), toolButtonSize,
                                                                 QPoint(offscreenAdvanceDayXPositon, currentYPosition), toolButtonSize);

    QPropertyAnimation* pesticideAnime = new QPropertyAnimation(ui->pesticideButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(pesticideAnime, advanceDayAnimationDuration, QPoint(toolPostionX, currentYPosition), toolButtonSize,
                                                                QPoint(offscreenAdvanceDayXPositon, currentYPosition), toolButtonSize);

    QPropertyAnimation* phMeterAnime = new QPropertyAnimation(ui->phMeterButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(phMeterAnime, advanceDayAnimationDuration, QPoint(toolPostionX, currentYPosition), toolButtonSize,
                                                              QPoint(offscreenAdvanceDayXPositon, currentYPosition), toolButtonSize);
    ui->phMeterLabel->setVisible(false);

    QPropertyAnimation* limestoneAnime = new QPropertyAnimation(ui->limestoneButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(limestoneAnime, advanceDayAnimationDuration, QPoint(toolPostionX, currentYPosition), toolButtonSize,
                                                                QPoint(offscreenAdvanceDayXPositon, currentYPosition), toolButtonSize);

    QPropertyAnimation* sulfurAnime = new QPropertyAnimation(ui->sulfurButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(sulfurAnime, advanceDayAnimationDuration, QPoint(toolPostionX - 5, currentYPosition - 10), sulfurSize,
                                                             QPoint(offscreenAdvanceDayXPositon - 5, currentYPosition - 10), sulfurSize);

    QPropertyAnimation* tapAnime = new QPropertyAnimation(ui->tapButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(tapAnime, advanceDayAnimationDuration, QPoint(toolPostionX, currentYPosition), toolButtonSize,
                                                          QPoint(offscreenAdvanceDayXPositon, currentYPosition), toolButtonSize);

    QPropertyAnimation* thermostatAnime = new QPropertyAnimation(ui->thermostatLabel, "geometry", this);
    setUpAnimation(thermostatAnime, advanceDayAnimationDuration, QPoint(587, 225), QSize(200, 130), QPoint(1387, 225), QSize(200, 130));

    QPropertyAnimation* thermostatSpinBoxAnime = new QPropertyAnimation(ui->thermostatSpinBox, "geometry", this);
    setUpAnimation(thermostatSpinBoxAnime, advanceDayAnimationDuration, QPoint(650, 269), QSize(37, 20), QPoint(1450, 269), QSize(37, 20));

    notebookAnime->start(QPropertyAnimation::DeleteWhenStopped);
    helpButtonAnim->start(QPropertyAnimation::DeleteWhenStopped);
    wateringCanAnime->start(QPropertyAnimation::DeleteWhenStopped);
    fertilizerAnime->start(QPropertyAnimation::DeleteWhenStopped);
    pesticideAnime->start(QPropertyAnimation::DeleteWhenStopped);
    phMeterAnime->start(QPropertyAnimation::DeleteWhenStopped);
    sulfurAnime->start(QPropertyAnimation::DeleteWhenStopped);
    limestoneAnime->start(QPropertyAnimation::DeleteWhenStopped);
    tapAnime->start(QPropertyAnimation::DeleteWhenStopped);
    thermostatAnime->start(QPropertyAnimation::DeleteWhenStopped);
    thermostatSpinBoxAnime->start(QPropertyAnimation::DeleteWhenStopped);

    emit incrementDay();
}

void MainWindow::returnWindow()
{
    int buttonCounter = 0;
    int currentYPosition = 0;

    if (hasBugs)
    {
        QPropertyAnimation* bugsAnim = new QPropertyAnimation(ui->bugsLabel, "geometry", this);
        ui->bugsLabel->setPixmap(QPixmap(":/images/Images/bugs.png"));
        ui->bugsLabel->setGeometry(220, 265, 100, 60);
        ui->bugsLabel->setScaledContents(true);
        setUpAnimation(bugsAnim, advanceDayAnimationDuration, QPoint(-480, 280), QSize(110, 40), QPoint(350, 280), QSize(110, 40));
        bugsAnim->start(QPropertyAnimation::DeleteWhenStopped);
        playSoundEffect(":/sounds/Sounds/bugs.wav", true);
    }

    QPropertyAnimation* windowBackgroundAnim = new QPropertyAnimation(ui->windowBackground, "geometry", this);
    setUpAnimation(windowBackgroundAnim, advanceDayAnimationDuration, backgroundLeftOffscreenPosition, backgroundSize, backgroundOnscreenPosition, backgroundSize);

    QPropertyAnimation* backgroundImageAnim = new QPropertyAnimation(ui->backgroundImage, "geometry", this);
    setUpAnimation(backgroundImageAnim, advanceDayAnimationDuration, backgroundLeftOffscreenPosition, backgroundSize, backgroundOnscreenPosition, backgroundSize);

    QPropertyAnimation* plantAnime = new QPropertyAnimation(ui->plantButton, "geometry", this);
    setUpAnimation(plantAnime, advanceDayAnimationDuration, QPoint(-480, 265), plantSize, plantOnscreenPosition, plantSize);

    QPropertyAnimation* notebookAnime = new QPropertyAnimation(ui->notebookButton, "geometry", this);
    ui->notebookButton->setVisible(true);
    setUpAnimation(notebookAnime, advanceDayAnimationDuration, QPoint(730, -700), notebookButtonSize, QPoint(730, 10), notebookButtonSize);

    QPropertyAnimation* helpButtonAnim = new QPropertyAnimation(ui->helpMenuButton, "geometry", this);
    ui->helpMenuButton->setVisible(true);
    setUpAnimation(helpButtonAnim, advanceDayAnimationDuration, QPoint(650, -700), QSize(60, 60), QPoint(650, 10), QSize(60, 60));

    QPropertyAnimation* toolMenuAnime = new QPropertyAnimation(ui->toolMenu, "geometry", this);
    setUpAnimation(toolMenuAnime, advanceDayAnimationDuration, QPoint(-800, -20), QSize(160, 550), QPoint(0, -30), QSize(160, 550));

    QPropertyAnimation* wateringCanAnime = new QPropertyAnimation(ui->waterCanButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(wateringCanAnime, advanceDayAnimationDuration, QPoint(offscreenReturnWindowXPositon - 13, currentYPosition), wateringCanSize,
                                                                  QPoint(toolPostionX - 13, currentYPosition), wateringCanSize);

    QPropertyAnimation* fertilizerAnime = new QPropertyAnimation(ui->fertilizerButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(fertilizerAnime, advanceDayAnimationDuration, QPoint(offscreenReturnWindowXPositon, currentYPosition), toolButtonSize,
                                                                 QPoint(toolPostionX, currentYPosition), toolButtonSize);

    QPropertyAnimation* pesticideAnime = new QPropertyAnimation(ui->pesticideButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(pesticideAnime, advanceDayAnimationDuration, QPoint(offscreenReturnWindowXPositon, currentYPosition), toolButtonSize,
                                                                QPoint(toolPostionX, currentYPosition), toolButtonSize);

    QPropertyAnimation* phMeterAnime = new QPropertyAnimation(ui->phMeterButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(phMeterAnime, advanceDayAnimationDuration, QPoint(offscreenReturnWindowXPositon, currentYPosition), toolButtonSize,
                                                              QPoint(toolPostionX, currentYPosition), toolButtonSize);

    QPropertyAnimation* limestoneAnime = new QPropertyAnimation(ui->limestoneButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(limestoneAnime, advanceDayAnimationDuration, QPoint(offscreenReturnWindowXPositon, currentYPosition), toolButtonSize,
                   QPoint(toolPostionX, currentYPosition), toolButtonSize);

    QPropertyAnimation* sulfurAnime = new QPropertyAnimation(ui->sulfurButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(sulfurAnime, advanceDayAnimationDuration, QPoint(offscreenReturnWindowXPositon - 5, currentYPosition - 10), sulfurSize,
                                                             QPoint(toolPostionX - 5, currentYPosition - 10), sulfurSize);

    QPropertyAnimation* tapAnime = new QPropertyAnimation(ui->tapButton, "geometry", this);
    currentYPosition = firstToolPositonY + buttonCounter++ * spacingBetweenToolsY;
    setUpAnimation(tapAnime, advanceDayAnimationDuration, QPoint(offscreenReturnWindowXPositon, currentYPosition), toolButtonSize,
                                                          QPoint(toolPostionX, currentYPosition), toolButtonSize);

    QPropertyAnimation* thermostatAnime = new QPropertyAnimation(ui->thermostatLabel, "geometry", this);
    setUpAnimation(thermostatAnime, advanceDayAnimationDuration, QPoint(-213, 225), QSize(200,130), QPoint(587, 225), QSize(200,130));

    QPropertyAnimation* thermostatSpinBoxAnime = new QPropertyAnimation(ui->thermostatSpinBox, "geometry", this);
    setUpAnimation(thermostatSpinBoxAnime, advanceDayAnimationDuration, QPoint(-150, 269), QSize(37,20), QPoint(650, 269), QSize(37,20));

    windowBackgroundAnim->start(QPropertyAnimation::DeleteWhenStopped);
    backgroundImageAnim->start(QPropertyAnimation::DeleteWhenStopped);
    plantAnime->start(QPropertyAnimation::DeleteWhenStopped);
    notebookAnime->start(QPropertyAnimation::DeleteWhenStopped);
    helpButtonAnim->start(QPropertyAnimation::DeleteWhenStopped);
    toolMenuAnime->start(QPropertyAnimation::DeleteWhenStopped);
    wateringCanAnime->start(QPropertyAnimation::DeleteWhenStopped);
    fertilizerAnime->start(QPropertyAnimation::DeleteWhenStopped);
    pesticideAnime->start(QPropertyAnimation::DeleteWhenStopped);
    phMeterAnime->start(QPropertyAnimation::DeleteWhenStopped);
    sulfurAnime->start(QPropertyAnimation::DeleteWhenStopped);
    limestoneAnime->start(QPropertyAnimation::DeleteWhenStopped);
    tapAnime->start(QPropertyAnimation::DeleteWhenStopped);
    thermostatAnime->start(QPropertyAnimation::DeleteWhenStopped);
    thermostatSpinBoxAnime->start(QPropertyAnimation::DeleteWhenStopped);

    QTimer::singleShot(advanceDayAnimationDuration, this, [this] () {ui->phMeterLabel->setVisible(true);});
    QTimer::singleShot(1000, this, [this] () {emit requestLeif();});
    QTimer::singleShot(2000, this, [this] () {emit getDialogue();});
}

void MainWindow::toolButtonClicked()
{
    QObject *senderObj = sender();
    QString toolName = senderObj->objectName();

    int width = 80;

    if (currentTool != "")
    {
        resetButtonVisibility();
    }

    if (toolName == wateringCan)
    {
        ui->waterCanButton->setVisible(false);
        width = ui->waterCanButton->iconSize().width();
    }
    else if (toolName == fertilizer)
    {
        ui->fertilizerButton->setVisible(false);
        width = ui->fertilizerButton->iconSize().width();
    }
    else if (toolName == pesticide)
    {
        ui->pesticideButton->setVisible(false);
        width = ui->pesticideButton->iconSize().width();
    }
    else if (toolName == phMeter)
    {
        ui->phMeterButton->setVisible(false);
        ui->phMeterLabel->setVisible(false);
        width = ui->phMeterButton->iconSize().width();
    }
    else if (toolName == sulfur)
    {
        ui->sulfurButton->setVisible(false);
        width = ui->sulfurButton->iconSize().width();
    }
    else if (toolName == limestone)
    {
        ui->limestoneButton->setVisible(false);
        width = ui->limestoneButton->iconSize().width();
    }

    if (toolName == tap)
    {
        setCursor(Qt::ArrowCursor);
        currentTool = "";
    }
    else
    {
        QPixmap cursorPixmap(toolsDirectory + toolName + ".png");
        cursorPixmap = cursorPixmap.scaledToWidth(width);

        setCursor(QCursor(cursorPixmap));
        currentTool = toolName;
    }
}

void MainWindow::plantClicked()
{
    if (currentTool != "")
    {
        if (currentTool == wateringCan)
        {
            playSoundEffect(":/sounds/Sounds/water.wav", false);
            playWaterAnimation();
            emit sendTask(Model::task::wateringCan);
        }
        else if (currentTool == fertilizer)
        {
            resetButtonVisibility();
            emit sendTask(Model::task::fertilizer);
        }
        else if (currentTool == pesticide)
        {
            resetButtonVisibility();
            emit sendTask(Model::task::pesticide);

            ui->bugsLabel->setScaledContents(false);
            effect.stop();
            if(hasBugs)
            {
                playSoundEffect(":/sounds/Sounds/startJingle.wav", false);
            }
            hasBugs = false;
        }
        else if (currentTool == phMeter)
        {
            resetButtonVisibility();
            emit sendTask(Model::task::phMeter);
        }
        else if (currentTool == sulfur)
        {
            resetButtonVisibility();
            emit sendTask(Model::task::sulfur);
        }
        else if (currentTool == limestone)
        {
            resetButtonVisibility();
            emit sendTask(Model::task::limestone);
        }
        setCursor(Qt::ArrowCursor);
    }
    currentTool = "";
}

void MainWindow::resetButtonVisibility()
{
    if (currentTool == wateringCan)
    {
        ui->waterCanButton->setVisible(true);
    }
    else if (currentTool == fertilizer)
    {
        ui->fertilizerButton->setVisible(true);
    }
    else if (currentTool == pesticide)
    {
        ui->pesticideButton->setVisible(true);
    }
    else if (currentTool == phMeter)
    {
        ui->phMeterButton->setVisible(true);
        ui->phMeterLabel->setVisible(true);
    }
    else if (currentTool == sulfur)
    {
        ui->sulfurButton->setVisible(true);
    }
    else if (currentTool == limestone)
    {
        ui->limestoneButton->setVisible(true);
    }
    else if (currentTool == tap)
    {
        ui->tapButton->setVisible(true);
    }
}

void MainWindow::playWaterAnimation()
{
    ui->waterAnimationLabel->setVisible(true);
    QPixmap image = ui->waterAnimationLabel->pixmap();
    QVariantAnimation* waterTiltForwardAnimation = new QVariantAnimation;
    waterTiltForwardAnimation->setDuration(600);
    waterTiltForwardAnimation->setStartValue(0.0f);
    waterTiltForwardAnimation->setEndValue(-30.0f);
    connect(waterTiltForwardAnimation, &QVariantAnimation::valueChanged, this, [=](const QVariant &value)
    {
        QTransform t;
        t.rotate(value.toReal());
        ui->waterAnimationLabel->setPixmap(image.transformed(t));
    });

    QVariantAnimation* waterTiltBackwardAnimation = new QVariantAnimation;
    waterTiltBackwardAnimation->setDuration(400);
    waterTiltBackwardAnimation->setStartValue(-30.0f);
    waterTiltBackwardAnimation->setEndValue(0.0f);
    connect(waterTiltBackwardAnimation, &QVariantAnimation::valueChanged, this, [=](const QVariant &value)
    {
        QTransform t;
        t.rotate(value.toReal());
        ui->waterAnimationLabel->setPixmap(image.transformed(t));
    });

    // Does nothing for 1 second.
    QPropertyAnimation* wait = new QPropertyAnimation(ui->waterAnimationLabel, "geometry", this);
    wait->setStartValue(ui->waterAnimationLabel->geometry());
    wait->setEndValue(ui->waterAnimationLabel->geometry());
    wait->setDuration(1000);

    connect(waterTiltForwardAnimation, &QVariantAnimation::finished, wait, [=]
    {
        wait->start();
        playWaterDropletAnimation();
    });
    connect(wait, &QPropertyAnimation::finished, waterTiltBackwardAnimation, [=]{waterTiltBackwardAnimation->start();});
    connect(waterTiltBackwardAnimation, &QVariantAnimation::finished, this, [=]
    {
        ui->waterAnimationLabel->setVisible(false);
        ui->waterAnimationLabel->setPixmap(QPixmap(":/images/Images/Tools/wateringCan.png"));
        ui->waterCanButton->setVisible(true);
        timer.stop();
        ui->graphicsView->setVisible(false);
    });

    waterTiltForwardAnimation->start();
}

void MainWindow::playWaterDropletAnimation()
{
    ui->graphicsView->setVisible(true);
    scene = new QGraphicsScene(0,209,800,151, this);
    ui->graphicsView->setStyleSheet("background:transparent;");

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFrameShape(QFrame::NoFrame);

    b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
    b2World* world = new b2World(gravity);

    int dropletWidth = 5;
    QPixmap * dropletImage = new QPixmap(QPixmap(":/images/Images/waterDroplet.png").scaledToWidth(dropletWidth));

    for(int i= 0; i < 100; i++)
    {
        b2BodyDef dropletDef;
        dropletDef.type = b2_dynamicBody;
        dropletDef.position.Set(3.0f,1.0f);
        b2Body* droplet = world->CreateBody(&dropletDef);

        b2PolygonShape dropletShape;
        dropletShape.SetAsBox(0.05f, 0.05f);

        b2FixtureDef dropletFixture;

        dropletFixture.shape = &dropletShape;
        dropletFixture.density = 1.0f;
        dropletFixture.friction = 0.3f;
        dropletFixture.restitution = 0.9f;
        droplet->CreateFixture(&dropletFixture);

        float32 random = (float(rand()) / float(RAND_MAX)) * 2 - 1;

        b2Vec2 force(random*10, 25.0f*random);

        droplet->ApplyForce(force, droplet->GetPosition(), true);
    }

    timer.setTimerType(Qt::PreciseTimer);
    connect(&timer, &QTimer::timeout, this, [this, world, dropletImage]
       {
            scene->clear();

            float timeStep = 1.0f/60.0f;

            int32 velocityIterations = 8;
            int32 positionIterations = 2;

            world->Step(timeStep, velocityIterations, positionIterations);

            auto nextBody = world->GetBodyList();

            while(nextBody)
            {
                auto position = nextBody->GetPosition();
                if(position.y > 0 && position.y < 150)
                {
                    QGraphicsPixmapItem* droplet = new QGraphicsPixmapItem(*dropletImage);
                    droplet->setPos(position.x + 400, position.y + 250);
                    scene->addItem(droplet);
                }
                nextBody = nextBody->GetNext();
            }
        });
    timer.setInterval(10);
    timer.start();
}

void MainWindow::populateBalloons()
{
    QPixmap * balloonImage1 = new QPixmap(QPixmap(balloonImagePath1).scaledToWidth(balloonWidth));
    QPixmap * balloonImage2 = new QPixmap(QPixmap(balloonImagePath2).scaledToWidth(balloonWidth));
    QPixmap * balloonImage3 = new QPixmap(QPixmap(balloonImagePath3).scaledToWidth(balloonWidth));
    QPixmap * balloonImage4 = new QPixmap(QPixmap(balloonImagePath4).scaledToWidth(balloonWidth));

    balloonImages.push_back(balloonImage1);
    balloonImages.push_back(balloonImage2);
    balloonImages.push_back(balloonImage3);
    balloonImages.push_back(balloonImage4);
}

void MainWindow::drawBalloons()
{
    ui->graphicsView->setVisible(true);
    scene = new QGraphicsScene(0, 0, screenWidth, screenHeight, this);
    ui->graphicsView->setStyleSheet("background:transparent;");

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFrameShape(QFrame::NoFrame);

    b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
    b2World* world = new b2World(gravity);

    QVector<int> balloonIndexes = populateBalloonWorld(world, gravity);

    timer.setTimerType(Qt::PreciseTimer);
    connect(&timer, &QTimer::timeout, this, [this, world, balloonIndexes]
            {
                this->balloonBox2DWorldStep(world, balloonIndexes);
            });
    timer.setInterval(timerIntervalMS);
    timer.start();
}

QVector<int> MainWindow::populateBalloonWorld(b2World* world, b2Vec2 gravity)
{
    QVector<int> balloonIndexes;
    for(int i= 0; i < totalBalloons; i++)
    {
        b2BodyDef balloon;
        balloon.type = b2_dynamicBody;
        int localX = ((double) (screenWidth/totalBalloons)) * i;
        balloon.position.Set(localX, bottomY);
        b2Body* balloonBody = world->CreateBody(&balloon);

        b2PolygonShape balloonShape;
        balloonShape.SetAsBox(1.0f, 1.5f);

        b2FixtureDef balloonFixture;

        balloonFixture.shape = &balloonShape;
        balloonFixture.density = 1.0f;
        balloonFixture.friction = 0.3f;
        balloonFixture.restitution = 0.9f;
        balloonBody->CreateFixture(&balloonFixture);

        float mass = balloonFixture.density * balloonSpeedUp;
        float upwardsForce = mass * gravity.y;
        b2Vec2 force(0, -upwardsForce);

        balloonBody->ApplyForce(force, balloonBody->GetPosition(), true);
        //random balloon image
        float32 random = (float(rand()) / float(RAND_MAX)) * 2 - 1;
        int randomInt = random * 10;
        int index = randomInt % balloonImages.size();

        if (index < 0)
            index *= -1;

        balloonIndexes.push_back(index);
    }
    return balloonIndexes;
}

void MainWindow::balloonBox2DWorldStep(b2World* world, QVector<int> balloonIndexes)
{
    scene->clear();

    float timeStep = 2.0f/60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 2;

    world->Step(timeStep, velocityIterations, positionIterations);

    auto nextBody = world->GetBodyList();

    int index = 0;
    while(nextBody)
    {
        auto position = nextBody->GetPosition();

        int balloonIndex = balloonIndexes[index];
        QPixmap * image = balloonImages[balloonIndex];

        QGraphicsPixmapItem* balloonItem = new QGraphicsPixmapItem(*image);
        float32 random = (float(rand()) / float(RAND_MAX)) * 2 - 1;
        balloonItem->setPos(position.x, position.y);
        scene->addItem(balloonItem);

        float xForce = (random * 20) * balloonSpeedUp;
        b2Vec2 force(xForce, -balloonSpeedUp * 10);
        nextBody->ApplyForce(force, nextBody->GetPosition(), true);
        nextBody = nextBody->GetNext();
        index++;
    }
}
