/**
 *  @file board.cpp
 *  @author Vincent Li
 */

#include <iostream>
#include <sstream>
#include "board.h"

std::string GameBoard::visBoard() {
    std::stringstream ss;
    ss << "   0  1  2\n"
       << "0 [" << grid[0][0] << "][" << grid[0][1] << "][" << grid[0][2] << "]\n" 
       << "1 [" << grid[1][0] << "][" << grid[1][1] << "][" << grid[1][2] << "]\n" 
       << "2 [" << grid[2][0] << "][" << grid[2][1] << "][" << grid[2][2] << "]\n";

    std::string display;

    display = ss.str();

    return display;
}

char* GameBoard::getGridPtr() {
    return (char*)(&this->grid);
}

bool GameBoard::set(int row, int col, char mark) {
    bool status = true;

    if(row > ROWS - 1 || col > COLS - 1 || row < 0 || col < 0) {
        status = false;
    }
    else if(grid[row][col] != CLEAR) {
        status = false;
    }
    else {
        grid[row][col] = mark;
    }

    return status;
}