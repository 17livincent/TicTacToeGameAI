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

    int depth = 0;  // The depth of this node relative to the root

    MonteCarloTreeNode* predecessor; // A pointer to the predecessor node.
    std::list<MonteCarloTreeNode*> successors;  // Pointers to the successor nodes.

    std::list<moveRCPair> untriedActions;  // A list of r/c pairs of the unexplored actions.

    int numOfVisits = 0;
    int numOfWins = 0;
    int numOfLosses = 0;
    int numOfDraws = 0;
    // The minimum number of moves for this or a descendant to simulate a win.
    // Used to weight node that lead to quicker wins.
    int minSimMovesToWin = INT32_MAX;
};

/**
 * Create a node with the given @param player, @param gameState, @param action, @param predecessor, and @param depth.
 * The untriedActions member is created from the @param gameState.
 */
MonteCarloTreeNode* createNode(bool player, char gameState[3][3], moveRCPair action, MonteCarloTreeNode* predecessor, int depth);

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

/**
 * @brief Delete the tree from the given root except for the node at keep and its descendents.
 * 
 * @param root The root of the game tree
 * @param keep The root of the subtree to avoid deleting
 */
void createScion(MonteCarloTreeNode* root, MonteCarloTreeNode* keep);

class AIPlayerMonteCarlo: public Player {
    public:
        // The number of iterations to run MCTS.
        int iterations = 0;

        // Game tree root
        MonteCarloTreeNode* tree = NULL;

        // The opponent.
        Player* opponent;

        AIPlayerMonteCarlo(int code, int mark, int iterations, Player* opponent): Player(code, mark) {
            this->iterations = iterations;
            this->opponent = opponent;

            // Introduction
#if defined(VERBOSE) || defined(DEBUG)
            std::cout << "Introducing Player " << this->mark << ", who is a Monte Carlo ST AI of " << this->iterations << " iterations" << std::endl;
#endif  // defined(VERBOSE) || defined(DEBUG)
        }

        ~AIPlayerMonteCarlo() {
            deleteTree(tree);
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
         * From the @param root node, traverse down the tree to find a leaf node with no successors.
         * Does not care if the found node is a terminal node (win/draw/loss).
         * The selection function could be ucb(), to rate each child node.
         */
        MonteCarloTreeNode* selection(MonteCarloTreeNode* root, float (*selectionFunction)(MonteCarloTreeNode*));

        /**
         * Fully expands the given leaf node if possible.
         * Returns a random child node if at least one was created.
         * If expansion wasn't possible (terminal node), return the given leaf node.
         */
        MonteCarloTreeNode* expansion(MonteCarloTreeNode* leaf);

        /**
         * Performs simulation/playout/rollout from the given node.
         * A playout function is passed for light/heavy playout.
         * Returns an int representing the result.
         * Returns the result if the given node is a terminal node.
         */
        int simulation(MonteCarloTreeNode* node, moveRCPair (*playoutFunction)(Player* player, char gameState[3][3]));

        /**
         * Updates all preceding nodes to the root with the given result from simulation().
         */
        void backpropagation(MonteCarloTreeNode* node, int result);
};

#endif  // AIPLAYERMONTECARLO