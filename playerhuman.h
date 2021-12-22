/**
 *  @file playerhuman.h
 *  @author Vincent Li
 *  Represnts a human player in Tic Tac Toe, who gives inputs via command line input.
 */

#pragma once
#ifndef HUMANPLAYER
#define HUMANPLAYER

#include "player.h"

class HumanPlayer: public Player {
    public:
        HumanPlayer(int code, char mark):Player(code, mark) {
            // Introduction
            std::cout << "Introducing Player " << this->mark << ", who is a human player" << std::endl;
        }

        ~HumanPlayer() {}

        moveRCPair chooseMove(Game* game);
};

#endif  // HUMANPLAYER