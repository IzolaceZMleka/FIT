#ifndef MESSENGER_H
#define MESSENGER_H

/**
 * @file   messenger.h
 * @Author Adam Piecek
 * @date   May, 9
 * @brief  komunikační objekt
 *
 */

#include "othello_types.h"

typedef std::vector<std::slice> 			slicesVect;
typedef std::vector<std::tuple<int, int>> 	scoreVect;
typedef std::vector<oboardPtr> 				oboardPtrVect;
typedef std::vector<bool>					turnVect;
typedef std::vector<int>					greyVect;

struct Coords_system{
    int x;
    int y;
    size_t size;
    int index;
    othAi ai;
};

namespace StateType{
    extern std::bitset<2> EMPTY;
    extern std::bitset<2> WHITE;
    extern std::bitset<2> BLACK;
    extern std::bitset<2> GREY;
}

/*
class To_historyObjs{
    virtual void uploadHistory() = 0;
    virtual void downloadHistory() = 0;
};
*/

class GameStateManipulator;
class Board_History;
class Move_switcher;
class Turn_History;
class Flipper;
class Grey_Manager;
class Score_History;
class Scout_flip;
class Scout_grey;
class Scout;
class IO_Game;

class AI_Othello_Intrfc;


/**
 * @brief       stará se o komunikaci mezi objekty
 */
class Messenger{
private: // nesmi mit pristup kvuli Messenger_ForAI
    GameStateManipulator* gsm;
    callDraw drawCallback;
    Coords_system* cs;

protected:

    Move_switcher* ms;
    Turn_History* th;
    Flipper* fl;
    Scout_flip* scf;
    Scout_grey* scg;
    Grey_Manager* gm;
    Score_History* sh;
    Board_History* bh;
    IO_Game* iog;

    AI_Othello_Intrfc* aiOi;

public:

    Messenger(Coords_system* cs): cs(cs){}

    /// Messenger
    void init(callDraw cd, GameStateManipulator* gsm, Move_switcher* ms, Turn_History* th, Flipper* fl,
              Scout_flip* scf, Scout_grey* scg, Grey_Manager* gm, Score_History* sh, Board_History* bh,
              AI_Othello_Intrfc* aiOi, IO_Game* iog);
    void actualize_gameState();
    othRet actualize_coords(int x, int y);
    void draw() { drawCallback();}
    void uploadHistory();
    void downloadHistory();
    void load_newPtr(Grey_Manager* gmp, AI_Othello_Intrfc* aiOip){ gm = gmp; aiOi = aiOip;}

    /// IO_Game
    void save_game(std::string name);

    /// GameStateManipulator
    void change_score(int index, int value);
    void change_turn(bool player);
    void change_board(oboardPtr b);

    /// Move_switcher
    othRet play();
    void putToken(int index, oboardPtr b, std::bitset<2> tokenColor);
    int main_ai();
    void actualizeSwitcher();

    /// Turn History
    virtual void setTurnH(bool player);
    virtual void undoTurnH();
    virtual void redoTurnH();
    virtual bool getActualTurn();

    /// Scout
    bool main_scout(Scout* ptr, int index, oboardPtr b, std::bitset<2> bits);
    slicesVect get_slices();
    void clear_slicesVect();

    /// Flipper
    void flip_tokens(int index, oboardPtr b, std::bitset<2> playerColor);

    /// Grey Manager
    bool checkIf_grey(int index);
    othRet grey_walk(oboardPtr b, std::bitset<2> playerColor);
    greyVect get_greyIndexes();

    /// Score History
    virtual void setScoreH(std::tuple<int, int> plScore);
    virtual void undoScoreH();
    virtual void redoScoreH();

    /// Board_history
    virtual oboardPtr initBoard();
    virtual oboardPtr new_board();
    virtual oboardPtr get_board(int get);
    virtual oboardPtr get_board();
    virtual othRet undo();
    virtual othRet redo();

};


/*
// Messenger_ForAI, urceno pro AI_3 viz artificial_intelligence.h

class Messenger_ForAI: public Messenger{
private: // zapouzdreni AI - nema pravo manipulovat s GameState, ci vykreslovat
    using Messenger::actualize_gameState;
    using Messenger::draw;
    using Messenger::uploadHistory;
    using Messenger::change_score;
    using Messenger::change_turn;
    using Messenger::change_board;

    Turn_History* thai;
    Score_History* shai;
    Board_History* bhai;
public:
    Messenger_ForAI(Coords_system* cs): Messenger(cs){}
    void initai(Move_switcher* ms, Turn_History* th, Turn_History* thai, Flipper* fl, Scout_flip* scf,
              Scout_grey* scg, Grey_Manager* gm, Score_History* sh,
              Score_History* shai, Board_History* bh, Board_History* bhai);
    void downloadHistory() override;

    // Board_history
    virtual oboardPtr initBoard();
    virtual oboardPtr new_board();
    virtual oboardPtr get_board(int get);
    virtual oboardPtr get_board();
    virtual othRet undo();
    virtual othRet redo();

    // Score History
    virtual void setScoreH(std::tuple<int, int> plScore);
    virtual void undoScoreH();
    virtual void redoScoreH();

    // Turn History
    virtual void setTurnH(bool player);
    virtual void undoTurnH();
    virtual void redoTurnH();
};
*/

#endif // MESSENGER_H
