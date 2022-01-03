/**
 *  @file playerhuman.cpp
 *  @author Vincent Li
 */

#include "playerhuman.h"


#if defined(MINIMAL_VERBOSE) || defined(VERBOSE) || defined(DEBUG)
#include <iostream>
#endif  // defined(MINIMAL_VERBOSE) || defined(VERBOSE) || defined(DEBUG)
moveRCPair HumanPlayer::chooseMove(Game* game) {
    std::string input;
    do {
#if defined(VERBOSE) || defined(DEBUG)
        std::cout << "\t" << this->mark << ":"<< "Enter a coordinate of the form <row>,<col>: ";
#elif defined(MINIMAL_VERBOSE)
        std::cout << game->turns << " " << this->mark << ":";
#endif
        std::cin >> input;
    } while(game->validatePlayerInput(input));

    moveRCPair move = processPlayerInput(input);
#if defined(VERBOSE) || defined(DEBUG)
    std::cout << "\tPlayer " << this->mark << " marks " << move.row << "," << move.column << std::endl; 
#endif
    return move;
}