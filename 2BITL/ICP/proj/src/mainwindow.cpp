/**
 * @file mainwindow.cpp
 * @brief Zdrojový soubor pro hlavní okno.
 *
 * Modul obstarává hlavní grafické okno.
 *
 * @author Jan Pavlica
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadgamemenu.h"
#include "newgamemenu.h"
#include "savegamemenu.h"
#include "form.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    status = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_Game_triggered()
{
    // ujisteni pri ukonceni hry
    QMessageBox::StandardButton response = QMessageBox::question(
        this,
        tr("Quit game."),
        tr("Are you sure, you want to quit game?"),
                QMessageBox::Yes|QMessageBox::No);

    if(response == QMessageBox::Yes)
    {
        QCoreApplication::quit();
    }

}

void MainWindow::on_actionNew_Game_triggered()
{
    if(status == 1)
    {
        // ujisteni pri zalozeni nove hry, pokud je aktivni jina
        QMessageBox::StandardButton response = QMessageBox::question(
            this,
            tr("New game."),
            tr("Are you sure, you want to start new game?"),
                    QMessageBox::Yes|QMessageBox::No);
        if(response == QMessageBox::Yes)
        {
            NewGameMenu * ngm = new NewGameMenu();
            ngm->myMainUI = this;
            ngm->show();
        }
    }
    else
    {
        NewGameMenu * ngm = new NewGameMenu();
        ngm->myMainUI = this;
        ngm->show();
    }
}

void MainWindow::on_actionLoad_Game_2_triggered()
{
    if(status == 1)
    {
        // ujisteni pri nacteni hry, pokud je aktivni jina
        QMessageBox::StandardButton response = QMessageBox::question(
            this,
            tr("Load game."),
            tr("Are you sure, you want to load game?"),
                    QMessageBox::Yes|QMessageBox::No);
        if(response == QMessageBox::Yes)
        {
            LoadGameMenu * lgm = new LoadGameMenu();
            lgm->myMainUI = this;
            lgm->show();
        }
    }
    else
    {
        LoadGameMenu * lgm = new LoadGameMenu();
        lgm->myMainUI = this;
        lgm->show();
    }

}


void MainWindow::setGameBoard(Ui::Form *game, int s)
{
    gameBoardUI = game;
    status = s;
}

void MainWindow::on_actionForward_triggered()
{
    //dopredu
    if(status == 1)
    {
        othRet ret = currentGame->redo();
        if (ret == othRet::REDO_LIMIT)
        {
         gameBoardUI->Msg->setText("You cannot go forward.");
        }
        else
        {
            gameBoardUI->Msg->setText("You have moved forward in history.");
        }
    }
    else
    {
        QMessageBox::warning(
            this,
            tr("Forward."),
            tr("You are not in the game.") );
    }
}

void MainWindow::on_actionBack_triggered()
{
    //zpet
    if(status == 1)
    {
       othRet ret = currentGame->undo();
       if (ret == othRet::UNDO_LIMIT)
       {
        gameBoardUI->Msg->setText("You cannot go back.");
       }
       else
       {
          gameBoardUI->Msg->setText("You have moved back in history.");
       }
    }
    else
    {
        QMessageBox::warning(
            this,
            tr("Back."),
            tr("You are not in the game.") );
    }

}

void MainWindow::on_actionSave_Game_triggered()
{
    if(status == 1)
    {
        SaveGameMenu * sgm = new SaveGameMenu();
        sgm->currentGame = currentGame;
        sgm->show();
    }
    else
    {
        QMessageBox::warning(
            this,
            tr("Save Game"),
            tr("Nothing to save.") );
    }
}
