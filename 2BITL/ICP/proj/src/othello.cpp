
/**
 * @file   othello.cpp
 * @Author Adam Piecek
 * @date   May, 9
 *
 */

#include "othello.h"
#include "builder.h"
#include <dirent.h>


Othello::Othello(size_t size, othAi aip, callDraw cd, GameState* &gsPtr){
   //@ kontrola argumentu

    ai = aip;
    // inicializace Builderu
    buildPtr = new Builder(size, aip, cd);
    // kontrukce objektu
    buildPtr->build();
    // ziskani ukazatele na Messenger
    msgPtr = buildPtr->get_messenger();
    // nastaveni nove hry
    oboardPtr b = msgPtr->initBoard();
    // nastaveni uzivatelova ukazatele na objekt GameState
    gsPtr = buildPtr->get_GameState();
    // zjisteni sedych poli
    msgPtr->grey_walk(b, StateType::BLACK);
    // zapsani do GameState
    msgPtr->actualize_gameState();
    // vykresleni
    //msgPtr->draw();
}

othRet Othello::putToken(int x, int y){
    msgPtr->actualizeSwitcher();
    if(msgPtr->actualize_coords(x, y) == othRet::INVALID_TURN)
        return othRet::INVALID_TURN;

    othRet response = msgPtr->play();
    if(response == othRet::INVALID_TURN)
        return othRet::INVALID_TURN;
    else{
        msgPtr->actualize_gameState();
        msgPtr->draw();
        return response;
    }
}

othRet Othello::undo(){
    if(ai != othAi::noneAi){
        do{
            if(msgPtr->undo() == othRet::UNDO_LIMIT){
                return othRet::UNDO_LIMIT;
            }
            if(msgPtr->undo() == othRet::UNDO_LIMIT){
                return othRet::UNDO_LIMIT;
            }
            msgPtr->undoTurnH();
            msgPtr->undoScoreH();
            msgPtr->undoTurnH();
            msgPtr->undoScoreH();
        } while(!(msgPtr->getActualTurn()));
    }
    else{
        if(msgPtr->undo() == othRet::UNDO_LIMIT){
            return othRet::UNDO_LIMIT;
        }
        msgPtr->undoTurnH();
        msgPtr->undoScoreH();
    }
    //msgPtr->actualizeSwitcher();
    msgPtr->actualize_gameState();
    msgPtr->draw();
    return othRet::VALID_TURN;
}

othRet Othello::redo(){
    if(ai != othAi::noneAi){
        do{
            if(msgPtr->redo() == othRet::REDO_LIMIT){
                return othRet::REDO_LIMIT;
            }
            if(msgPtr->redo() == othRet::REDO_LIMIT){
                return othRet::REDO_LIMIT;
            }
            msgPtr->redoTurnH();
            msgPtr->redoScoreH();
            msgPtr->redoTurnH();
            msgPtr->redoScoreH();
        } while(!(msgPtr->getActualTurn()));
    }
    else{
        if(msgPtr->redo() == othRet::REDO_LIMIT){
            return othRet::REDO_LIMIT;
        }
        msgPtr->redoTurnH();
        msgPtr->redoScoreH();
    }
    //msgPtr->actualizeSwitcher();
    msgPtr->actualize_gameState();
    msgPtr->draw();
    return othRet::VALID_TURN;
}

void Othello::quitGame(){
    this->~Othello();
}

std::vector<std::string> chooseGame(){
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> names;
    std::string name;
    if ((dir = opendir ("save_games/")) != NULL) {
      while ((ent = readdir (dir)) != NULL) {
        name = ent->d_name;
        if((name != ".") && (name != ".."))
            names.push_back(ent->d_name);
      }
      closedir (dir);
    }
    return names;
}

int Othello::loadGame(std::string name){
    int size;
    size = buildPtr->loadgame(name);
    msgPtr->downloadHistory();
    msgPtr->actualize_gameState();
    return size;
}

void Othello::saveGame(std::string name){
    msgPtr->save_game(name);
}

Othello::~Othello(){
    delete buildPtr;
}
