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
- ```playermontecarlo.cpp``` and ```playermontecarlo.h```
    - An AI player that uses Monte Carlo Tree Search to pick an optimal move.
    - Given a number of iterations to run MCTS.
    - Chooses the best action based on the which node is the most promising, which is a combo of wins and losses per visit.
    - The same game tree is maintained from start to finish, and it is grafted after each action so that the current root is the current game state.  The reasons for this are that the player can utilize simulation results from iterations done during its previous turns, and since it uses light playout (random moves), the tree will (hopefully) become more 'balanced' and thorough by the end of the game, rather than being limited to X iterations for every turn.
- ```board.cpp``` and ```board.h```
    - Implements the Tic Tac Toe board plus get/set functions.
- ```util.h```
    - Defines constants, parameters, and values used by multiple files.
