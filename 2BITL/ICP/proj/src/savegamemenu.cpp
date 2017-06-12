/**
 * @file savegamemenu.cpp
 * @brief Zdrojový soubor pro menu zobrazující se při výběru spuštění nové hry.
 *
 *
 * @author Jan Pavlica
 */

#include "savegamemenu.h"
#include "ui_savegamemenu.h"
#include "iostream"

SaveGameMenu::SaveGameMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveGameMenu)
{
    ui->setupUi(this);
}

SaveGameMenu::~SaveGameMenu()
{
    delete ui;
}

void SaveGameMenu::on_pushButton_clicked()
{
    SaveGameMenu::close();
}

void SaveGameMenu::on_pushButton_2_clicked()
{
    QString saveName = ui->lineEdit->text(); //zjisteni nazvu hry
    currentGame->saveGame(saveName.toStdString()); // ulozeni nazvu hry

    SaveGameMenu::close();
}
