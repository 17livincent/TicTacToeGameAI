/**
 *  @file playermontecarlo.cpp
 *  @author Vincent Li
 */

#include "defines.h"

#if defined(MINIMAL_VERBOSE) || defined(VERBOSE) || defined(DEBUG)
#include <iostream>
#endif  // defined(MINIMAL_VERBOSE) || defined(VERBOSE) || defined(DEBUG)

#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <iterator>

#include "playermontecarlo.h"

MonteCarloTreeNode* createNode(bool player, char gameState[3][3], moveRCPair action, MonteCarloTreeNode* predecessor, int depth) {
    MonteCarloTreeNode* node = new MonteCarloTreeNode;
    node->player = player;
    copyGameState(gameState, node->gameState);
    node->action = action;
    node->depth = depth;
    node->predecessor = predecessor;
    node->untriedActions = getValidActions(node->gameState);

    return node;
}

moveRCPair lightPlayout(char player, char gameState[3][3]) {
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
        root->successors.clear();
        root->untriedActions.clear();

        delete root;
        root = NULL;
    }
}

void createScion(MonteCarloTreeNode* root, MonteCarloTreeNode* keep) {
    if(root != NULL && root != keep) {
        for(MonteCarloTreeNode* successor : root->successors) {
            createScion(successor, keep);
        }
        root->successors.clear();
        root->untriedActions.clear();

        delete root;
        root = NULL;
    }
    else if(root == keep) {
        root->predecessor = NULL;
    }
}

moveRCPair AIPlayerMonteCarlo::chooseMove(Game* game) {
    moveRCPair move;

    // Init the tree if it doesn't exist yet
    if(this->tree == NULL) {
        // Create root node from given game
        // The player of this node is the one that just played
        bool currentPlayer = ((game->currentPlayer == PLAYER_X_CODE && this->code == PLAYER_X_CODE) 
                                || (game->currentPlayer == PLAYER_O_CODE && this->code == PLAYER_O_CODE)) ? OPPONENT : SELF;
        moveRCPair placeholder = std::make_pair(-1, -1);
        this->tree = createNode(currentPlayer, game->board.grid, placeholder, NULL, 0);
    }

    // Find the current game state in the tree
    // Update the game tree so that the root is the current game state
    if(gameStatesAreEqual(this->tree->gameState, game->board.grid)) {    // If the root already matches the current game state (first play)
        // Do nothing
    }
    else {  // If the root does not match the current game state
        // Check if the current node has immediate successors
        bool hasSuccessors = !this->tree->successors.empty();

        // If the root has no successors (the node of the previous action was unexpanded and not very promising), do one expansion of the root
        if(!hasSuccessors) {
            this->expansion(this->tree);
        }

        // Find it in the immediate successors
        MonteCarloTreeNode* match = NULL;
        for(MonteCarloTreeNode* s : this->tree->successors) {
            if(gameStatesAreEqual(game->board.grid, s->gameState)) {
                match = s;
                break;
            }
        }
        createScion(this->tree, match);
        this->tree = match;
    }

    // Do MCTS for the given number of iterations
    for(int i = 0; i < this->iterations; i++) {
        // Select a leaf
        MonteCarloTreeNode* leaf = this->selection(this->tree, &ucb);
        // Try expansion
        MonteCarloTreeNode* newNode = this->expansion(leaf);
        // Simulate or get terminal node result
        int result = this->simulation(newNode, lightPlayout);
        // Backpropagate result
        this->backpropagation(newNode, result);
    }
    // From the root, find the immediate child with the greatest promise and get its action.
    float max = -1;
    MonteCarloTreeNode* mostPromising = this->tree;
    for(MonteCarloTreeNode* successor : this->tree->successors) {
        // Prevent divide by zero
        float numOfVisits = (successor->numOfVisits == 0) ? 0.0000001 : successor->numOfVisits;
        // Calculate value for best action: (1/sqrt(min of simulated moves to win)) * (2*numOfWins + numOfDraws) / numOfVisits
        float value = (1 / std::sqrt(successor->minSimMovesToWin)) * (2 * successor->numOfWins + successor->numOfDraws) / numOfVisits;
#if defined(DEBUG)
        std::cout << "Action: " << successor->action.row << "," << successor->action.column << "\tValue: " << value << "\tMin exp moves to win: " << successor->minSimMovesToWin << "\tVisits: " << successor->numOfVisits << std::endl;    
#endif  // defined(VERBOSE) || defined(DEBUG)
        if(value > max) {
            max = value;
            mostPromising = successor;
        }
    }
    move.row = mostPromising->action.row;
    move.column = mostPromising->action.column;
#if defined(DEBUG)
    std::cout << "Root visits: " << this->tree->numOfVisits << std::endl;
#endif  // DEBUG
    // Move the root to the most promising node and delete the rest
    createScion(this->tree, mostPromising);
    this->tree = mostPromising;
#if defined(VERBOSE) || defined(DEBUG)
    std::cout << "\tFound optimal move: " << move.row << ", " << move.column << " of value " << max << std::endl;
#elif defined(MINIMAL_VERBOSE)
    std::cout << game->turns << " " << this->mark << ":" << move.row << "," << move.column << std::endl;
#endif
    return move;
}

