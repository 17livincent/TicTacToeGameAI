/**
 *  @file game.cpp
 *  @author Vincent Li
 */

#include <iostream>

#include "game.h"

Game::Game() {
    // Initialize currentPlayer
    srand(time(NULL));
    currentPlayer = (rand() % 10) % 2;
}

bool Game::playerWins(const char playerMark) {
    bool won = false;

    // check horizontal
    if((playerMark == board.grid[0][0] && playerMark == board.grid[0][1] && playerMark == board.grid[0][2])
        || (playerMark == board.grid[1][0] && playerMark == board.grid[1][1] && playerMark == board.grid[1][2])
        || (playerMark == board.grid[2][0] && playerMark == board.grid[2][1] && playerMark == board.grid[2][2])) {
        won = true;
    }
    // check vertical
    else if((playerMark == board.grid[0][0] && playerMark == board.grid[1][0] && playerMark == board.grid[2][0])
        || (playerMark == board.grid[0][1] && playerMark == board.grid[1][1] && playerMark == board.grid[2][1])
        || (playerMark == board.grid[0][2] && playerMark == board.grid[1][2] && playerMark == board.grid[2][2])) {
        won = true;    
        
    }
    // check diagnol
    else if((playerMark == board.grid[0][0] && playerMark == board.grid[1][1] && playerMark == board.grid[2][2])
        || (playerMark == board.grid[2][0] && playerMark == board.grid[1][1] && playerMark == board.grid[0][2])) {
        won = true;
    }

    return won;
}

bool Game::isDraw() {
    bool draw = false;

    int markCount = 0;

    for(int r = 0; r < ROWS; r++) {
        for(int c = 0; c < COLS; c++) {
            if(board.grid[r][c] != CLEAR) markCount++;
        }
    }

    if(markCount == ROWS * COLS) draw = true;

    return draw;
}

bool Game::playerMarks(const char playerMark, int row, int col) {
    bool status = board.set(row, col, playerMark);

    if(status == true) {
        // add to history
        move m;
        m.playerMark = playerMark;
        m.row = row;
        m.col = col;
        history.push_back(m);

        // update current player
        currentPlayer = (currentPlayer + 1) % 2;

        // update turns
        turns++;
    }

    return status;
}

int Game::validatePlayerInput(std::string input) {
    std::string inputCopy = input;
    int status = 0;

    // find ',' and split input
    std::string delim = ",";
    int delimLoc = inputCopy.find(delim);
    std::string r = inputCopy.substr(0, delimLoc);
    std::string c = inputCopy.substr(delimLoc + 1, inputCopy.length() - 1);

    int rVal;
    int cVal;
    try {
        rVal = std::stoi(r);
    }
    catch(std::invalid_argument const& e) {
        // if the string is not numeric, give -1
        rVal = -1;
    }
    try {
        cVal = std::stoi(c);
    }
    catch(std::invalid_argument const& e) {
        // if the string is not numeric, give -1
        cVal = -1;
    }

    if((long unsigned int)delimLoc >= input.length()) {
        // 0b1 means no ',' given
        status |= 0b1;
    }
    if(r.empty() || rVal < 0 || rVal >= ROWS) {
        // 0b10 means row value is invalid
        status |= 0b10;
    }
    if(c.empty() || cVal < 0 || cVal >= COLS) {
        // 0b100 means col value is invalid
        status |= 0b100;
    }
    if(cVal >= 0 && cVal < COLS && rVal >= 0 && rVal < ROWS && board.grid[rVal][cVal] != CLEAR) {
        // 0b1000 means the grid slot is already taken
        status |= 0b1000;
    }

    // notify
    if(status != 0) {
        std::cout << "INVALID input" << std::endl;
        
        if((status & 0b1) == 0b1) {
            std::cout << "\tMissing ','" << std::endl;
        }
        if((status & 0b10) == 0b10) {
            std::cout << "\tRow value must be in [0, " << ROWS << ")" << std::endl;
        }
        if((status & 0b100) == 0b100) {
            std::cout << "\tCol value must be in [0, " << COLS << ")" << std::endl;
        }
        if((status & 0b1000) == 0b1000) {
            std::cout << "\tThe grid spot is already marked" << std::endl;
        }
    }

    return status;
}