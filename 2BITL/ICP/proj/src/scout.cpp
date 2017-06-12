
/**
 * @file   scout.cpp
 * @Author Adam Piecek
 * @date   May, 9
 *
 */

#include "core.h"

/* prehled tabulka
 *  souradnice [0, 0] je vlevo nahore:
 * 		x---->
 * y  	  0   1   2
 * |  0   E   E   B
 * v  1   W   E   E
 * 	  2   W   B   G
 *
    S_start 	= index - size;
    S_size 		= y;
    S_stride 	= -size;

    J_start 	= index + size;
    J_size 		= size - y - 1;
    J_stride	= size;

    V_start		= index + 1;
    V_size		= size - x - 1;
    V_stride	= 1;

    Z_start		= index - 1;
    Z_size		= x;
    Z_stride	= -1;

    SV_start	= index - size + 1;
    SV_size		= ((y < (n-x)) ? y : (n-(x+1)));
    SV_stride	= -size + 1;

    SZ_start	= index - size - 1;
    SZ_size		= (y > x ? x : y);
    SZ_stride	= -size - 1;

    JV_start	= index + size + 1;
    JV_size		= (((n-x) < (n-y)) ? (n-(x+1)) : (n-(y+1)));
    JV_stride	= size + 1;

    JZ_start	= index + size - 1;
    JZ_size		= ((x < (n-y)) ? x : (n-(y+1)));
    JV_stride	= size - 1;

*/


bool Scout::main_scout(Scout* ptr, int index, oboardPtr b, std::bitset<2> bits){
    board = b;
    lookFor = bits;

    int sizeArray = (*size) * (*size);
    int sizeRange = (*size);
    int x = index%sizeRange;
    int y = index/sizeRange;
    int start;
    int size;
    int stride;
    std::bitset<2> opposite = bits.flip();

    // smer sever
    start = index - sizeRange;
    if((start >= 0) && (start < sizeArray) && (b[0][start] == opposite)){
        size = y;
        stride = -sizeRange;
        if(ptr->start_scouting(start, size, stride))
            return true;
    }

    // smer jih
    start = index + sizeRange;
    if((start >= 0) && (start < sizeArray) && (b[0][start] == opposite)){
        size = sizeRange - y - 1;
        stride = sizeRange;
        if(ptr->start_scouting(start, size, stride))
            return true;
    }

    // smer vychod
    start = index + 1;
    if((start >= 0) && (start < sizeArray) && (b[0][start] == opposite)){
        size = sizeRange - x - 1;
        stride = 1;
        if(ptr->start_scouting(start, size, stride))
            return true;
    }

    // smer zapad
    start = index - 1;
    if((start >= 0) && (start < sizeArray) && (b[0][start] == opposite)){
        size = x;
        stride = -1;
        if(ptr->start_scouting(start, size, stride))
            return true;
    }

    // smer severo-vychod
    start = index - sizeRange + 1;
    if((start >= 0) && (start < sizeArray) && (b[0][start] == opposite)){
        size = ((y < (sizeRange-x)) ? y : (sizeRange-(x+1)));
        stride = -sizeRange + 1;
        if(ptr->start_scouting(start, size, stride))
            return true;
    }

    // smer severo-zapad
    start = index - sizeRange - 1;
    if((start >= 0) && (start < sizeArray) && (b[0][start] == opposite)){
        size = (y > x ? x : y);
        stride = -sizeRange - 1;
        if(ptr->start_scouting(start, size, stride))
            return true;
    }

    // smer jiho-vychod
    start = index + sizeRange + 1;
    if((start >= 0) && (start < sizeArray) && (b[0][start] == opposite)){
        size = (((sizeRange-x) < (sizeRange-y)) ? (sizeRange-(x+1)) : (sizeRange-(y+1)));
        stride = sizeRange + 1;
        if(ptr->start_scouting(start, size, stride))
            return true;
    }

    // smer jiho-zapad
    start = index + sizeRange - 1;
    if((start >= 0) && (start < sizeArray) && (b[0][start] == opposite)){
        size = ((x < (sizeRange-y)) ? x : (sizeRange-(y+1)));
        stride = sizeRange - 1;
        if(ptr->start_scouting(start, size, stride))
            return true;
    }

    return false;
}

bool Scout_grey::start_scouting(int start, int size, int stride){
    int position = start;
    std::bitset<2> seeker;
    for(int step = 1; step <= size; position += stride){
        seeker = board[0][position];
        if((seeker == StateType::EMPTY) || (seeker == StateType::GREY))
            break;
        if(lookFor == seeker)
            return true; // <-
        step++;
    }
    return false;
}

bool Scout_flip::start_scouting(int start, int size, int stride){
    int position = start;
    std::bitset<2> seeker;
    for(int step = 1; step <= size; position += stride){
        seeker = board[0][position];
        if((seeker == StateType::EMPTY) || (seeker == StateType::GREY))
            break;
        if(lookFor == seeker){
            match.push_back(std::slice(start, step, stride)); // <-
            return false; // <-
        }
        step++;
    }
    return false;
}
