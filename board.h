/**
 *  @file board.h
 *  @author Vincent Li
 *  This file implements the Tic Tac Toe board, specficially its configuration, 
 *  and functions that return the game state and status. 
 */

#pragma once
#ifndef BOARD
#define BOARD

#include <string>
#include "util.h"

class GameBoard {
    public:
        // 2D array representing the board.
        char grid[3][3] = {{CLEAR, CLEAR, CLEAR}, {CLEAR, CLEAR, CLEAR}, {CLEAR, CLEAR, CLEAR}};

        // Returns a string representation of the board.
        std::string visBoard();

        /**
         *  Return the address of the first element of the grid.
         */
        char* getGridPtr();

        /**
         *  Place a marker on the board in the given row and col.
         *  Returns a bool for the status.  true: successful; false: failed/illegal.
         */
        bool set(int row, int col, char mark);        

};

#endif  // BOARD