#ifndef BUILDER_H
#define BUILDER_H

/**
 * @file   builder.h
 * @Author Adam Piecek
 * @date   May, 9
 * @brief  volání konstruktorů
 *
 */

#include "core.h"
#include "artificial_intelligence.h"
#include "io.h"


class Builder{
    size_t size;
    othAi ai;
    /// callback funkce pro vykreslení
    callDraw drawCallback;
    struct Coords_system cs;

    /// ukazatele na všechny objekty
    std::unique_ptr<Messenger> msg;
    std::unique_ptr<GameStateManipulator> gsm;
    std::unique_ptr<Move_switcher> ms;
    std::unique_ptr<Turn_History> th;
    std::unique_ptr<Scout_flip> scf;
    std::unique_ptr<Scout_grey> scg;
    std::unique_ptr<Flipper> fl;
    std::unique_ptr<Board_History> bh;
    std::unique_ptr<Score_History> sh;
    std::unique_ptr<Grey_Manager> gm;
    std::unique_ptr<IO_Game> iog;

    std::unique_ptr<Grey_Manager_ForAI> gmai;

    /*
     * pro AI_3
    struct Coords_system csai;
    std::unique_ptr<Messenger_ForAI> msgai;
    std::unique_ptr<Move_switcher> msai;
    std::unique_ptr<Turn_History> thai;
    std::unique_ptr<Score_History> shai;
    std::unique_ptr<Board_History> bhai;
    */

    //
    // Doplneni AI
    //

    // AI
    std::unique_ptr<AI_1> ai1;
    std::unique_ptr<AI_2> ai2;

    /**
     * @brief      rozhodování a sestavení AI objektů
    */
    void build_AI();
public:
    //set(string n, size_t s, bool a): name(n), size(s), ai(a) { build();}
    Builder(size_t s, othAi a, callDraw cd): size(s), ai(a), drawCallback(cd) {}
    /**
     * @brief      volání všech potřebných konstruktorů
    */
    void build();

    /**
     * @return     ukazatel na aktuální stav hry
    */
    GameState* get_GameState()		{ return static_cast<GameStateManipulator*>(gsm.get());}

    /**
     * @return     ukazatel na messengera
    */
    Messenger* get_messenger()		{ return msg.get();}

    /**
     * @brief      obsluha load game
     * @details	   dosud používané nastavení AI dealokuje a zavolá potřebné konstruktory pro nahrávanou hru
    */
    int loadgame(std::string name);
    ~Builder(){}
};

#endif // BUILDER_H
