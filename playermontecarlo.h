/**
 * @file playermontecarlo.h
 * @author Vincent Li
 * Implements an AI player that uses Monte Carlo Tree Search.
 */

#pragma once
#ifndef AIPLAYERMONTECARLO
#define AIPLAYERMONTECARLO

#include "player.h"
#include "game.h"

#define SELF true
#define OPPONENT false

struct MonteCarloTreeNode {
    int player = -1;    // -1 by default.  Given value SELF or OPPONENT
    char gameState[3][3] = BLANK_BOARD; // The game state.
    moveRCPair action;  // The action that lead to this state.

    MonteCarloTreeNode* predecessor; // A pointer to the predecessor node.
    std::list<MonteCarloTreeNode*> successors;  // Pointers to the successor nodes.

    std::list<moveRCPair> untriedActions;  // A list of r/c pairs of the unexplored actions.

    int numOfVisits = 0;
    int numOfWins = 0;
    int numOfLosses = 0;
    int numOfDraws = 0;
};

/**
 * Create a node with the given @param player, @param gameState, @param action, and @param predecessor.
 * The untriedActions member is created from the @param gameState.
 */
MonteCarloTreeNode* createNode(bool player, char gameState[3][3], moveRCPair action, MonteCarloTreeNode* predecessor);

/**
 * Performs light playout which returns a random valid move from the given player and game state.
 */
moveRCPair lightPlayout(Player* player, char gameState[3][3]);

/**
 * Return the upper confidence bound value of the given node.
 */
float ucb(MonteCarloTreeNode* node);

/**
 * Returns if the node corresponds to a win/loss/draw.
 */
bool isTerminalNode(MonteCarloTreeNode* node);

/**
 * Delete all nodes of the search tree with the given @param root.
 */
void deleteTree(MonteCarloTreeNode* root);

class AIPlayerMonteCarlo: public Player {
    public:
        // The number of iterations to run MCTS.
        int iterations = 0;

        // The opponent.
        Player* opponent;

        AIPlayerMonteCarlo(int code, int mark, int iterations, Player* opponent): Player(code, mark) {
            this->iterations = iterations;
            this->opponent = opponent;

            // Introduction
            std::cout << "Introducing Player " << this->mark << ", who is a Monte Carlo ST AI of " << this->iterations << " iterations" << std::endl;
        }

        /**
         * Creates a game tree and uses Monte Carlo Tree Search (offline) to pick the best move.
         */
        virtual moveRCPair chooseMove(Game* game);

        /**
         * Returns -1/0/1 if the given grid corresponds to a loss/draw/win.
         * isTerminalNode() must be used before this.
         */
        int getNodeResult(MonteCarloTreeNode* node);

        /**
         * From the @param root node, traverse down the tree to find a leaf node with no history of simulation (aka no successors).
         * Does not care if the found node is a terminal node (win/draw/loss).
         * The selection function could be ucb(), to rate each child node.
         */
        MonteCarloTreeNode* selection(MonteCarloTreeNode* root, float (*selectionFunction)(MonteCarloTreeNode*));

        /**
         * Expands the given leaf node by one randomly-picked untried action.
         * Returns the new child node.
         */
        MonteCarloTreeNode* expansion(MonteCarloTreeNode* leaf);

        /**
         * Performs simulation/playout/rollout from the given node.
         * A playout function is passed for light/heavy playout.
         * Returns an int representing the result.
         */
        int simulation(MonteCarloTreeNode* node, moveRCPair (*playoutFunction)(Player* player, char gameState[3][3]));

        /**
         * Updates all preceding nodes to the root with the given result from simulation().
         */
        void backpropagation(MonteCarloTreeNode* node, int result);
};

#endif  // AIPLAYERMONTECARLO