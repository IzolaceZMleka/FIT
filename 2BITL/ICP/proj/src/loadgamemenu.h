// $Id$
/**
 * @file loadgamemu.h
 * @brief Hlavičový soubor pro menu zobrazující se při výběru načtení uložené hry.
 *
 *
 * @author Jan Pavlica
 */
// $Log$

#ifndef LOADGAMEMENU_H
#define LOADGAMEMENU_H

#include <QWidget>
#include <iostream>
#include <algorithm>
#include <vector>
#include <othello.h>
#include <mainwindow.h>
#include "ui_mainwindow.h"

namespace Ui {
class LoadGameMenu;
}

/**
 * @brief      Třída pro Load Menu
 */

class LoadGameMenu : public QWidget
{
    Q_OBJECT

public:
    explicit LoadGameMenu(QWidget *parent = 0);
    ~LoadGameMenu();
    /// Ukazatel na hru.
    Othello *currentGame;
    MainWindow *myMainUI;

private slots:
    /// Slot pro potvrzení načtení hry.
    void on_buttonBox_accepted();
    /// Slot při zrušení načtení hry
    void on_buttonBox_rejected();

private:
    Ui::LoadGameMenu *ui;
    Form* desk = NULL;
};

#endif // LOADGAMEMENU_H
