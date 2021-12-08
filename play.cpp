/**
 *  @file play.cpp
 *  @author Vincent Li
 *  The executed file.
 */

#include <iostream>

#include "play.h"
#include "game.h"

int Play::play(Player& playerX, Player& playerO) {
    int result;
    Game game;
    
    // game status
    int xWon = 0;
    int oWon = 0;
    int draw = 0;
    std::string input;
    Player* player;

    std::cout << "Welcome to a game of Tic-Tac-Toe!" << std::endl;

    while(xWon == 0 && oWon == 0 && draw == 0) {
        // Get current player
        player = (game.currentPlayer == playerO.code) ? &playerO : &playerX;

        std::cout << "Board:\n" << game.board.visBoard() << std::endl;
        std::cout << "Turn " << game.turns + 1 << std::endl;

        // Notify whose turn it is
        std::cout << "Player " << player->mark << " goes: " << std::endl;
        
        // Get valid player input
        moveRCPair move = player->chooseMove(&game);

        // Make the move
        game.playerMarks(player->mark, move.row, move.column);

        // Check game status
        xWon = game.playerWins(playerX.mark);
        oWon = game.playerWins(playerO.mark);
        draw = game.isDraw();
    }

    if(xWon) {
        std::cout << "Player X won!" << std::endl;
        result = PLAYER_X_WON;
    }
    else if(oWon) {
        std::cout << "Player O won!" << std::endl;
        result = PLAYER_O_WON;
    }
    else if(draw) {
        std::cout << "The game is a draw!" << std::endl;
        result = DRAW;
    }

    return result;
}

int main(int argc, char** argv) {
    Play playGame;

    // Run a game between two human players.
    //HumanPlayer playerX = HumanPlayer(PLAYER_X_CODE, PLAYER_X_MARK);
    //Player playerO = Player(PLAYER_O_CODE, PLAYER_O_MARK);
    //playGame.play(playerX, playerO);

    // Run a game between a human and an AI player.
    //HumanPlayer playerX = HumanPlayer(PLAYER_X_CODE, PLAYER_X_MARK);
    //AIPlayerMinimax ai = AIPlayerMinimax(PLAYER_O_CODE, PLAYER_O_MARK, 5, &playerX);
    //playGame.play(playerX, ai);

    // Run a game between two minimax AI players.
    AIPlayerMinimax ai1 = AIPlayerMinimax(PLAYER_X_CODE, PLAYER_X_MARK, 9, NULL);
    AIPlayerMinimax ai2 = AIPlayerMinimax(PLAYER_O_CODE, PLAYER_O_MARK, 3, NULL);
    ai1.opponent = &ai2;
    ai2.opponent = &ai1;
    playGame.play(ai1, ai2);
    /*
    // Loops to see the weird behaviors of the evaluation function due to search depths, in games between two minimax AIs.
    for(int ai1Depth = 1; ai1Depth <= 9; ai1Depth++) {
        for(int ai2Depth = 1; ai2Depth <= ai1Depth; ai2Depth++) {
            int wins = 0;
            int draws = 0;
            int losses = 0;
            int result;
            std::cout << "X depth: " << ai1Depth << ", O depth: " << ai2Depth << std::endl;
            for(int i = 0; i < 10; i++) {
                AIPlayerMinimax ai1 = AIPlayerMinimax(PLAYER_X_CODE, PLAYER_X_MARK, ai1Depth, NULL);
                AIPlayerMinimax ai2 = AIPlayerMinimax(PLAYER_O_CODE, PLAYER_O_MARK, ai2Depth, NULL);
                ai1.opponent = &ai2;
                ai2.opponent = &ai1;
                result = playGame.play(ai1, ai2);

                if(result == 1) wins++;
                else if(result == 0) losses++;
                else draws++;
            }
            
            std::cout << "Wins: " << wins << std::endl;
            std::cout << "Draws: " << draws << std::endl;
            std::cout << "Losses: " << losses << std::endl;

        }
    }
    */
   
    return 0;
}