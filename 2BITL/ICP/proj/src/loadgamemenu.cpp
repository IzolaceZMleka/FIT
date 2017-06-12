/**
 * @file loadgamemu.cpp
 * @brief Zdrojový soubor pro menu zobrazující se při výběru načtení uložené hry.
 *
 *
 * @author Jan Pavlica
 */

#include "loadgamemenu.h"
#include "ui_loadgamemenu.h"
#include <string>
#include <vector>
#include <iostream>
#include <QTextStream>
using namespace std;



LoadGameMenu::LoadGameMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadGameMenu)
{
    ui->setupUi(this);
    vector<string> save = chooseGame();

    for(vector<string>::iterator it = save.begin(); it != save.end(); ++it){
        ui->comboBox->addItem(QString::fromStdString(*it));
    }
    ui->comboBox->setCurrentIndex(0);
}

LoadGameMenu::~LoadGameMenu()
{
    delete ui;
    delete desk;
}

void LoadGameMenu::on_buttonBox_accepted()
{
    if(myMainUI->status == 1)
    {
        myMainUI->currentGame->quitGame();
    }
    else
    {
        myMainUI->ui->label->hide();
    }

    delete desk;
    desk = new Form();
    callDraw drawFunc = std::bind(&Form::reDrawDesk, desk);

    desk->gameInstance = new Othello(12, othAi::noneAi, drawFunc, gs);

    //  QString gameName = ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
    QString gameName = ui->comboBox->currentText();

    int mySize = desk->gameInstance->loadGame(gameName.toStdString());

    //std::cout << mySize;
    //std::cout << "DE";

    desk->setDeskSize(mySize);
    desk->setUpDesk(mySize);
    myMainUI->setCentralWidget(desk);
    myMainUI->setGameBoard(desk->ui,1);
    myMainUI->setStyleSheet("MainWindow{background-image: url(:/background.jpg);}");

    myMainUI->currentGame = desk->gameInstance;
    desk->setGameInstanceForCells();

    desk->reDrawDesk();

    LoadGameMenu::close();
}

void LoadGameMenu::on_buttonBox_rejected()
{
    LoadGameMenu::close();
}
