/**
 *  @file play.cpp
 *  @author Vincent Li
 *  The executed file.
 */

#include "defines.h"

#if defined(MINIMAL_VERBOSE) || defined(VERBOSE) || defined(DEBUG)
#include <iostream>
#endif  // defined(MINIMAL_VERBOSE) || defined(VERBOSE) || defined(DEBUG)

#include <algorithm>
#include <vector>
#include <signal.h>

#include "play.h"
#include "game.h"

int Play::play(Player& playerX, Player& playerO) {
    int result;
    Game game;
    
    // game status
    int xWon = 0;
    int oWon = 0;
    int draw = 0;
    Player* player;
#if defined(VERBOSE) || defined(DEBUG)
    std::cout << "Welcome to a game of Tic-Tac-Toe!" << std::endl;
#endif  // defined(VERBOSE) || defined(DEBUG)
    while(xWon == 0 && oWon == 0 && draw == 0) {
        // Get current player
        player = (game.currentPlayer == playerO.code) ? &playerO : &playerX;
#if defined(VERBOSE) || defined(DEBUG)
        std::cout << "Board:\n" << game.board.visBoard() << std::endl;
        std::cout << "Turn " << game.turns + 1 << std::endl;

        // Notify whose turn it is
        std::cout << "Player " << player->mark << " goes: " << std::endl;
#endif  // defined(VERBOSE) || defined(DEBUG)     
        // Get valid player input
        moveRCPair move = player->chooseMove(&game);

        // Make the move
        game.playerMarks(player->mark, move.row, move.column);

        // Check game status
        xWon = playerWins(playerX.mark, game.board.grid);
        oWon = playerWins(playerO.mark, game.board.grid);
        draw = isDraw(game.board.grid);
    }

    if(xWon) {
#if defined(VERBOSE) || defined(DEBUG)
        std::cout << "Player X won!" << std::endl;
#endif  // defined(VERBOSE) || defined(DEBUG)
        result = PLAYER_X_WON;
#if defined (MINIMAL_VERBOSE)
        std::cout << "Result:" << result << std::endl;
#endif  // defined (MINIMAL_VERBOSE)
    }
    else if(oWon) {
#if defined(VERBOSE) || defined(DEBUG)
        std::cout << "Player O won!" << std::endl;
#endif  // defined(VERBOSE) || defined(DEBUG)
        result = PLAYER_O_WON;
#if defined (MINIMAL_VERBOSE)
        std::cout << "Result:" << result << std::endl;
#endif  // defined (MINIMAL_VERBOSE)
    }
    else if(draw) {
#if defined(VERBOSE) || defined(DEBUG)
        std::cout << "The game is a draw!" << std::endl;
#endif  // defined(VERBOSE) || defined(DEBUG)
        result = DRAW;
#if defined (MINIMAL_VERBOSE)
        std::cout << "Result:" << result << std::endl;
#endif  // defined (MINIMAL_VERBOSE)
    }

    return result;
}

void toExit(int sig) {
#if defined(VERBOSE) || defined(DEBUG)
    std::cout << std::endl << "Program terminated" << std::endl;
#endif  // defined(VERBOSE) || defined(DEBUG)
    delete playerO;
    delete playerX;
    exit(sig);
}

