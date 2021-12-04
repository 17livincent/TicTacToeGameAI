/**
 *  @file playerminimax.cpp
 *  @author Vincent Li
 */

#include "playerminimax.h"
#include <iostream>

moveRCPair AIPlayerMinimax::chooseMove(Game* game) {
    this->treeSize = 0;
    // Create game tree
    moveRCPair initialAction;
    MinimaxTreeNode* gameTree = createGameTree(initialAction, game->board.grid, this->depthLimit * 2);
    std::cout << "\tMinimax AI created game tree of size " << treeSize << std::endl;
    // Perform minimax search and get the best move
    std::pair<moveRCPair, int> minimax = minimaxSearch(gameTree, this->depthLimit * 2, true, initialAction);
    moveRCPair optAction = minimax.first;
    std::cout << "\tFound optimal move: " << optAction.row << ", " << optAction.column << " of value " << minimax.second << std::endl;
    // Delete the game tree
    deleteTree(gameTree);

    return optAction;
}

std::pair<moveRCPair, int> AIPlayerMinimax::minimaxSearch(MinimaxTreeNode* node, int depth, bool maxPlayer, moveRCPair action) {
    if(depth == 0 || node->successors.size() == 0) {
        return std::make_pair(action, evalFunction(node));
    }

    if(maxPlayer) { // player is self
        moveRCPair localAction = action; // track optimal action
        int maxValue = -1000;  // negative infinity
        //std::cout << node->successors.size() << std::endl;
        for(MinimaxTreeNode* successor : node->successors) {
            std::pair<moveRCPair, int> temp = minimaxSearch(successor, depth - 1, false, action);
            if(temp.second > maxValue) {
                maxValue = temp.second;
                localAction = successor->action;
            }
        }
        return std::make_pair(localAction, maxValue);
    }
    else {  // player is opponent
        moveRCPair localAction = action; // track optimal action
        int minValue = 1000;  // infinity
        //std::cout << node->successors.size() << std::endl;
        for(MinimaxTreeNode* successor : node->successors) {
            std::pair<moveRCPair, int> temp = minimaxSearch(successor, depth - 1, true, action);
            if(temp.second < minValue) {
                minValue = temp.second;
                localAction = successor->action;
            }
        }
        return std::make_pair(localAction, minValue);
    }
}

