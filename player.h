/**
 *  @file player.h
 *  @author Vincent Li
 *  Parent class of the player in Tic Tac Toe.
 */

#pragma once
#ifndef PLAYER
#define PLAYER

#include "defines.h"

#include <iostream>
#include <list>

#include "game.h"

class Game; // forware declaration

#define copyGameState(original, copyTo) for(int i = 0; i < 9; i++) copyTo[int(i / 3)][i % 3] = original[int(i / 3)][i % 3];

/**
 * Returns whether or not two grids are equal.
 */
bool gameStatesAreEqual(char g1[3][3], char g2[3][3]);

/**
 *  Get a list of valid actions.
 */
std::list<moveRCPair> getValidActions(char gameState[3][3]);

class Player {
    public:
        char mark;  // player mark (X or O)
        int code;   // 0 for player O, 1 for player X

        // Constructor
        Player(int code, char mark);

        // Destructor
        virtual ~Player();

        /**
         *  Create and return a pair based on the row,column input.
         *  Player input must be validated before this using Game::validatePlayerInput().
         */
        moveRCPair processPlayerInput(std::string input);

        /**
         *  Return a moveRCPair after looking at the game.
         *  Wait for the human player to enter his/her move.  The AI player will calculate the best move and the return it.
         */
        virtual moveRCPair chooseMove(Game* game) {
            // dummy body
#if defined(DEBUG)
            std::cout << "Dummy function called" << std::endl;
#endif  // defined(DEBUG)
            moveRCPair dummy;
            return dummy;
        };
};

#endif  // PLAYER