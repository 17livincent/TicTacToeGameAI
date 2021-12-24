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
    - Also contains some helper functions.
- ```playerhuman.cpp``` and ```playerhuman.h```
    - A player that uses command line input to pick moves.
- ```playerminimax.cpp``` and ```playerminimax.h```
    - An AI player that uses the Minimax algorithm to pick an optimal move.
    - Given a search depth limit to create the game tree.
    - Uses a simple evaluation function as the heuristic.
- ```playermontecarlo.cpp``` and ```playermontecarlo.h```
    - An AI player that uses Monte Carlo Tree Search to pick an optimal move.
    - MCTS is run for a given number of iterations.
    - There are a few differences in this version of MCTS.  Selection can return a terminal node, and if this happens, expansion won't happen.  Still, simulation will return the result of a terminal node, and that result will be backpropagated.
    - The same game tree is maintained from start to finish.  As moves are played, a scion of the tree is created from the node with the current game state.  This node is labeled as the new root, and then the nodes of alternate pasts/presents/futures are deleted starting from the old root.  MCTS is then run from the new root.  The reasons for this are that the player can utilize knowledge accumulated during the previous iterations and turns, and since light playout is used, the various simulations and their results will create a better-informed game tree.
    - The estimated number of moves from a game state to a win, calculated for each simulated win, is a factor in determining the optimal action.  The goal is that the most promising node has a high (win + draw) : visit ratio as well as being closer to a winning move.  This is helpful for playing Tic Tac Toe because playing a closer or immediate winning move is far more important than longevity and playing a distant winning move.
- ```board.cpp``` and ```board.h```
    - Implements the Tic Tac Toe board plus get/set functions.
- ```util.h```
    - Defines constants, parameters, and values used by multiple files.
