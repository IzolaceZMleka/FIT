
/**
 * @file   core.cpp
 * @Author Adam Piecek
 * @date   May, 9
 *
 */

#include "core.h"
#include <iostream>

//using namespace std;

// Move_switcher
Move_switcher::Move_switcher(Messenger* msgp, othAi aip, Coords_system* csp){
    msg = msgp; ai = aip; cs = csp;
    player1 = new Human(msg, this, true, StateType::BLACK, cs);
    if(ai == othAi::noneAi){
        player2 = new Human(msg, this, false, StateType::WHITE, cs);
    }
    else{
        player2 = new Computer(msg, this, false, StateType::WHITE, cs);
    }
    whichPlayer = player1;
}

void Move_switcher::actualizeSwitcher(){
    // nasledujici tah
    bool nextTurn = msg->getActualTurn();
    if(nextTurn){
        whichPlayer = player1;
    }
    else{
        whichPlayer = player2;
    }
}

othRet Move_switcher::play(){
    if(ai != othAi::noneAi){
        othRet hRes = whichPlayer->play();
        if(hRes != othRet::VALID_TURN)
            return hRes;
        msg->actualizeSwitcher();
        othRet cRes = whichPlayer->play();
        if(cRes == othRet::GAME_OVER)
            return cRes;
        return othRet::VALID_TURN;
    }
    return whichPlayer->play();
}

othRet Move_switcher::Human::play(){
    // kontrola, zda dal hrac kamen na sede misto
    bool validTurn = msg->checkIf_grey(cs->index);
    if(validTurn != true){
        return othRet::INVALID_TURN;
    }

    // vytvoreni nove desky -> kopie dosud aktualni
    oboardPtr b = msg->new_board();
    // polozeni tokenu
    msg->putToken(cs->index, b, playerColor);
    // otoceni kamenu
    msg->flip_tokens(cs->index, b, playerColor);

    // pridani sedych ploch nasledujicimu hraci
    if(msg->grey_walk(b, enemyColor) == othRet::REPEATE_TURN){
        // pokud se zadna seda plocha nenasla -> opakovani tahu hrace
        // nastaveni sedych ploch pro hrace, co prave hraje
        if(msg->grey_walk(b, playerColor) == othRet::REPEATE_TURN){
            // neni plocha, na kterou by se dalo dat kamen
            msg->setTurnH(!player1);
            return othRet::GAME_OVER;
        }
        // zaznam pristiho tahu
        msg->setTurnH(player1);
        return othRet::REPEATE_TURN;
    }
    else{
        // zaznam pristiho tahu
        msg->setTurnH(!player1);
        return othRet::VALID_TURN;
    }
}

othRet Move_switcher::Computer::play(){
    msg->actualize_gameState();
    msg->draw();
    // vytvoreni nove desky -> kopie dosud aktualni
    oboardPtr b = msg->new_board();
    int index = msg->main_ai();
    // polozeni tokenu
    msg->putToken(index, b, playerColor);
    // otoceni kamenu
    msg->flip_tokens(index, b, playerColor);

    // pridani sedych ploch nasledujicimu hraci
    if(msg->grey_walk(b, enemyColor) == othRet::REPEATE_TURN){
        // pokud se zadna seda plocha nenasla -> opakovani tahu hrace
        // nastaveni sedych ploch pro hrace, co prave hraje
        if(msg->grey_walk(b, playerColor) == othRet::REPEATE_TURN){
            // neni plocha, na kterou by se dalo dat kamen
            msg->setTurnH(!player1);
            return othRet::GAME_OVER;
        }
        // zaznam pristiho tahu
        msg->setTurnH(player1);
        play();
        return othRet::REPEATE_TURN;
    }
    else{
        // zaznam pristiho tahu
        msg->setTurnH(!player1);
        return othRet::VALID_TURN;
    }
}

void Move_switcher::putToken(int index, oboardPtr b, std::bitset<2> tokenColor){
    b[0][index] = tokenColor;
}

Move_switcher::Player::~Player(){}

Move_switcher::~Move_switcher(){
    delete player1;
    delete player2;
}


// Turn_History
Turn_History::Turn_History(Messenger* msg): msg(msg){
    turns.push_back(true);
    actualPlayer = turns.begin();
}
void Turn_History::setTurnH(bool player){
    if(actualPlayer != std::prev(turns.end())){
        actualPlayer++;
        turns.erase(actualPlayer, turns.end());
    }
    turns.push_back(player);
    actualPlayer = std::prev(turns.end());
}
void Turn_History::undoTurnH(){
    if(actualPlayer != turns.begin())
        actualPlayer--;
}
void Turn_History::redoTurnH(){
    if(actualPlayer != std::prev(turns.end()))
        actualPlayer++;
}


// Flipper
void Flipper::flip_tokens(int index, oboardPtr b, std::bitset<2> playerColor){
    msg->clear_slicesVect();
    msg->main_scout(scoutPtr, index, b, playerColor);
    slicesVect match = msg->get_slices();
    for(slicesVect::iterator it = match.begin(); it != match.end(); ++it){
        b[0][*it] = playerColor;
    }
}


// Score_History
Score_History::Score_History(Messenger* msg): msg(msg){
    score.push_back(std::make_tuple(2, 2));
    actualScore = score.begin();
}

void Score_History::setScoreH(std::tuple<int, int> plScore){
    if(actualScore != std::prev(score.end())){
        actualScore++;
        score.erase(actualScore, score.end());
    }
    score.push_back(plScore);
    actualScore = std::prev(score.end());
}
void Score_History::undoScoreH(){
    if(actualScore != score.begin())
        actualScore--;
}
void Score_History::redoScoreH(){
    if(actualScore != std::prev(score.end()))
        actualScore++;
}
