#include "model.h"

#include <QDebug>

Model::Model(QObject *parent):
    QObject(parent)
{
    currentDay = 1;
    leifDialogue = getDialogueFromFile(":/dialogue/Dialogue/intro.txt");
    funFactsDialogue = getFunFactsFromFile(":/dialogue/Dialogue/funFacts.txt");
    usedFunFacts = new QSet<int>();
    plantTemperature = 70;
    plantPHValue = 6.5;
}

void Model::progressNextDay()
{
    QVector<QString> missedTasks = currentPlant->checkStage(currentPlant->getPlantStage());

    //if there are missed tasks, send the message to the user and end the game
    if (missedTasks.size() !=0)
    {
        leifDialogue->enqueue("UH OH! You should have taken better care of your plant. It died because:");
        for (QString task : missedTasks)
        {
            if (task == "overwatered")
            {
                leifDialogue->enqueue("You overwatered your plant.");
                continue;
            }
            if (task == "wrongTemperature")
            {
                if (currentPlant ->getTemp() > currentPlant->getPreferredHighTemp())
                {
                    leifDialogue->enqueue("Your room was too hot! It was at " + QString::number(currentPlant->getTemp()) + "degrees.");
                }
                if (currentPlant ->getTemp() < currentPlant->getPreferredLowTemp())
                {
                    leifDialogue->enqueue("Your room was too cold! It was at " + QString::number(currentPlant->getTemp()) + "degrees.");
                }
                continue;
            }
            if (task == "hasBugs")
            {
                leifDialogue->enqueue("The bugs ate your plant :(.");
                continue;
            }
            if (task == "overfertilized")
            {
                leifDialogue->enqueue("You added too much fertilizer.");
                continue;
            }
            if (task == "wrongPH")
            {
                leifDialogue->enqueue("Your plant's soil was at the wrong PH level. It was at " + QString::number(currentPlant->getPH()));
                continue;
            }
            if (task == "notFertilized")
            {
                leifDialogue->enqueue("You did not fertilize your plant.");
                continue;
            }
            if (task == "notWatered")
            {
                leifDialogue->enqueue("You did not give your plant enough water!");
                continue;
            }
        }
        leifDialogue->enqueue("Want to start again?");
        emit plantDied();
    }

    //no missed tasks, then the user can continue playing game
    else if (missedTasks.size()==0 && currentPlant -> getPlantStage() < totalGrowingStages)
    {
        addRandomEvents();
        bool funFactFound = false;
        leifDialogue->enqueue("It's learning time!");
        while(!funFactFound || usedFunFacts->size() == funFactsDialogue->size())
        {
            int count = 0;
            int random = QRandomGenerator::global()->bounded(0, funFactsDialogue->size());
            count++;
            if(!usedFunFacts->contains(random))
            {
                leifDialogue->enqueue(funFactsDialogue->at(random));
                funFactFound = true;
                usedFunFacts->insert(random);
            }
            if(count > 5)
            {
                break;
            }
        }
        return;
    }

    // No missed tasks and plant is done growing, end the game
    else if (missedTasks.size()==0 && currentPlant->getPlantStage() == totalGrowingStages)
    {
        leifDialogue->enqueue("Your plant is all grown! Thanks for playing!");
        emit endGame();
        return;
    }
}

void Model::addRandomEvents()
{
    int random = QRandomGenerator::global()->bounded(0,3);
    event e;

    leifDialogue->enqueue("Today is day " + QString::number(currentDay)+"!");

    //plant has bugs
    if (random == 0)
    {
        e = bugs;
        currentPlant->setHasBugs();
        leifDialogue->enqueue("I snuck out to meet my sneaky leek through your window! Now there are bugs on your plant.");
    }
    //plants ph has changed
    else if (random == 1)
    {
        e = phChanged;
        plantPHValue += QRandomGenerator::global()->bounded(-4, 4);
        currentPlant->setPHLevel(plantPHValue);
        leifDialogue->enqueue("The PH of your plant has suddenly changed! Your soil's pH is now " + QString::number(currentPlant ->getPH()));
    }
    // Environment temperature has changed
    else
    {
        e = temperatureChanged;
        plantTemperature += QRandomGenerator::global()->bounded(-20,20);
        currentPlant->changeTemp(plantTemperature);
        leifDialogue->enqueue("You forgot to pay your power bill, so your room temperature changed!");
        emit sendCurrentTemp(plantTemperature);
    }

    emit randomEvent(e);
    currentPlant->updateStage();
    QTimer::singleShot(300, this, [this](){emit displayPlant(currentPlant->getPlantImageName());});
    QTimer::singleShot(1500, this, [this](){emit startNextDay();});
    QTimer::singleShot(6000, this, [this](){emit showNextDayButton();});
}

