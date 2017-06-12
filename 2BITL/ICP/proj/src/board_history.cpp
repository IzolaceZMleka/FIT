
/**
 * @file   board_history.cpp
 * @Author Adam Piecek
 * @date   May, 9
 *
 */

#include "core.h"
//#include <iterator>
#include <iostream>

// Board_history
oboardPtr Board_History::initBoard(){
    int sizeRange = *size;
    oboardPtr b = new std::valarray<std::bitset<2>>(sizeRange * sizeRange);
    int firstPat = ((sizeRange/2 -1)*sizeRange + sizeRange/2) - 1;
    int secondPat = (sizeRange/2*sizeRange + sizeRange/2) - 1;

    b[0][firstPat]    = StateType::WHITE;
    b[0][firstPat+1]  = StateType::BLACK;
    b[0][secondPat]   = StateType::BLACK;
    b[0][secondPat+1] = StateType::WHITE;


    hist.push_back(b);
    actualBoard = hist.begin();

    return b;
}

oboardPtr Board_History::new_board(){
    // historie se musi promazat, jestlize se po undo vlozi kamen
    if(actualBoard != std::prev(hist.end())){
        actualBoard++; // nesmi se smazat na prave nahlizejici
        // dealokace
        oboardPtrVect::iterator eraseIt = actualBoard;
        for( ; actualBoard != hist.end(); actualBoard++)
            delete *actualBoard;
        // smazani pointeru z vectoru
        hist.erase(eraseIt, hist.end());
        actualBoard = std::prev(hist.end());
    }
    // novy board je kopii predesleho
    oboardPtr bnew = new std::valarray<std::bitset<2>>((*size) * (*size));
    *bnew = **actualBoard;
    // kopii se ulozi do vectoru
    hist.push_back(bnew);
    // posune se iterator, aby ukazoval na kopii
    actualBoard = std::prev(hist.end());
    return *actualBoard;
}

oboardPtr Board_History::get_board(int get){
    // i = 0 aktualni board, i = 1 redo board o 1, i = -2 undo board o 2
    std::vector<oboardPtr>::iterator temp;
    temp = actualBoard;
    if(get == 0)
        return *temp;
    if(0 < get)
        for( ; (get != 0) && (temp != hist.begin()); --temp);
    else
        for( ; (get != 0) && (temp != hist.end()); ++temp);
    return *temp;
}

othRet Board_History::undo(){
    if(actualBoard != hist.begin()){
        actualBoard--;
        return othRet::VALID_TURN;
    }
    else
        return othRet::UNDO_LIMIT;
}

othRet Board_History::redo(){
    if(actualBoard != std::prev(hist.end())){
        actualBoard++;
        return othRet::VALID_TURN;
    }
    else
        return othRet::REDO_LIMIT;
}

Board_History::~Board_History(){
    for(std::vector<oboardPtr>::iterator it = hist.begin(); it != hist.end(); ++it){
        delete *it;
    }
}
