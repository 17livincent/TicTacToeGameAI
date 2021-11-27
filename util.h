/**
 *  @file util.h
 *  @author Vincent Li
 *  Provides constants/values/functions shared by multiple files.
 */

#pragma once
#ifndef UTIL
#define UTIL

#include <string>

#define row first
#define column second
#define moveRCPair std::pair<int,int>

// Details of player X
const int PLAYER_X_CODE = 1;
const std::string PLAYER_X_NAME = "Player X";
const char PLAYER_X_MARK = 'X';

// Details of player O
const int PLAYER_O_CODE = 0;
const std::string PLAYER_O_NAME = "Player O";
const char PLAYER_O_MARK = 'O';

// Placeholder for an empty box in the board
const char CLEAR = '_';

const int ROWS = 3;

const int COLS = 3;

const int PLAYER_O_WON = PLAYER_O_CODE;
const int PLAYER_X_WON = PLAYER_X_CODE;
const int DRAW = -1;

#endif  // UTIL