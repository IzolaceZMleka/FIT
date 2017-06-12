#ifndef OTHELLO_H
#define OTHELLO_H

/**
 * @file   othello.h
 * @Author Adam Piecek
 * @date   May, 9
 * @brief  interface Othella
 *
 */

#include <string>

#include "gamestate.h"
#include "othello_types.h"

extern GameState* gs;

class Messenger;
class Builder;

std::vector<std::string> chooseGame();

/// pro prehlednost vypsane rozhranni
class OthelloInterface{
public:
    virtual othRet putToken(int x, int y) = 0;
    virtual othRet undo() = 0;
    virtual othRet redo() = 0;
    virtual void newGame() = 0;
    virtual int loadGame(std::string name) = 0;
    virtual void saveGame(std::string name) = 0;
    virtual void quitGame() = 0;
    virtual ~OthelloInterface(){}
};

/**
 * @brief       vstupní manipulace s hrou Othello
 */
class Othello: public OthelloInterface {
    Messenger* msgPtr;
    Builder* buildPtr;
    othAi ai;
public:
    /**
     * @brief       vstupní manipulace s hrou Othello
     * @param		s	velikost hrací desky - 6, 8, 10, 12
     * 				a	typ umělé inteligence
     * 				c	uživatelova funkce na vykreslení
     * 				gsPtr	ukazatel, který po kontrukci bude ukazovat na GameState
     */
    Othello(size_t s, othAi a, callDraw c, GameState* &gsPtr);
    othRet putToken(int x, int y) override;
    othRet undo() override;
    othRet redo() override;
    void newGame() override{}
    int loadGame(std::string name) override;
    void saveGame(std::string name) override;
    void quitGame() override;
    ~Othello();
};

#endif // OTHELLO_H
