/*
 Assignment 9 - Educational App - Leif's Garden

 Team: Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac
 Date:      11/29/2023
 Course:    CS 3505, University of Utah, School of Computing
 Copyright: CS 3505 and Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac - This work may not be copied for use in Academic Coursework.

 We, Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac, certify that we wrote this code from scratch and
 did not copy it in part or whole from another source. All references used in the completion of the assignments are cited in my README file.

 File Contents

    This file is used to create the UI for the help menu.
*/

#ifndef HELPMENU_H
#define HELPMENU_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class HelpMenu;
}
/// \brief The HelpMenu class - creates the help menu UI
class HelpMenu : public QWidget
{
    Q_OBJECT

    QPixmap * pixmap;

public:
    explicit HelpMenu(QWidget *parent = nullptr);
    ~HelpMenu();

private:
    Ui::HelpMenu *ui;
};
#endif // HELPMENU_H
