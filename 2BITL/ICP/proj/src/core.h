#ifndef CORE_H
#define CORE_H

/**
 * @file   core.h
 * @Author Adam Piecek
 * @date   May, 9
 * @brief  logické jádro Othella
 *
 */

#include <memory>
#include <tuple>

#include "gamestate.h"
#include "othello_types.h"
#include "messenger.h"

// pres tento objekt hra Othello upravuje stav aktualne hrane hry

/**
 * @brief       Manipuluje s objektem GameState, který ukazuje stav hry
 */
class GameStateManipulator: public GameState{
    /// ukazatel na Messenger
    Messenger* msg;
public:
    GameStateManipulator(Messenger* msg): msg(msg){}
/**
 * @brief       Nastavuje skore
 */
    void change_score(int index, int value)	{ set_score(index, value);}

/**
 * @brief       Nastavuje kdo je právě ted na tahu
 */
    void change_turn(bool player)			{ set_turn(player);}

/**
 * @brief       Nastavení nové desky, po každém tahu
 */
    void change_board(oboardPtr o)			{ set_board(o);}

/**
 * @brief       Přenastavení ukazatele
 * @details		Při load game, nutno vyměnit ukazatele na Messengera
 */
    void new_msg(Messenger* msgp) { msg = msgp;}
};


/**
 * @brief
 * @details		obsahuje hlavní pořadí volání objektů
 * 				přepíná těla funkcí mezi hráčem x hráčem, hráčem x AI
 */
class Move_switcher{


    /**
     * @brief       abstraktní třída hráč pro třídu Human nebo Computer
     */
    class Player {
    protected:
        /// true = pvní hráč, false druhý
        bool player1;
        /// barva hráče
        std::bitset<2> playerColor;
        /// barva protihráče
        std::bitset<2> enemyColor;
        /// ukazatel na strukturu obsahující obecné informace, souřadnice
        Coords_system* cs;
    public:
        Player(bool p1, std::bitset<2> pc, Coords_system* cs):
            player1(p1), playerColor(pc), enemyColor(pc.flip()), cs(cs){}

        /**
         * @brief       obsahuje pořadí volání objektů, algoritmus hry
         */
        virtual othRet play() = 0;
        virtual ~Player() = 0;
    };

    /**
     * @brief       lidský hráč
     */
    class Human : public Player {
        Messenger* msg;
        Move_switcher* ms;
    public:
        Human(Messenger* msg, Move_switcher* ms, bool pl, std::bitset<2> plc, Coords_system* cs):
            Player(pl, plc, cs), msg(msg), ms(ms){}
        othRet play() override;
        ~Human(){}
    };

    /**
     * @brief       hráč s AI
     */
    class Computer : public Player {
        Messenger* msg;
        Move_switcher* ms;
    public:
        Computer(Messenger* msg, Move_switcher* ms, bool pl, std::bitset<2> plc, Coords_system* cs):
                 Player(pl, plc, cs), msg(msg), ms(ms){
        }
        othRet play() override;
        ~Computer(){}
    };

    Messenger* msg;
    Coords_system* cs;
    othAi ai;

    /// ukazatel na virtuální metodu play, třídy Player
    Player* whichPlayer;
    Player* player1;
    Player* player2;

public:
    /**
     * @brief       položení hráčova kamenu na desku
     * @details		volá se z metody Player::play
     */
    void putToken(int index, oboardPtr b, std::bitset<2> tokenColor);
    Move_switcher(Messenger* msg, othAi ai, Coords_system* cs);
    /**
     * @brief       metodu volá messenger, ten volá příslušnou metodu Player::play
     */
    othRet play();
    /**
     * @brief       aktualizace kdo je na tahu
     * @details		volá Score_History
     */
    void actualizeSwitcher();
    ~Move_switcher();
};



/**
 * @brief       prohledává hrací desku
 */
class Scout{
protected:
    Messenger* msg;
    /// velikost desky - 8, 12...
    size_t* size;
    /// hleda tuto barvu kamene
    std::bitset<2> lookFor;
    /// ukazatel na desku
    oboardPtr board;
public:
    Scout(Messenger* msg, size_t* size): msg(msg), size(size){}

    /**
     * @brief       hlavní tělo hledání
     * @details		nahlíží se na sousední políčka
     */
    bool main_scout(Scout* ptr, int index, oboardPtr b, std::bitset<2> bits);

    /**
     * @brief       prohledává daným směrem
     * @details		pokud u sousedního políčka byl správný kámen
     */
    virtual bool start_scouting(int start, int size, int stride) = 0;
};

class Scout_grey: public Scout{
    bool start_scouting(int start, int size, int stride) override;
public:
    Scout_grey(Messenger *msg, size_t* size): Scout(msg, size){}
};

class Scout_flip: public Scout{
    /// vektor směrů, které vedou k otočení kamenů
    slicesVect match;
    bool start_scouting(int start, int size, int stride) override;
public:
    Scout_flip(Messenger *msg, size_t* size): Scout(msg, size){}

    /**
     * @brief       vrací vektor std::slice nalezených směrů, které vedou k otáčení kamenů
     * @details		pokud u sousedního políčka byl správný kámen
     */
    slicesVect get_slices() { return match;}

