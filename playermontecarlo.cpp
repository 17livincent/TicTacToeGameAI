/**
 *  @file playermontecarlo.cpp
 *  @author Vincent Li
 */

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <iterator>

#include "playermontecarlo.h"

MonteCarloTreeNode* createNode(bool player, char gameState[3][3], moveRCPair action, MonteCarloTreeNode* predecessor) {
    MonteCarloTreeNode* node = new MonteCarloTreeNode;
    node->player = player;
    copyGameState(gameState, node->gameState);
    node->action = action;
    node->predecessor = predecessor;
    node->untriedActions = getValidActions(node->gameState);
    //for(int i = 0; i < 9; i++) std::cout << node->gameState[(int)(i / 3)][i % 3];
    //std::cout << std::endl;

    return node;
}

moveRCPair lightPlayout(Player* player, char gameState[3][3]) {
    std::list<moveRCPair> possibleActions = getValidActions(gameState);
    // Randomly pick one
    srand(time(NULL));
    std::list<moveRCPair>::iterator it = possibleActions.begin();
    int randomIndex = rand() % possibleActions.size();
    for(int i = 0; i < randomIndex; i++) it++;
    moveRCPair nextAction = *it;

    return nextAction;
}

float ucb(MonteCarloTreeNode* node) {
    float value = 1;

    // Prevent divide by zero
    float ownNumOfVisits = (node->numOfVisits == 0) ? 0.0000001 : node->numOfVisits;
    float predNumOfVisits = (node->predecessor->numOfVisits == 0) ? 1 : node->predecessor->numOfVisits;
    value = ((node->numOfWins + node->numOfDraws) / (ownNumOfVisits)) + (std::sqrt(2) * std::sqrt(std::log(predNumOfVisits) / ownNumOfVisits));

    return value;
}

bool isTerminalNode(MonteCarloTreeNode* node) {
    bool result = false;

    int filled = 0;
    for(int i = 0; i < ROWS * COLS; i++) {
        if(node->gameState[(int)(i / 3)][i % 3] != CLEAR) filled++;
    }

    if(filled == ROWS * COLS || playerWins(PLAYER_X_MARK, node->gameState) || playerWins(PLAYER_O_MARK, node->gameState)) result = true;

    return result;
}

void deleteTree(MonteCarloTreeNode* root) {
    if(root != NULL) {
        for(MonteCarloTreeNode* successor : root->successors) {
            deleteTree(successor);
        }
    	root->predecessor = NULL;
        root->successors.clear();
        root->untriedActions.clear();
        delete root;
        root = NULL;
    }
}

void graftTree(MonteCarloTreeNode* root, MonteCarloTreeNode* graft) {
    if(root != graft && root != NULL) {
        for(MonteCarloTreeNode* successor : root->successors) {
            graftTree(successor, graft);
        }
    	root->predecessor = NULL;
        root->successors.clear();
        root->untriedActions.clear();
        delete root;
        root = NULL;
    }
    else if(root == graft) {
        // Remove predecessor pointer
        root->predecessor = NULL;
        // Keep the rest of the details, including action
    }
}

moveRCPair AIPlayerMonteCarlo::chooseMove(Game* game) {
    // Create root node from given game if this is the player's first move
    if(this->root == NULL) {
        // The player of this node is the one that just played
        bool currentPlayer = ((game->currentPlayer == PLAYER_X_CODE && this->code == PLAYER_X_CODE) 
                                || (game->currentPlayer == PLAYER_O_CODE && this->code == PLAYER_O_CODE)) ? OPPONENT : SELF;
        moveRCPair placeholder = std::make_pair(-1, -1);
        this->root = createNode(currentPlayer, game->board.grid, placeholder, NULL);
    }

    // Graft the tree in response to the opponent's move, if it happened
    // From root, find the successor in depth 1 that matches the game
    // If the tree is depth 0 (one node), then match is root
    MonteCarloTreeNode* match;
    if(root->successors.empty()) {
        match = root;
    }
    else {
        for(MonteCarloTreeNode* d1 : root->successors) {
            if(gameStatesAreEqual(d1->gameState, game->board.grid)) {
                match = d1;
                break;
            }
        }
    }
    // Graft tree and relabel the root
    graftTree(this->root, match);
    this->root = match;
    
    // Do MCTS for the given number of iterations
    for(int i = 0; i < this->iterations; i++) {
        //std::cout << i << std::endl;
        // Select a leaf
        MonteCarloTreeNode* leaf = this->selection(this->root, &ucb);
        if(!isTerminalNode(leaf)) {
            // Try expansion
            MonteCarloTreeNode* newNode = this->expansion(leaf);
            // Simulate
            int result = this->simulation(newNode, lightPlayout);
            //std::cout << "Simulated\n";
            // Backpropagate result
            this->backpropagation(newNode, result);
            //std::cout << "Recorded\n";
        }
    }

    // From the root, find the immediate child with the greatest promise and get its action.
    float max = -1;
    MonteCarloTreeNode* mostPromising;
    for(MonteCarloTreeNode* successor : this->root->successors) {
        // Prevent divide by zero
        float numOfVisits = (successor->numOfVisits == 0) ? 0.0000001 : successor->numOfVisits;
        float value = (2 * successor->numOfWins + successor->numOfDraws) / numOfVisits;
        //std::cout << successor->action.row << "," << successor->action.column << " " << value << std::endl;
        if(value > max) {
            max = value;
            mostPromising = successor;
        }
    }

    moveRCPair move;
    move.row = mostPromising->action.row;
    move.column = mostPromising->action.column;

    // Graft the tree in response to own action
    graftTree(this->root, mostPromising);
    this->root = mostPromising;

    return move;
}

