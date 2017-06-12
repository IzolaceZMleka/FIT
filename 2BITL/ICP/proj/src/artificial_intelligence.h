#ifndef ARTIFICIAL_INTELLIGENCE_H
#define ARTIFICIAL_INTELLIGENCE_H

/**
 * @file   artificial_intelligence.h
 * @Author Adam Piecek
 * @date   May, 9
 * @brief  Interface a umělá inteligence
 *
 * Skládá se z AI_1 a AI_2.
 */

#include "messenger.h"
#include "gamestate.h"

/**
 * @brief      Interface pro třídy umělé inteligence
 */
class AI_Othello_Intrfc{
protected:
    /// ukazatel na messenger
    Messenger* msg;
    /// ukazatel na aktualni stav hry
    GameState* gs;
public:
    AI_Othello_Intrfc(Messenger* msgPtr, GameState* gs): msg(msgPtr), gs(gs){}
    /**
     * @brief	Hlavní tělo algoritmu umělé inteligence
     */
    virtual int main_ai() = 0;
    virtual ~AI_Othello_Intrfc() = 0;
};

/**
 * @brief		Interface pro třídu AI_3 umělé inteligence
 * @details		AI_1 a AI_2 tuto třídu nepoužívají, jen pouze potenciální pro AI_3
 *
 */
class AI_Othello: public AI_Othello_Intrfc{
public:
    AI_Othello(Messenger* msg, GameState* gs): AI_Othello_Intrfc(msg, gs){}
    othRet putToken(int x, int y);
    othRet undo();
    othRet redo();
    void downloadHistory(){ msg->downloadHistory();}
    virtual ~AI_Othello() = 0;
};

/**
 * @brief		Třída AI_1
 * @details		jednoduchá AI, volí náhodný tah z šedých míst
 */
class AI_1: public AI_Othello_Intrfc{
    Scout* sp;
public:
    AI_1(Messenger* msgPtr, GameState* gs, Scout* sp): AI_Othello_Intrfc(msgPtr, gs), sp(sp){}
    int main_ai() override; // vrátí index, kam polozil kamen
    ~AI_1();
};

/**
 * @brief		Třída AI_2
 * @details		jednoduchá AI, vybere ze šedých míst to, které nabízí nejvyšší skore
 */
class AI_2: public AI_Othello_Intrfc{
    Scout* sp;
public:
    AI_2(Messenger* msgPtr, GameState* gs, Scout* sp): AI_Othello_Intrfc(msgPtr, gs), sp(sp){}
    int main_ai() override; // vrátí index, kam polozil kamen
    ~AI_2();
};

/*
// Pro sofistikovanejsi AI, ktere prohledava stavovy prostor. Pouziva svuj vlastni upraveny Messenger_ForAI, ktery
    umoznuje volat vsechny objekty v core.h, krome metod k upravovani GameState a vykreslovacich metod.
// Objekty se suffixem "History" se vytvori jeste jednou a tyto objekty Messenger_ForAI vola, nikoli herni, takze
    nehrozi nebezpeci prepsani hry pri prohledavani stavoveho prostoru.
// Nutne na zacatku funkce main volat downloadHistory, ktera aktualizuje historii ve vlastnich
    AI History objektu.
// AI_Othello_Intrfc je podobne tride Othello v othello.h. K ziskani informaci o aktualnim stavu hry je urcen
    ukazatel na objekt GameState* gs. Pro ziskani informaci, na jakych mistech jsou seda policka, je urcena v
    v Messenger_ForAI metoda get_greyIndexes();
class AI_3: public AI_Othello{
public:
    AI_3(Messenger* msgPtr, GameState* gs): AI_Othello(msgPtr, gs){}
    int main_ai() override; // vrati index, kam polozil kamen
    ~AI_3();
};
*/



#endif // ARTIFICIAL_INTELLIGENCE_H
