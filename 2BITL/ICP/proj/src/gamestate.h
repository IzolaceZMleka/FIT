#ifndef GAMESTATE_H
#define GAMESTATE_H

/**
 * @file   gamestate.h
 * @Author Adam Piecek
 * @date   May, 9
 * @brief  objekt informující o stavu hry
 *
 */

#include "othello_types.h"


/**
 * @brief       zaznaménává historii, kdo hrál
 * @details		komunikacni objekt, pres nej se zjistuje stav aktualne hrane hry
 */
class GameState{
    /// score prvního a druhého hráče
    int score[2] = {0, 0};
    /// player1 = True, player2 = False;
    bool turn = true;
    /// ze šachovnice se čte např board[0][44]
    oboardPtr board;

    /// právo zápisu má pouze GameStateManipulator
protected:
    void set_score(int index, int value) { score[index] = value;}
    void set_turn(bool t) 				 { turn = t;}
    void set_board(oboardPtr b) 		 { board = b;}
    ~GameState(){}

    /// pouze čtení
public:
    const int* get_score()		{ return score;}
    bool get_turn() 			{ return turn;}
    othBoardPtr get_board()		{ return board;}
    const allInOne get_allInOne() {
        allInOne aio;
        aio.board = board;
        aio.player1Score = score[0];
        aio.player2Score = score[1];
        aio.turn = turn;
        return aio;
    }
};

#endif // GAMESTATE_H