int AIPlayerMinimax::evalFunction(MinimaxTreeNode* node) {
    /**
     * Heuristic = 5X3(s) + 3X2(s) + X1(s) - (7O3(s) + 3O2(s) + O1(s)) 
     * Xn is the number of rows, columns, or diagonals with just n X's.
     * On is the number of rows, columns, or diagonals with just n O's.
     * In this case, X is this player's mark.
     */

    int own3 = 0;
    int own2 = 0;
    int own1 = 0;
    int opp3 = 0;
    int opp2 = 0;
    int opp1 = 0;

    // Iterate rows
    for(int r = 0; r < 3; r++) {
        int ownCount = 0;
        int oppCount = 0;
        for(int c = 0; c < 3; c++) {
            if(node->gameState[r][c] == this->mark) ownCount++;
            else if(node->gameState[r][c] == this->opponent->mark) oppCount++;
        }
        if(ownCount == 0) {
            if(oppCount == 1) opp1++;
            else if(oppCount == 2) opp2++;
            else if(oppCount == 3) opp3++;
        }
        if(oppCount == 0) {
            if(ownCount == 1) own1++;
            else if(ownCount == 2) own2++;
            else if(ownCount == 3) own3++;
        }
    }

    // Iterate columns
    for(int c = 0; c < 3; c++) {
        int ownCount = 0;
        int oppCount = 0;
        for(int r = 0; r < 3; r++) {
            if(node->gameState[r][c] == this->mark) ownCount++;
            else if(node->gameState[r][c] == this->opponent->mark) oppCount++;
        }
        if(ownCount == 0) {
            if(oppCount == 1) opp1++;
            else if(oppCount == 2) opp2++;
            else if(oppCount == 3) opp3++;
        }
        if(oppCount == 0) {
            if(ownCount == 1) own1++;
            else if(ownCount == 2) own2++;
            else if(ownCount == 3) own3++;
        }
    }

    // Check backwards diagnol
    int ownCount = 0;
    int oppCount = 0;
    for(int d = 0; d < 3; d++) {
        if(node->gameState[d][d] == this->mark) ownCount++;
        else if(node->gameState[d][d] == this->opponent->mark) oppCount++;
    }
    if(ownCount == 0) {
        if(oppCount == 1) opp1++;
        else if(oppCount == 2) opp2++;
        else if(oppCount == 3) opp3++;
    }
    if(oppCount == 0) {
        if(ownCount == 1) own1++;
        else if(ownCount == 2) own2++;
        else if(ownCount == 3) own3++;
    }

    // Check forward diagnol
    ownCount = 0;
    oppCount = 0;
    for(int d = 0; d < 3; d++) {
        if(node->gameState[d][COLS - 1 - d] == this->mark) ownCount++;
        else if(node->gameState[d][COLS - 1 - d] == this->opponent->mark) oppCount++;
    }
    if(ownCount == 0) {
        if(oppCount == 1) opp1++;
        else if(oppCount == 2) opp2++;
        else if(oppCount == 3) opp3++;
    }
    if(oppCount == 0) {
        if(ownCount == 1) own1++;
        else if(ownCount == 2) own2++;
        else if(ownCount == 3) own3++;
    }

    int h = 5 * own3 + 3 * own2 + own1 - (7 * opp3 + 3 * opp2 + opp1);

    return h;
}

MinimaxTreeNode* AIPlayerMinimax::createGameTree(moveRCPair action, char gameState[3][3], int layer) {
    this->treeSize++;
    // Create a new node
    MinimaxTreeNode* node = new MinimaxTreeNode;
    node->player = (layer % 2 == 0) ? MAXPLAYER : MINPLAYER;
    node->action = action;
    // Set node's game state to the given one.
    for(int i = 0; i < 9; i++) {
        node->gameState[int(i / 9)][i % 9] = gameState[int(i / 9)][i % 9];
    }
    if(layer == 0) {    // final layer, so stop
        return node;
    }
    else {  // Generate successors
        // If layer is even or 0, then the current player is this, next player is the opponent
        // If the layer is odd, then the current palyer is opponent, next player is this player
        Player* currentPlayer = (layer % 2 == 0) ? this : this->opponent;
        
        // Create nodes for each action of the current player
        // If validActions is empty, then no successors are created
        std::list<moveRCPair> validActions = currentPlayer->getValidActions(gameState);
        for(moveRCPair move : validActions) {
            // Copy game state
            char nextGameState[3][3];
            for(int i = 0; i < 9; i++) {
                nextGameState[int(i / 9)][i % 9] = gameState[int(i / 9)][i % 9];
            }
            // Create new game state
            nextGameState[move.row][move.column] = currentPlayer->mark;
            MinimaxTreeNode* successor = createGameTree(move, nextGameState, layer - 1);
            node->successors.push_back(successor);
        }

        return node;
    }
}

void AIPlayerMinimax::deleteTree(MinimaxTreeNode* root) {
    if(root != NULL) {
        for(MinimaxTreeNode* successor : root->successors) {
            deleteTree(successor);
        }
        delete root;
        root = NULL;
    }
}

void AIPlayerMinimax::postOrderTraversal(MinimaxTreeNode* root, int layer) {
    if(root != NULL) {
        for(MinimaxTreeNode* successor : root->successors) {
            postOrderTraversal(successor, layer + 1);
        }
        std::cout << "Layer: " << layer << ", Player: " << root->player << ", Node: " << root->action.row << "," << root->action.column << std::endl;   
    }
}