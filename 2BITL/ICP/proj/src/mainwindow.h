/**
 * @file mainwindow.h
 * @brief Hlavičový soubor pro hlavní okno.
 *
 * Modul obstarává hlavní grafické okno.
 *
 * @author Jan Pavlica
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <form.h>
#include "ui_form.h"
#include "QMessageBox"

namespace Ui {
class MainWindow;
}

/**
 * @brief      Třída hlavního okna.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /// Ukazatel na své vlastní grafické rozhraní.
    Ui::MainWindow *ui;
    /// Nastavení informací o vytvořené hře.
    /**
       Metoda nastaví patřičný status a také nastaví ukazatel na hrací desku.

       @param[in] *gameBoard Ukazatel na desu.
       @param[in] status Parametra pro nastavení statusu.
     */
    void setGameBoard(Ui::Form *gameBoard, int status);
    /// Ukazatel na hru.
    Othello *currentGame;
    /// Ukazatel na grafické rozhraní hrací desky.
    Ui::Form *gameBoardUI;
    /// Status zaznamenávající, zda-li se jedná o první hru.
    int status;
private slots:
    /// Slot při zmáčknutí tlačtka Exit Game.
    void on_actionExit_Game_triggered();
    /// Slot při zmáčknutí tlačtka New Game.
    void on_actionNew_Game_triggered();
    /// Slot při zmáčknutí tlačtka Load Game.
    void on_actionLoad_Game_2_triggered();
    /// Slot při zmáčknutí tlačtka Forward.
    void on_actionForward_triggered();
    /// Slot při zmáčknutí tlačtka Back.
    void on_actionBack_triggered();
    /// Slot při zmáčknutí tlačtka Save Game.
    void on_actionSave_Game_triggered();
};


#endif // MAINWINDOW_H
