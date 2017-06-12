#ifndef IO_H
#define IO_H

/**
 * @file   io.h
 * @Author Adam Piecek
 * @date   May, 9
 * @brief  ukládání, nahrávání hry
 *
 */

#include "messenger.h"
#include <string>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/bitset.hpp>
#include <boost/serialization/valarray.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

/**
 * @brief       třída pro práci se serializací
 */
class ToFile{
public:
    int ai;
    int size;
    std::vector<int> score;
    oboardPtrVect boards;
    turnVect turns;
private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & ai;
        ar & size;
        ar & score;
        ar & boards;
        ar & turns;
    }
};

/// struktura pro core
typedef struct fileDf{
    othAi ai;
    int size;
    scoreVect score;
    oboardPtrVect boardsptr;
    turnVect turns;
}fromfile;


/**
 * @brief       stará se o save game/ load game
 * @details		komunikacni objekt, pres nej se zjistuje stav aktualne hrane hry
 */
class IO_Game{
    Messenger* msg;
    /// struktura pro zápis
    ToFile f;
    /// ukazatel na strukturu po načtení
    fromfile* ffptr = NULL;
public:
    IO_Game(Messenger* msg): msg(msg){}
    void saveHistory(std::string name);
    void loadHistory();
    fromfile* loadGame(std::string name);
    void saveGame(std::string name);
    void uploadHistory(int s, int a) { f.size = s; f.ai = a;}
    void uploadHistory(scoreVect);
    void uploadHistory(oboardPtrVect opv) { f.boards = opv;}
    void uploadHistory(turnVect t)	{ f.turns = t;}
    fromfile* downloadHistory();
    ~IO_Game(){ delete ffptr;}
};

#endif // IO_H
