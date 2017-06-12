
/**
 * @file   artificial_intelligence.cpp
 * @Author Adam Piecek
 * @date   May, 9
 *
 */

#include "artificial_intelligence.h"
#include <algorithm>
#include <valarray>
#include <random>

int AI_1::main_ai(){
    greyVect greys = msg->get_greyIndexes();

    std::srand(time(NULL));
    int s = greys.size();
    int i = (std::rand()  % s);
    return greys[i];
}

int AI_2::main_ai(){
    greyVect greys = msg->get_greyIndexes();
    msg->clear_slicesVect();
    oboardPtr b = msg->get_board();

    for(greyVect::iterator it = greys.begin(); it != greys.end(); ++it){
        msg->main_scout(sp, *it, b, StateType::WHITE);
    }
    slicesVect sv = msg->get_slices();
    int m = 0;
    int slicSize;
    std::slice s;
    slicesVect::iterator mit;
    for(slicesVect::iterator it = sv.begin(); it != sv.end(); ++it){
        s = *it;
        slicSize = s.size();
        if(slicSize >= m){
            m = slicSize;
            mit = it;
        }
    }
    s = *mit;
    return s.start() - s.stride();
}

/*
int AI_3::main_ai(){
    // vzdy se musi na zacatku main_ai aktualizovat pomocne History objekty
    downloadHistory();
    //...
    return 0;
}
*/
/*
othRet AI_Othello::putToken(int x, int y){
    if(msg->actualize_coords(x, y) != othRet::VALID_TURN)
        return othRet::INVALID_TURN;
    if(msg->play() != othRet::VALID_TURN)
        return othRet::INVALID_TURN;
    else
        return othRet::VALID_TURN;
}

othRet AI_Othello::undo(){
    if(msg->undo() == othRet::UNDO_LIMIT){
        return othRet::UNDO_LIMIT;
    }
    msg->undoTurnH();
    msg->undoScoreH();
    return othRet::VALID_TURN;
}

othRet AI_Othello::redo(){
    if(msg->redo() == othRet::REDO_LIMIT){
        return othRet::REDO_LIMIT;
    }
    msg->redoTurnH();
    msg->redoScoreH();
    return othRet::VALID_TURN;
}
*/

AI_Othello_Intrfc::~AI_Othello_Intrfc(){}
AI_Othello::~AI_Othello(){}
AI_1::~AI_1(){}
AI_2::~AI_2(){}
