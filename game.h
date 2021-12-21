/**
 *  @file game.h
 *  @author Vincent Li
 *  The Game class manages the game board on behalf of the players.
 */

#pragma once
#ifndef GAME
#define GAME

#include <stdlib.h>
#include <time.h>
#include <vector>

#include "util.h"
#include "board.h"

/**
 *  Scan the board to determine if Player X/O won.
 *  Return true if so, false if not.
 */
bool playerWins(const char playerMark, char grid[3][3]);

/**
 *  Scan the board to determine if there is a draw.
 *  Should call this after calling playerWins() for both players.
 *  Return true if so, false if not.
 */
bool isDraw(char grid[3][3]);

class Game {
    public:
        GameBoard board;
        int currentPlayer = -1;  // code for current player.  Initial value determines the player that goes first.
        int turns = 0;  // number of turns taken / boxes filled
        std::vector<moveRCPair> history;  // vector of moves
        
        /**
         *  Constructor
         * Initializes currentPlayer randomly to 0 or 1.
         */
        Game();

        /**
         *  Player makes a move with its given mark and grid row and col.
         *  Return true if successful, false otherwise.
         *  Subsequently updates history, turns, and current player.
         */
        bool playerMarks(const char playerMark, int row, int col);

        /**
         *  Validate player input.  
         *  If the input is of form '<row>,<col>' and all rules pass, return 0.  
         *  Otherwise, return something else.
         */
        int validatePlayerInput(std::string input);

};

#endif  // GAME