// $Id$
/**
 * @file cli.h
 * @brief Hlavičový soubor pro konzoli.
 *
 * Modul pro rozhraní zobrazitelné v konzoli.
 *
 * @author Jan Pavlica
 */
// $Log$
 

#ifndef CLI_H
#define CLI_H
#include<stdio.h>
#include<iostream>
#include"othello.h"

using namespace std;

/**
 * @brief      Třída pro obstarávání hry v konzolovém zobrazenení.
 */

class NewGame
{
public:

/// Nastavení vychozího stavu.

/**
   Metoda nastaví, zda-li je spuštěna nejaka dalsi hra.
 */
    void init();
/// Získání pamaetru pro novou hru.

/**
   Metoda zpracuje spouštěcí parametry pro novou hru.

   @return Vrací 0 pi úspěchu. V opačném případě vrací 1.
 */
    int getParams();
/// Spuštění nové hry.

/**
   Metoda spustí novou hru.
 */
    void startGame();
/// Vykreslovací funkce.

/**
   Metoda vykreslí hrací desku a vše potřebné k ní.
 */
    void drawGame();
/// Zpracování příkazů ve hře.

/**
   Obsluha příkazu load-game.
 */
    void loadGame();

/**
   Obsluha příkazu save-game.
 */
    void saveGame();

/**
   Metoda zpacovává příkazy ve hře.
 */
    void playGame();
/// Ukazatel na současnou hru.

    Othello *currentGame;
private:
/// Uložení velikosti hrací desky.
    int size;
/// Proměnná pro uložení umělé inteligence nebo lidského protivníka.
    othAi ai;
/// Proměnná pro zaznamenání již spuštěné hry.
    int status;
};

#endif // CLI_H
