/**
 * @file savegamemenu.h
 * @brief Hlavičový soubor pro menu zobrazující se při výběru spuštění nové hry.
 *
 *
 * @author Jan Pavlica
 */

#ifndef SAVEGAMEMENU_H
#define SAVEGAMEMENU_H

#include <QDialog>
#include <othello.h>

namespace Ui {
class SaveGameMenu;
}

/**
 * @brief      Třída pro Save Menu
 */


class SaveGameMenu : public QDialog
{
    Q_OBJECT

public:
    explicit SaveGameMenu(QWidget *parent = 0);
    ~SaveGameMenu();
    /// Ukazatel na hru.
    Othello *currentGame;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SaveGameMenu *ui;
};

#endif // SAVEGAMEMENU_H
