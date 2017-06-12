/**
 * @file newgamemenu.cpp
 * @brief Zdrojový soubor pro menu zobrazující se při výběru spuštění nové hry.
 *
 *
 * @author Jan Pavlica
 */

#include "newgamemenu.h"
#include "ui_newgamemenu.h"
#include <QTextStream>
#include "othello.h"
#include <functional>
GameState* gs;

NewGameMenu::NewGameMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewGameMenu)
{
    // comboBox pro velikost hraci desky
    ui->setupUi(this);
    ui->comboBox->addItem("6", QVariant(6));
    ui->comboBox->addItem("8", QVariant(8));
    ui->comboBox->addItem("10", QVariant(10));
    ui->comboBox->addItem("12", QVariant(12));
    ui->comboBox->setCurrentIndex(1);
}

NewGameMenu::~NewGameMenu()
{
    delete ui;
    delete desk;
}

void NewGameMenu::on_buttonBox_accepted() // akce pri kliknuti na ok
{
    if(myMainUI->status == 1)
    {
        myMainUI->currentGame->quitGame(); // pokud bezi hra, prvne ji zrusim
    }
    else
    {
        myMainUI->ui->label->hide(); // schovam popis hry
    }
    int size = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();
    int enemy = ui->comboBox_2->currentIndex();
    desk = new Form();
    desk->setDeskSize(size);
    desk->setUpDesk(size); // nastavim hraci desku
    myMainUI->setCentralWidget(desk);
    myMainUI->setGameBoard(desk->ui,1);
    myMainUI->setStyleSheet("MainWindow{background-image: url(:/background.jpg);}"); // nastavim pozadi
    NewGameMenu::close(); // zavru menu pro novou hru
    callDraw drawFunc = std::bind(&Form::reDrawDesk, desk); // vykreslovaci funkce
    // spusteni hry
    if(enemy == 0)
    {
        Othello* o = new Othello(size, othAi::noneAi, drawFunc, gs);
        desk->gameInstance = o;
    }
    else if(enemy == 1)
    {
        Othello* o = new Othello(size, othAi::firstAi, drawFunc, gs);
        desk->gameInstance = o;
        desk->ai = true;
    }
    else if(enemy == 2)
    {
        Othello* o = new Othello(size, othAi::secondAi, drawFunc, gs);
        desk->gameInstance = o;
        desk->ai = true;
    }
    myMainUI->currentGame = desk->gameInstance;
    desk->setGameInstanceForCells();
    desk->reDrawDesk();
}

void NewGameMenu::on_buttonBox_rejected()
{
    // pri odmitnuti zavru
    NewGameMenu::close();
    delete this;
}
