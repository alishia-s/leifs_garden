#include "helpmenu.h"
#include "ui_helpmenu.h"

HelpMenu::HelpMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("Help");
    this->setWindowIcon(QIcon(":/images/Images/questionMarkIcon.png"));

    QPixmap temp = QPixmap(":/images/Images/helpMenuImage.png.png");
    temp = (temp.scaledToWidth(700));
    pixmap = new QPixmap(temp);
    ui->imageHolder->setPixmap(*pixmap);
}

HelpMenu::~HelpMenu()
{
    delete ui;
    delete pixmap;
}