    /**
     * @brief       smazání vektoru match
     */
    void clear_slicesVect() { match.clear();}
};


/**
 * @brief       přepisuje barvu kameny na desce
 */
class Flipper{
    Messenger* msg;
    Scout* scoutPtr;
public:
    Flipper(Messenger* msg, Scout_flip* scPtr): msg(msg), scoutPtr(scPtr){}

    /**
     * @brief       otáčí kameny
     * @details		volá Scouta a na základě nalezených směrů otáčení kameny otočí
     */
    void flip_tokens(int index, oboardPtr b, std::bitset<2> playerColor);
};



/**
 * @brief       zobrazuje šedé plochy a počítá skore
 * @details		volá Scouta a na základě nalezených směrů otáčení kameny otočí
 */
class Grey_Manager {
protected:
    Messenger* msg;
    Scout* scoutPtr;
    /// velikost hraci desky - 6, 8...
    size_t* sizeRange;
public:
    Grey_Manager(Messenger* msg, Scout* scPtr, size_t* size): msg(msg), scoutPtr(scPtr), sizeRange(size){}

    /**
     * @brief       kontrola, zda je kámen položen na šedou plochu
     */
    bool checkIf_grey(int index);

    /**
     * @brief       hledá místo, kde může dát šedou
     * @details		prochází všechna políčka, volá třídu Scout, ta mu zjistí zda má dát na místo šedý nebo ne
     */
    virtual othRet grey_walk(oboardPtr b, std::bitset<2> playerColor);
    virtual greyVect get_greyIndexes(){ greyVect a; return a;}
    virtual ~Grey_Manager(){}
};
class Grey_Manager_ForAI: public Grey_Manager{
private:
    greyVect indexes;
public:
    Grey_Manager_ForAI(Messenger* msg, Scout* scPtr, size_t* size): Grey_Manager(msg, scPtr, size){}
    othRet grey_walk(oboardPtr b, std::bitset<2> playerColor) override;

    /**
     * @brief       vrací vector indexů, které značí místo, kde je šedý
     */
    greyVect get_greyIndexes() override { return indexes;}
};


/**
 * @brief       zaznamenává historii skore
 * @details		volán z Grey_Manager
 */
class Score_History{
    Messenger* msg;
    /// vector uchovávající skore hráčů
    scoreVect score;
    /// iterator ukazující na aktuální záznam v závislosti na undo/redo
    scoreVect::iterator actualScore;
public:
    Score_History(Messenger* msg);

    /**
     * @brief       přidání score
     * @details		prvni misto prvni hraci, druhe druhy
     */
    void setScoreH(std::tuple<int, int> plScore);
    void undoScoreH();
    void redoScoreH();

    /**
     * @brief       zápis do GameState
     */
    void writeTo_gameState() {
        msg->change_score(0, std::get<0>(*actualScore));
        msg->change_score(1, std::get<1>(*actualScore));
    }
    scoreVect uploadHistory()					{ return score;}
    void downloadHistory(scoreVect scorep)		{ score = scorep; actualScore = std::prev(score.end());}
};

/**
 * @brief       zaznaménává historii, kdo hrál
 */
class Turn_History{
    Messenger* msg;

    /// vektor boolu
    turnVect turns;
    /// iteratoru ukazujcící na poslední záznam v závislosti na undo/redo
    turnVect::iterator actualPlayer;
public:
    Turn_History(Messenger* msg);
    void setTurnH(bool player);
    void undoTurnH();
    void redoTurnH();
    bool getActualTurn() { return *actualPlayer;}

    void writeTo_gameState()					{ msg->change_turn(*actualPlayer);}
    turnVect uploadHistory()					{ return turns;}
    void downloadHistory(turnVect turnsp)		{ turns = turnsp; actualPlayer = std::prev(turns.end());}
};

/**
 * @brief       zaznaménává historii, kdo hrál
 */
class Board_History{
    Messenger* msg;
    size_t* size;
    oboardPtrVect hist;
    /// aktuálně hraná deska
    oboardPtrVect::iterator actualBoard;
public:
    Board_History(Messenger* msg, size_t* size): msg(msg), size(size){}

    /**
     * @brief       vytvoří inicializační desku a sestavení kamenů
     */
    oboardPtr initBoard();

    /**
     * @brief       vytvoří novou desku
     * @details		volá se při každém položení kamenu
     */
    oboardPtr new_board();

    /**
     * @brief       vrací desku
     */
    oboardPtr get_board(int get);
    oboardPtr get_board()		{ return *actualBoard;}

    /**
     * @brief       posun iterátoru actualBoard v závislosti na unod/redo rozkazu
     */
    othRet undo();
    othRet redo();

    /**
     * @brief       zápis do GameState
     */
    void writeTo_gameState()				  { msg->change_board(*actualBoard);}

    /**
     * @brief       pro load game / save game
     */
    oboardPtrVect uploadHistory()			  { return hist;}
    void downloadHistory(oboardPtrVect histp) { hist = histp; actualBoard = std::prev(hist.end());}
    ~Board_History();
};

#endif // CORE_H
