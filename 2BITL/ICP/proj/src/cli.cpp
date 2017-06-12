/**
 * @file cli.cpp
 * @brief Zdorjový soubor pro CLI.
 *
 * Modul pro rozhraní zobrazitelné v konzoli.
 *
 * @author Jan Pavlica, Adam Piecek
 */

#include "cli.h"
#include <vector>
// globální proměnná pro předávání stavových infromací
GameState *gsc;

void NewGame::init()
{
    status = 0;
}
// vykreslovací funce pro CLI
void NewGame::drawGame(){

    bool turn = gsc->get_turn();
    const int* score = gsc->get_score();
    othBoardPtr b = gsc->get_board();
    int index;
    char c;
    cout << "  ";
    for(int j = 1; j<size+1; j++)
        cout << j << " ";
    cout << endl;
    for(int y = 0; y < size; y++){
        c = y + 'A';
        cout << c << " ";
        for(int x = 0; x < size; x++){
            index = y*size + x;
            if(b[0][index] == BitStateType::BLACK)
                cout << "B ";
            else if(b[0][index] == BitStateType::WHITE)
                cout << "W ";
            else if(b[0][index] == BitStateType::GREY)
                cout << "G ";
            else if(b[0][index] == BitStateType::EMPTY)
                cout << "_ ";
        }
        cout << endl;
    }
    cout << "score P1: " << *score << endl;
    cout << "stones left: "<< ((size*size)-(*score)) << endl;
    score++;
    cout << "score P2: " << *score << endl;
    cout << "stones left: "<< ((size*size)-(*score)) << endl;
    cout << "turn " << (turn ? "P1" : "P2") << endl;
}
// získání parametrů
int NewGame::getParams()
{
    int inSize = 0;
    string inEnemy = "";

    cout << "Please type size of the board, you can choose from [6,8,10,12] or type [0] for cancel: ";
    cin >> inSize;
    while(inSize != 6 && inSize != 8 && inSize != 10 && inSize != 12 && inSize != 0)
    {
        cout << "Choosen size is not avaliable, choose from 6,8,10,12 or 0: ";
        if(!cin)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin >> inSize;
    }
    if(inSize == 0)
    {
        cout << "New game was canceled" << endl;
        return 1;
    }
    size = inSize;

    cout << "Please type specify your oponnent from [P,AI1,AI2] or type [C] for cancel: ";
    cin >> inEnemy;
    while(inEnemy.compare("P") && inEnemy.compare("AI1") && inEnemy.compare("AI2") && inEnemy.compare("C"))
    {
        cout << "Choosen size is not avaliable, choose from P,AI1,AI2 or C: ";
        cin >> inEnemy;
    }

    if(!inEnemy.compare("P"))
    {
        ai = othAi::noneAi;
    }
    else if(!inEnemy.compare("AI1"))
    {
        ai = othAi::firstAi;
    }
    else if(!inEnemy.compare("AI2"))
    {
        ai = othAi::secondAi;
    }
    else if(!inEnemy.compare("C"))
    {
        cout << "New game was canceled" << endl;
        return 1;
    }
    else
    {
        cout << "ERROR";
        return 1;
    }

    return 0;
}

void NewGame::startGame()
{
    if(getParams())
    {
        cout << "Wrong specify of enemy or board size.";
        return;
    }

    if(status == 1)
    {
        currentGame->quitGame();
    }

    callDraw drawFunc = std::bind(&NewGame::drawGame, this);

    currentGame = new Othello(size, ai, drawFunc, gsc);
    status = 1;
    NewGame::drawGame();
}


void NewGame::saveGame(){
    string input;

    cout << "Type name of your save: ";
    cin >> input;
    cout << endl;

    currentGame->saveGame(input);
}

void NewGame::loadGame(){
    unsigned int input;
    int numOfGame = 0;

    vector<string> games;
    games = chooseGame();
    cout << "Games: " << endl;
    for(vector<string>::iterator it = games.begin(); it != games.end(); ++it){
        cout << numOfGame << "\t" << *it << endl;
        numOfGame++;
    }

    if(games.size() > 0){
        cout << "Type number of game to be load: ";
        cin >> input;
        while(input >= games.size()){
            cout << endl;
            cout << "Mismatch number. Type again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> input;
        }
        size = currentGame->loadGame(games[input]);
        drawGame();
    }
    else{
        cout << "There are no saved games" << endl;
    }
}

void NewGame::playGame()
{
    cout << "In game you can use these commands:\n \
            S - for save game \n \
            L - for load game \n \
            U - for undo \n \
            R - for redo \n \
            P - put stone on [X,Y] which will be specified (X is number and Y letter)\n \
            Q - for quit game \n \
            H - for this help\n";

    othRet returnValue = othRet::VALID_TURN;
    string input;

    while(returnValue != othRet::GAME_OVER)
    {
        cout << "Type your command: ";
        cin >> input;
        cout << endl;

        if(!input.compare("S"))
        {
            saveGame();
            continue;
        }
        else if(!input.compare("L"))
        {
            loadGame();
            continue;
        }
        else if(!input.compare("U"))
        {
            returnValue = currentGame->undo();
        }
        else if(!input.compare("R"))
        {
            returnValue = currentGame->redo();
        }
        else if(!input.compare("H"))
        {
            cout << "In game you can use these commands:\n \
                    S - for save game \n \
                    L - for load game \n \
                    U - for undo \n \
                    R - for redo \n \
                    P X Y - put stone on [X,Y] where X is number and Y letter\n \
                    Q - for quit game \n \
                    H - for this help\n";
        }
        else if(!input.compare("Q"))
        {
            currentGame->quitGame();
            return;
        }
        else if(!input.compare("P"))
        {
            int x, y;
            char yIn;
            cout << "Specify X (number): ";
            cin >> x;
            while (x < 1 || x > (size+1)){
                if(!cin)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cout << "Wrong X, specify again: ";
                cin >> x;
            }

            cout << "Specify Y (Upper letter): ";
            cin >> yIn;
            y = int(yIn) - 65;
            while (y < 0 || y > (size))
            {
                       cout << "Wrong Y, specify again: ";
                       cin >> yIn;
                       y = int(yIn) - 65;
                   }
            x = x - 1;

            cout << "X: " << x << "Y: " << y << endl;

            returnValue = currentGame->putToken(x,y);
            switch(returnValue)
            {
            case othRet::INVALID_TURN:
                cout << "Invalid move.\n";
                break;
            case othRet::GAME_OVER:
                cout << "Game over.\n";
                break;
            case othRet::INTERNAL_ERROR:
                cout << "Internal error.\n";
                break;
            case othRet::REPEATE_TURN:
                cout << "Your opponent cannot move now. Your turn.\n";
                break;
            default:
                break;
            }

        }
        else
        {
           cout << "Wrong command" << endl;
        }
    }

}
