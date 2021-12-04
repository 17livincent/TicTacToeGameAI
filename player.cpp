/**
 *  @file player.cpp
 *  @author Vincent Li
 */

#include <iostream>

#include "player.h"
#include "util.h"


Player::Player(int code, char mark) {
    this->code = code;
    this->mark = mark;
}

std::list<moveRCPair> Player::getValidActions(char gameState[3][3]) {
    std::list<moveRCPair> actions;

    for(int i = 0; i < 9; i++) {
        int row = int(i / 3);
        int col = i % 3;
        if(gameState[row][col] == CLEAR) {
            moveRCPair move;
            move.row = row;
            move.column = col;
            actions.push_back(move);
        }
    }

    return actions;
}

moveRCPair Player::processPlayerInput(std::string input) {
    moveRCPair pair;
    std::string inputCopy = input;

    std::string delim = ",";
    int delimLoc = inputCopy.find(delim);
    std::string r = inputCopy.substr(0, delimLoc);
    std::string c = inputCopy.substr(delimLoc + 1, inputCopy.length() - 1);
    int rVal = std::stoi(r);
    int cVal = std::stoi(c);

    pair.row = rVal;
    pair.column = cVal;

    return pair;
}