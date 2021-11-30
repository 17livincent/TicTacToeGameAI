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

moveRCPair Player::chooseMove(Game game) {
    std::string input;
    do {
        std::cout << "\tEnter a coordinate of the form <row>,<col>: ";
        std::cin >> input;
    } while(game.validatePlayerInput(input));

    moveRCPair move = processPlayerInput(input);
    std::cout << "\tPlayer " << this->mark << " marks " << move.row << "," << move.column << std::endl; 

    return move;
}