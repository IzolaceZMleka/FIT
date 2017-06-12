// $Id$
/**
 * @file form.h
 * @brief Hlavičový soubor pro hrací desku.
 *
 * Modul slouží k práci s hrací deskou.
 *
 * @author Jan Pavlica
 */
// $Log$

#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include "othello.h"
#include <iostream>
#include <QString>
#include <QTime>
#include <QMessageBox>

class Form;

/**
 * @brief      Třída pro jednotlivé políčka na desce.
 */

class Cell : public QPushButton
{
    Q_OBJECT

public:
    explicit Cell(QWidget *parent = 0);
    ~Cell();
    /// Vrstva pro do které se budou ukládat buňky.
    QGridLayout *myLayout;
    /// Nastavení buňky.
    /**
       Metoda nastaví pozici buňce.

       @param[in] pos Pozice na desce.
     */
    void setPosition(int pos);
    /// Velikost hrací desky.
    int pomSize;
    /// Ukazazatel na hraci desku.
    Form *myBoard;
    /// Ukazatel na hru.
    Othello *gameInstance;
private slots:
    /// Slot při stisku myši.
    void getPosition();
private:
    /// Pozice na desce.
    int position;
};

namespace Ui {

class Form;
}

/**
 * @brief Třída pro obstaravání hrací desky.
 */

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    /// Vytvoření hrací desky.
    /**
       Metoda vytvoří hrací desku a vytvoří vše potřebné.

       @param[in] s Velikost desky.
     */
    void setUpDesk(int s);
    /// Nastavení velikosti hrací desky.
    /**
       Metoda nastaví získanou velikost desky.

       @param[in] s Velikost desky.
     */
    void setDeskSize(int s);
    /// Přiřazení odkazu na hru každé buňce.
    /**
       Metoda přiřadí jednotlivým buňkám zjištěný odkaz na vytvořenou hru.
     */
    void setGameInstanceForCells();
    /// Vykreslovací metoda.
    /**
       Metoda vykreslí všechny změny.
     */
    void reDrawDesk();
    /// Zpoždění tahu.
    /**
       Metoda zpoždí tah při tahu AI k zachování přirozenosti.
       @param[in] millisecondsToWait Počet milisekund o které se zpozdí tah.
     */
    void delay( int millisecondsToWait );
    /// Vrstva pro do které se budou ukládat buňky.
    QGridLayout *myLayout;
    /// Ukazatel na hru.
    Othello *gameInstance = NULL;
    /// Ukazatel na UI.
    Ui::Form *ui;
    /// Proměnná pro záznam ai.
    /**
       Nutné pro zpoždění, aby AI tahy vypadaly přirozeněji.
     */
    bool ai;
private:
    /// Velikost hrací desky.
    int size;
    /// Seznam buněk.
    QList<Cell*> cellsList;
    /// Seznam popisků hrací desky.
    QList<QLabel*> desList;
    /// Plonkovní tlačítko pro zachování velikosti desky.
    Cell *plus;
};

#endif // FORM_H