int AIPlayerMonteCarlo::getNodeResult(MonteCarloTreeNode* node) {
    int result = 0;

    result = playerWins(this->mark, node->gameState) ? 1 : result;
    result = playerWins(this->opponent->mark, node->gameState) ? -1 : result;

    return result;
}

MonteCarloTreeNode* AIPlayerMonteCarlo::selection(MonteCarloTreeNode* root, float (*selectionFunction)(MonteCarloTreeNode*)) {
    MonteCarloTreeNode* node = root;

    while(!node->successors.empty()) {
        // Scan successors to find which one has the greatest promise
        float max = -1;
        MonteCarloTreeNode* mostPromising;
        for(MonteCarloTreeNode* successor : node->successors) {
            float value = selectionFunction(successor);
            if(value > max) {
		        //std::cout << value << std::endl;
                max = value;
                mostPromising = successor;
            }
        }
        node = mostPromising;
    }

    return node;
}

MonteCarloTreeNode* AIPlayerMonteCarlo::expansion(MonteCarloTreeNode* leaf) {
    MonteCarloTreeNode* newNode = NULL;
    //std::cout << newNode << std::endl;
    // If this is a terminal node, do nothing.
    if(!leaf->untriedActions.empty()) { // Otherwise,
	    // Expand all untried actions
	    bool nextPlayer = (leaf->player == SELF) ? OPPONENT : SELF;
	    for(moveRCPair untriedAction : leaf->untriedActions) {
    	    char nextGameState[3][3];
	        copyGameState(leaf->gameState, nextGameState);
	        nextGameState[untriedAction.row][untriedAction.column] = (nextPlayer == SELF) ? this->mark : this->opponent->mark;
	        newNode = createNode(nextPlayer, nextGameState, untriedAction, leaf);
	        leaf->successors.push_back(newNode);
	    }
	    leaf->untriedActions.clear();
    }

    // Randomly pick one of the new nodes
    srand(time(NULL));
    std::list<MonteCarloTreeNode*>::iterator it = leaf->successors.begin();
    int randomIndex = rand() % leaf->successors.size();
    for(int i = 0; i < randomIndex; i++) it++;
    MonteCarloTreeNode* picked = *it;

    return picked;
}

int AIPlayerMonteCarlo::simulation(MonteCarloTreeNode* node, moveRCPair (*playoutFunction)(Player* player, char gameState[3][3])) {
    int result = 0;

    MonteCarloTreeNode nodeCopy;
    copyGameState(node->gameState, nodeCopy.gameState);
    Player* currentPlayer = (node->player == false) ? this : this->opponent;

    while(!isTerminalNode(&nodeCopy)) {
        // Let the current player play a move
        moveRCPair move = lightPlayout(currentPlayer, nodeCopy.gameState);  // Pick a move
        nodeCopy.gameState[move.row][move.column] = currentPlayer->mark;    // Mark the game state
        // Switch players
        currentPlayer = (currentPlayer == this) ? this->opponent : this;
    }

    result = getNodeResult(&nodeCopy);

    return result;
}

void AIPlayerMonteCarlo::backpropagation(MonteCarloTreeNode* node, int result) {
    MonteCarloTreeNode* temp = node;

    while(temp != NULL) {
        // Update values
        temp->numOfVisits++;
        if(result == 0) temp->numOfDraws++;
        else if(result == -1) temp->numOfLosses++;
        else if(result == 1) temp->numOfWins++;

        // Move to the next predecessor
        temp = temp->predecessor;
    }
}