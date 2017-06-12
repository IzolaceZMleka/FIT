
/**
 * @file   messenger.cpp
 * @Author Adam Piecek
 * @date   May, 9
 *
 */

#include "core.h"
#include "artificial_intelligence.h"
#include "io.h"

#include <iostream>

namespace StateType{
    std::bitset<2> EMPTY = std::bitset<2>(0x0);
    std::bitset<2> WHITE = std::bitset<2>(0x1);
    std::bitset<2> BLACK = std::bitset<2>(0x2);
    std::bitset<2> GREY = std::bitset<2>(0x3);
}

// Messenger
void Messenger::init(callDraw cdp, GameStateManipulator* gsmp, Move_switcher* msp, Turn_History* thp, Flipper* flp,
                     Scout_flip* scfp, Scout_grey* scgp, Grey_Manager* gmp, Score_History* shp, Board_History* bhp,
                     AI_Othello_Intrfc* aiOip, IO_Game* iogp){
        drawCallback = cdp; gsm = gsmp; ms = msp; th = thp; fl = flp; scf = scfp; scg = scgp; gm = gmp; sh = shp; bh = bhp;
        aiOi = aiOip; iog = iogp;
}

void Messenger::actualize_gameState(){
    th->writeTo_gameState();
    sh->writeTo_gameState();
    bh->writeTo_gameState();
}

othRet Messenger::actualize_coords(int x, int y){
    int size = cs->size -1;
    if((x > size) || (x < 0) || (y > size) || (y < 0))
        return othRet::INVALID_TURN;
    cs->x = x;
    cs->y = y;
    cs->index = y * cs->size + x;
    return othRet::VALID_TURN;
}
void Messenger::uploadHistory(){
    iog->uploadHistory(th->uploadHistory());
    iog->uploadHistory(sh->uploadHistory());
    iog->uploadHistory(bh->uploadHistory());
    iog->uploadHistory(cs->size, cs->ai);
}

void Messenger::downloadHistory(){
    fromfile* ffptr = iog->downloadHistory();
    th->downloadHistory(ffptr->turns);
    sh->downloadHistory(ffptr->score);
    bh->downloadHistory(ffptr->boardsptr);
}

    // IO_Game
void Messenger::save_game(std::string name){
    iog->saveGame(name);
}


// GameStateManipulator
void Messenger::change_score(int index, int value){
    gsm->change_score(index, value);
}
void Messenger::change_turn(bool player){
    gsm->change_turn(player);
}
void Messenger::change_board(oboardPtr b){
    gsm->change_board(b);
}


// Move_switcher

void Messenger::putToken(int index, oboardPtr b, std::bitset<2> tokenColor){
    ms->putToken(index, b, tokenColor);
}
othRet Messenger::play(){
    return ms->play();
}
int Messenger::main_ai(){
    return aiOi->main_ai();
}
void Messenger::actualizeSwitcher(){
    ms->actualizeSwitcher();
}

// Turn_History
void Messenger::setTurnH(bool player){
    th->setTurnH(player);
}
void Messenger::undoTurnH(){
    th->undoTurnH();
}
void Messenger::redoTurnH(){
    th->redoTurnH();
}
bool Messenger::getActualTurn(){
    return th->getActualTurn();
}

// Scout
bool Messenger::main_scout(Scout* ptr, int index, oboardPtr b, const std::bitset<2> bits){
    return ptr->main_scout(ptr, index, b, bits);
}
slicesVect Messenger::get_slices(){
    return scf->get_slices();
}
void Messenger::clear_slicesVect(){
    scf->clear_slicesVect();
}

// Flipper
void Messenger::flip_tokens(int index, oboardPtr b, std::bitset<2> playerColor){
    fl->flip_tokens(index, b, playerColor);
}


// Grey Manager
bool Messenger::checkIf_grey(int index){
    return gm->checkIf_grey(index);
}
othRet Messenger::grey_walk(oboardPtr b, std::bitset<2> playerColor){
    return gm->grey_walk(b, playerColor);
}
greyVect Messenger::get_greyIndexes(){
    return gm->get_greyIndexes();
}

// Score_History
void Messenger::setScoreH(std::tuple<int, int> plScore){
    sh->setScoreH(plScore);
}
void Messenger::undoScoreH(){
    sh->undoScoreH();
}
void Messenger::redoScoreH(){
    sh->redoScoreH();
}

// Board_history
oboardPtr Messenger::initBoard(){
    return bh->initBoard();
}
oboardPtr Messenger::new_board(){
    return bh->new_board();
}
oboardPtr Messenger::get_board(int get){
    return bh->get_board(get);
}
oboardPtr Messenger::get_board(){
    return bh->get_board();
}
othRet Messenger::undo(){
    return bh->undo();
}
othRet Messenger::redo(){
    return bh->redo();
}

/*
//
// Messenger_ForAI
//

// Messenger_ForAI
void Messenger_ForAI::initai(Move_switcher* msp, Turn_History* thp, Turn_History* thaip, Flipper* flp, Scout_flip* scfp,
          Scout_grey* scgp, Grey_Manager* gmp, Score_History* shp,
          Score_History* shaip, Board_History* bhp, Board_History* bhaip) {
    ms = msp; th = thp; thai = thaip, fl = flp; scf = scfp; scg = scgp; gm = gmp; sh = shp;
    shai = shaip; bh = bhp; bhai = bhaip;
}

void Messenger_ForAI::downloadHistory(){
    thai->downloadHistory(th->uploadHistory());
    bhai->downloadHistory(bh->uploadHistory());
    shai->downloadHistory(sh->uploadHistory());
}

// Grey_Manager_ForAI
greyVect Messenger_ForAI::get_greyIndexes(){
    return gm->get_greyIndexes();
}

// Turn_History
void Messenger_ForAI::setTurnH(bool player){
    thai->setTurnH(player);
}
void Messenger_ForAI::undoTurnH(){
    thai->undoTurnH();
}
void Messenger_ForAI::redoTurnH(){
    thai->redoTurnH();
}

// Score_History
void Messenger_ForAI::setScoreH(std::tuple<int, int> plScore){
    shai->setScoreH(plScore);
}
void Messenger_ForAI::undoScoreH(){
    shai->undoScoreH();
}
void Messenger_ForAI::redoScoreH(){
    shai->redoScoreH();
}

// Board_history
oboardPtr Messenger_ForAI::initBoard(){
    return bhai->initBoard();
}
oboardPtr Messenger_ForAI::new_board(){
    return bhai->new_board();
}
oboardPtr Messenger_ForAI::get_board(int get){
    return bhai->get_board(get);
}
oboardPtr Messenger_ForAI::get_board(){
    return bhai->get_board();
}
othRet Messenger_ForAI::undo(){
    return bhai->undo();
}
othRet Messenger_ForAI::redo(){
    return bhai->redo();
}
*/
