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
        if(game.currentPlayer == playerO.code) player = &playerO;
        else if(game.currentPlayer == playerX.code) player = &playerX;

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
    if(oWon) {
        std::cout << "Player O won!" << std::endl;
        result = PLAYER_O_WON;
    }
    if(draw) {
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
    //int result = playGame.play(playerX, playerO);

    // Run a game between a human and an AI player.
    HumanPlayer playerX = HumanPlayer(PLAYER_X_CODE, PLAYER_X_MARK);
    AIPlayerMinimax ai = AIPlayerMinimax(PLAYER_O_CODE, PLAYER_O_MARK, 9, &playerX);
    playGame.play(playerX, ai);

    return 0;
}