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
    - Base class of all player types.
    - Players can view the board, see possible actions, and pick a move.
- ```playerhuman.cpp``` and ```playerhuman.h```
    - A player that uses command line input to pick moves.
- ```playerminimax.cpp``` and ```playerminimax.h```
    - An AI player that uses the Minimax algorithm to pick an optimal move.
    - Given a search depth limit to create the game tree.
    - Uses a simple evaluation function as the heuristic.
- ```board.cpp``` and ```board.h```
    - Implements the Tic Tac Toe board plus get/set functions.
- ```util.h```
    - Defines constants, parameters, and values used by multiple files.
