/**
 * @file   io.cpp
 * @Author Adam Piecek
 * @date   May, 9
 *
 */

#include "io.h"
#include <fstream>


void IO_Game::saveHistory(std::string name){
    msg->uploadHistory();
    name = "save_games/" + name;

    // Serializing struct to student.data
    std::ofstream output_file(name);
    boost::archive::binary_oarchive oa(output_file);
    oa << f;
}

void IO_Game::loadHistory(){
    msg->downloadHistory();
}

fromfile* IO_Game::downloadHistory(){
    return ffptr;
}

fromfile* IO_Game::loadGame(std::string name){
    // Reading from it
    name = "save_games/" + name;

    delete ffptr;

    std::ifstream input_file{name};
    ToFile fi;
    boost::archive::binary_iarchive ia{input_file};
    ia >> fi;

    // ToFile to fromfile
    ffptr = new fromfile;
    std::valarray<std::bitset<2>> temp;

    // prevod vector<int> na vector<tuple<int,int>>
    int first;
    int second;
    for(std::vector<int>::iterator it = fi.score.begin() ; it != fi.score.end(); ++it){
        first = *it;
        ++it;
        second = *it;
        ffptr->score.push_back(std::make_tuple(first, second));
    }

    ffptr->ai = static_cast<othAi>(fi.ai);
    ffptr->boardsptr = fi.boards;
    ffptr->size = fi.size;
    ffptr->turns = fi.turns;

    return ffptr;
}

void IO_Game::saveGame(std::string name){
    saveHistory(name);
}

void IO_Game::uploadHistory(scoreVect s)	{
    // realokace vectoru
    std::vector<int>().swap(f.score);

    for (std::tuple<int, int> const& c : s){
        f.score.push_back(std::get<0>(c));
        f.score.push_back(std::get<1>(c));
    }
}
