/**
 * @file newgamemenu.h
 * @brief Hlavičový soubor pro menu zobrazující se při výběru spuštění nové hry.
 *
 *
 * @author Jan Pavlica
 */

#ifndef NEWGAMEMENU_H
#define NEWGAMEMENU_H

#include <QWidget>
#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <form.h>

namespace Ui {
class NewGameMenu;
}

/**
 * @brief      Třída pro New Game Menu
 */


class NewGameMenu : public QWidget
{
    Q_OBJECT

public:
    explicit NewGameMenu(QWidget *parent = 0);
    ~NewGameMenu();
    MainWindow *myMainUI;

private slots:
    /// Slot pro potvrzení nové hry.
    void on_buttonBox_accepted();
    /// Slot při zrušení nové hry
    void on_buttonBox_rejected();

private:
    Ui::NewGameMenu *ui;
    Form* desk;
};

#endif // NEWGAMEMENU_H

