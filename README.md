# TicTacToeGameAI

This personal project is a Tic Tac Toe Game between 2 human or AI (WORK IN PROGRESS) players.

## File Descriptions
- ```play.cpp``` and ```play.h```
    - Creates the executed ```play``` or ```play.exe``` file.
    - Runs a full game of Tic Tac Toe between two players.
- ```game.cpp``` and ```game.h```
    - Manages the game on behalf of the players.
    - Validates player inputs.
    - Determines the game result: win/loss/draw.
- ```player.cpp``` and ```player.h```
    - Specifies a player that can view the game grid and make moves.
    - Inherited to create different types of players.
- ```board.cpp``` and ```board.h```
    - Implements the Tic Tac Toe board plus get/set functions.
- ```util.h```
    - Defines constants, parameters, and values used by multiple files.
