/**
 *  @file playerhuman.cpp
 *  @author Vincent Li
 */

#include <iostream>

#include "playerhuman.h"

moveRCPair HumanPlayer::chooseMove(Game* game) {
    std::string input;
    do {
        std::cout << "\tEnter a coordinate of the form <row>,<col>: ";
        std::cin >> input;
    } while(game->validatePlayerInput(input));

    moveRCPair move = processPlayerInput(input);
    std::cout << "\tPlayer " << this->mark << " marks " << move.row << "," << move.column << std::endl; 

    return move;
}