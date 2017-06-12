
/**
 * @file   grey_manager.cpp
 * @Author Adam Piecek
 * @date   May, 9
 *
 */

#include "core.h"


othRet Grey_Manager::grey_walk(oboardPtr b, std::bitset<2> playerColor){
    using namespace BitStateType;
    int whiteCount = 0;
    int blackCount = 0;
    int greyCount = 0;
    int size = (*sizeRange) * (*sizeRange);
    std::bitset<2> bits;

    for(int i = 0; i < size; i++){
        bits = b[0][i];
        if((bits == EMPTY) || (bits == GREY)){

            //volani scouta
            if(msg->main_scout(scoutPtr, i, b, playerColor)){
                b[0][i] = GREY;
                greyCount++;
            }
            else{
                b[0][i] = EMPTY;
            }
        }
        else{
            // inkrementace pocitadla kamenu
            if(bits == WHITE)
                whiteCount++;
            else
                blackCount++;
        }
    }

    msg->setScoreH(std::make_tuple(blackCount, whiteCount));
    if(greyCount == 0)
        return othRet::REPEATE_TURN;
    return othRet::VALID_TURN;
}

othRet Grey_Manager_ForAI::grey_walk(oboardPtr b, std::bitset<2> playerColor){
    using namespace BitStateType;
    indexes.clear();
    int whiteCount = 0;
    int blackCount = 0;
    int greyCount = 0;
    int size = (*sizeRange) * (*sizeRange);
    std::bitset<2> bits;

    for(int i = 0; i < size; i++){
        bits = b[0][i];
        if((bits == EMPTY) || (bits == GREY)){

            //volani scouta
            if(msg->main_scout(scoutPtr, i, b, playerColor)){
                b[0][i] = GREY;
                greyCount++;
                indexes.push_back(i); // <-
            }
            else{
                b[0][i] = EMPTY;
            }
        }
        else{
            // inkrementace pocitadla kamenu
            if(bits == WHITE)
                whiteCount++;
            else
                blackCount++;
        }
    }

    msg->setScoreH(std::make_tuple(blackCount, whiteCount));
    if(greyCount == 0)
        return othRet::REPEATE_TURN;
    return othRet::VALID_TURN;
}

bool Grey_Manager::checkIf_grey(int index){
    oboardPtr b = msg->get_board();
    if(b[0][index] == BitStateType::GREY)
        return true;
    else
        return false;
}
