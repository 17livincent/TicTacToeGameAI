/**
 *  @file player.h
 *  @author Vincent Li
 *  Represents the player in Tic Tac Toe.
 */

#pragma once
#ifndef PLAYER
#define PLAYER

#include "game.h"

class Game; // forware declaration

class Player {
    public:
        char mark;  // player mark (X or O)
        int code;   // 0 for player O, 1 for player X

        // Constructor
        Player(int code, char mark);

        /**
         *  Create and return a pair based on the row,column input.
         *  Player input must be validated before this using Game::validatePlayerInput().
         */
        moveRCPair processPlayerInput(std::string input);

        /**
         *  Return a moveRCPair after looking at the game.
         *  Wait for the human player to enter his/her move.  The AI player will calculate the best move and the return it.
         */
        virtual moveRCPair chooseMove(Game game);
};

#endif  // PLAYER