int AIPlayerMonteCarlo::getNodeResult(MonteCarloTreeNode* node) {
    int result = 0;

    result = playerWins(this->mark, node->gameState) ? 1 : result;
    result = playerWins(this->opponentMark, node->gameState) ? -1 : result;

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
                max = value;
                mostPromising = successor;
            }
        }
        node = mostPromising;
    }

    return node;
}

MonteCarloTreeNode* AIPlayerMonteCarlo::expansion(MonteCarloTreeNode* leaf) {
    MonteCarloTreeNode* newNode = leaf;
    // If this is a terminal node, do nothing.
    if(!leaf->untriedActions.empty()) { // Otherwise,
	    // Expand all untried actions
	    bool nextPlayer = (leaf->player == SELF) ? OPPONENT : SELF;
	    for(moveRCPair untriedAction : leaf->untriedActions) {
    	    char nextGameState[3][3];
	        copyGameState(leaf->gameState, nextGameState);
	        nextGameState[untriedAction.row][untriedAction.column] = (nextPlayer == SELF) ? this->mark : this->opponentMark;
	        newNode = createNode(nextPlayer, nextGameState, untriedAction, leaf, leaf->depth++);
	        leaf->successors.push_back(newNode);
	    }
	    leaf->untriedActions.clear();

        // If successors were generated, randomly pick one of the new nodes
        srand(time(NULL));
        std::list<MonteCarloTreeNode*>::iterator it = leaf->successors.begin();
        int randomIndex = rand() % leaf->successors.size();
        for(int i = 0; i < randomIndex; i++) it++;
        newNode = *it;
    }

    return newNode;
}

int AIPlayerMonteCarlo::simulation(MonteCarloTreeNode* node, moveRCPair (*playoutFunction)(char player, char gameState[3][3])) {
    int result = 0;
    int moves = 1;

    if(isTerminalNode(node)) {  // If this is a terminal node
        // Return the actual result
        result = getNodeResult(node);
    }
    else {
        MonteCarloTreeNode nodeCopy;
        copyGameState(node->gameState, nodeCopy.gameState);
       char currentPlayer = (node->player == false) ? this->mark : this->opponentMark;

        while(!isTerminalNode(&nodeCopy)) {
            moves++;
            // Let the current player play a move
            moveRCPair move = lightPlayout(currentPlayer, nodeCopy.gameState);  // Pick a move
            nodeCopy.gameState[move.row][move.column] = currentPlayer;    // Mark the game state
            // Switch players
            currentPlayer = (currentPlayer == this->mark) ? this->opponentMark : this->mark;
        }

        result = getNodeResult(&nodeCopy);
    }

    // If the result is a win, update minSimMovesToWin
    if(result == 1) node->minSimMovesToWin = moves;

    return result;
}

void AIPlayerMonteCarlo::backpropagation(MonteCarloTreeNode* node, int result) {
    MonteCarloTreeNode* temp = node;
    int minSimMovesToWin = node->minSimMovesToWin;

    while(temp != NULL) {
        // Update values
        temp->numOfVisits++;
        if(result == 0) temp->numOfDraws++;
        else if(result == -1) temp->numOfLosses++;
        else if(result == 1) temp->numOfWins++;

        if(result == 1 && temp->minSimMovesToWin > minSimMovesToWin) {
            temp->minSimMovesToWin = minSimMovesToWin;
        }

        // Move to the next predecessor
        temp = temp->predecessor;
    }
}