Model::~Model()
{
    delete currentPlant;
    delete usedFunFacts;
    delete funFactsDialogue;
    delete notebookPages;
}

QQueue<QString>* Model::getDialogueFromFile(QString filePath)
{
    QQueue<QString>* newDialogue = new QQueue<QString>();

    //Read dialogue from file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open file";
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        newDialogue->enqueue(QString(in.readLine()));
    }

    file.close();

    return newDialogue;
}

QVector<QString>* Model::getFunFactsFromFile(QString filepath)
{
    QVector<QString>* newDialogue = new QVector<QString>();

    // Read dialogue from file
    QFile file (filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open file";
    }

    QTextStream in(&file);
    while(!in.atEnd())
    {
        newDialogue->append(QString(in.readLine()));
    }

    file.close();

    return newDialogue;
}

void Model::sendDialogue()
{
    emit sendDisplayDialogue(leifDialogue->dequeue());
}

void Model::progressDialogue()
{
    if (leifDialogue->isEmpty())
    {
        emit endDialogue();
    }
    else
    {
        QString nextLine = leifDialogue->head();
        if (nextLine.startsWith("\\"))
        {
            emit endDialogue();
            QString command = nextLine.removeFirst();
            if (command == "plantSelection")
            {
                emit showPlantSelection(true);
                leifDialogue->dequeue();
            }
        }
        else
        {
            emit sendDisplayDialogue(leifDialogue->dequeue());
        }
    }
}

void Model::sendNotebookInstructions()
{
    emit handleNotebookNextButton(currentPageIndex + 1 < notebookPages->length());
    emit handleNotebookBackButton(currentPageIndex - 1 >= 0);
    emit sendDisplayInstructions(currentPage);
}

void Model::notebookNextPage()
{
    currentPageIndex++;
    currentPage = notebookPages->at(currentPageIndex);

    sendNotebookInstructions();
}

void Model::notebookPreviousPage()
{
    currentPageIndex--;
    currentPage = notebookPages->at(currentPageIndex);

    sendNotebookInstructions();
}

void Model::plantSelected()
{
    QString selection = QString(sender() -> objectName());
    int numberOfPages = 0;

    if(selection == "succulent")
    {
        Succulent* s = new Succulent;
        numberOfPages = 5;
        currentPlant = s;
    }
    if (selection == "snakePlant")
    {
        SnakePlant* s = new SnakePlant;
        numberOfPages = 5;
        currentPlant = s;
    }
    if (selection == "tomato")
    {
        Tomato* t = new Tomato;
        numberOfPages = 4;
        currentPlant = t;
    }
    currentPlant->changeTemp(plantTemperature);
    currentPlant->setPHLevel(plantPHValue);

    QString filePath = currentPlant->getInstructionsPath();
    notebookPages = new QVector<QString>();
    for(int i = 0; i < numberOfPages; i++)
    {
        notebookPages->append(filePath + QString::number(i) + ".png");
    }
    currentPage = notebookPages->at(0);

    emit showPlantSelection(false);
    emit displayPlant(currentPlant->getPlantImageName());
    emit sendCurrentTemp(plantTemperature);
    emit showNextDayButton();
    emit enableAllButtons();
    sendNotebookInstructions();
}

void Model::updateCurrentDay()
{
    currentDay++;
}

void Model::processTaskDone(task taskPerformed)
{
    if (taskPerformed == wateringCan)
    {
        currentPlant->addWater();
    }
    if (taskPerformed == fertilizer)
    {
        currentPlant -> fertilize();
    }
    if (taskPerformed == thermostat)
    {
        emit sendCurrentTemp(currentPlant->getTemp());
    }
    if (taskPerformed == sulfur)
    {
        plantPHValue -= 1;
        currentPlant->setPHLevel(plantPHValue);
    }
    if (taskPerformed == phMeter)
    {
        emit sendCurrentPH(QString::number(currentPlant->getPH()));
    }
    if (taskPerformed == limestone)
    {
        plantPHValue += 1;
        currentPlant->setPHLevel(plantPHValue);
    }
    if (taskPerformed == pesticide)
    {
        currentPlant -> addPestilient();
    }
}

void Model::changeTemp(int temp)
{
    currentPlant -> changeTemp(temp);
    plantTemperature = temp;
}
