#ifndef TYPEDEFS_H
#define TYPEDEFS_H

/**
 * @file   othello_types.h
 * @Author Adam Piecek
 * @date   May, 9
 * @brief  datové typy pro práci s Othellem, i v něm
 *
 */

#include <valarray>
#include <bitset>
#include <functional>

/// ukazatel na desku, vraci funkce GameState::get_board()
typedef const std::valarray<std::bitset<2>>* othBoardPtr;

/// vykreslovaci callback funkce
typedef std::function<void()> callDraw;

namespace BitStateType{
    const std::bitset<2> EMPTY (0x0);
    const std::bitset<2> WHITE (0x1);
    const std::bitset<2> BLACK (0x2);
    const std::bitset<2> GREY  (0x3);
}

typedef struct{
    int player1Score;
    int player2Score;
    bool turn;
    othBoardPtr board;
}allInOne;


enum class othRet{
    /// validní tah
    VALID_TURN,			// validni tah
    /// nevalidní tah
    INVALID_TURN,		// nevalidni tah
    /// hráči už nemají kam táhnout
    GAME_OVER,			// hraci uz nemaji kam tahnout
    /// neočekávaná chyba
    INTERNAL_ERROR,		// neocekavana chyba
    /// už nelze dále posouvat historii dopředu
    REDO_LIMIT,			// uz nelze dale posouvat historii dopredu
    /// už nelze dále posouvat historii dozadu
    UNDO_LIMIT,			// -||- dozadu
    /// protihráč nemá kam táhnout, hráč hraje znova
    REPEATE_TURN		// protihrac nema kam tahnout, hrac hraje znovu
};

/// typi umělé inteligence
enum othAi{
    noneAi,
    firstAi,
    secondAi
};

/// nekonstantí ukazatel na desku
typedef std::valarray<std::bitset<2>>* oboardPtr;

#endif // TYPEDEFS_H