int main(int argc, char** argv) {
    signal(SIGINT, toExit);

    Play playGame;

    // Run a game between two human players.
    //HumanPlayer playerX = HumanPlayer(PLAYER_X_CODE, PLAYER_X_MARK);
    //HumanPlayer playerO = HumanPlayer(PLAYER_O_CODE, PLAYER_O_MARK);
    //playGame.play(playerX, playerO);

    // Run a game between a human and an AI player.
    //HumanPlayer playerX = HumanPlayer(PLAYER_X_CODE, PLAYER_X_MARK);
    //AIPlayerMinimax ai = AIPlayerMinimax(PLAYER_O_CODE, PLAYER_O_MARK, 5, &playerX);
    //playGame.play(playerX, ai);

    // Run a game between two minimax AI players.
    //AIPlayerMinimax ai1 = AIPlayerMinimax(PLAYER_X_CODE, PLAYER_X_MARK, 9, NULL);
    //AIPlayerMinimax ai2 = AIPlayerMinimax(PLAYER_O_CODE, PLAYER_O_MARK, 3, NULL);
    //ai1.opponent = &ai2;
    //ai2.opponent = &ai1;
    //playGame.play(ai1, ai2);

    // Run a game between two MCTS players.
    //AIPlayerMonteCarlo ai1 = AIPlayerMonteCarlo(PLAYER_X_CODE, PLAYER_X_MARK, 100, NULL);
    //AIPlayerMonteCarlo ai2 = AIPlayerMonteCarlo(PLAYER_O_CODE, PLAYER_O_MARK, 2, NULL);
    //ai1.opponent = &ai2;
    //ai2.opponent = &ai1;
    //playGame.play(ai1, ai2);

    // Command line inputs
    // Player O: -pO
    // Player X: -pX
    // Human player: -hp | --human
    // Minimax player: --mm | --minimax
    // Monte carlo player: --mc | --montecarlo
    // Example: -pO mc 10 -pX mc 20

    std::vector<std::string> inputs(argv, argv + argc);

    std::vector<std::string>::iterator helpLoc = std::find(inputs.begin(), inputs.end(), "-h");
    std::vector<std::string>::iterator pOLoc = std::find(inputs.begin(), inputs.end(), "-pO");
    std::vector<std::string>::iterator pXLoc = std::find(inputs.begin(), inputs.end(), "-pX");
    std::vector<std::string>::iterator pOTypeLoc = ++pOLoc;
    std::vector<std::string>::iterator pXTypeLoc = ++pXLoc;

    // Display help
    if(argc == 1 || helpLoc != inputs.end()) {
        std::cout << "Options:\n" 
                    << "Player O: -pO\n" 
                    << "Player X: -pX\n"
                    << "Human player: hp | human\n"
                    << "Minimax player: mm | minimax <tree depth>\n"
                    << "Monte carlo player: mc | montecarlo <iterations>\n"
                    << "Example: ./play -pO hp -pX mc 100" << std::endl;
    }
    else {
        // Strict inputs
        if(*pXTypeLoc == "hp" || *pXTypeLoc == "human") {
            playerX = new HumanPlayer(PLAYER_X_CODE, PLAYER_X_MARK);
        }
        else if(*pXTypeLoc == "mm" || *pXTypeLoc == "minimax") {
            playerX = new AIPlayerMinimax(PLAYER_X_CODE, PLAYER_X_MARK, std::stoi(*(++pXTypeLoc)));
        }
        else if(*pXTypeLoc == "mc" || *pXTypeLoc == "montecarlo") {
            playerX = new AIPlayerMonteCarlo(PLAYER_X_CODE, PLAYER_X_MARK, std::stoi(*(++pXTypeLoc)));
        }
        else {
            std::cout << "Error: Player X defined incorrectly." << std::endl;
        }

        if(*pOTypeLoc == "hp" || *pOTypeLoc == "human") {
            playerO = new HumanPlayer(PLAYER_O_CODE, PLAYER_O_MARK);
        }
        else if(*pOTypeLoc == "mm" || *pOTypeLoc == "minimax") {
            playerO = new AIPlayerMinimax(PLAYER_O_CODE, PLAYER_O_MARK, std::stoi(*(++pOTypeLoc)));
        }
        else if(*pOTypeLoc == "mc" || *pOTypeLoc == "montecarlo") {
            playerO = new AIPlayerMonteCarlo(PLAYER_O_CODE, PLAYER_O_MARK, std::stoi(*(++pOTypeLoc)));
        }
        else {
            std::cout << "Error: Player O defined incorrectly." << std::endl;
        }

        playGame.play(*playerX, *playerO);

        delete playerX;
        delete playerO;
    }

    return 0;
}
