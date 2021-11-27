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
#include "player.h"

// Struct to record move history
struct move {
    char playerMark;
    // box marked
    int row;
    int col;
};

class Game {
    public:
        GameBoard board;
        /**
         *  Game state.
         *  0: still playing
         *  1: playerX won
         *  2: playerO won
         *  3: draw
         */
        int gameState = 0;
        int currentPlayer = -1;  // code for current player.  Initial value determines the player that goes first.
        int turns = 0;  // number of turns taken / boxes filled
        std::vector<move> history;  // vector of moves
        
        /**
         *  Constructor
         * Initializes currentPlayer randomly to 0 or 1.
         */
        Game();

        /**
         *  Scan the board to determine if Player X/O won.
         *  Return true if so, false if not.
         */
        bool playerWins(const char playerMark);

        /**
         *  Scan the board to determine if there is a draw.
         *  Should call this after calling playerWins() for both players.
         *  Return true if so, false if not.
         */
        bool isDraw();

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