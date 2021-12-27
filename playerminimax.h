/**
 *  @file playerminimax.h
 *  @author Vincent Li
 *  Implements an AI player that uses the Minimax algorithm.
 */

#pragma once
#ifndef AIPLAYERMINIMAX
#define AIPLAYERMINIMAX

#include <list>

#include "player.h"

#define MAXPLAYER true
#define MINPLAYER false

// A node in a minimax search tree.
struct MinimaxTreeNode {
    int player = -1;    // -1 by default.  Given value MAXPLAYER or MINPLAYER
    char gameState[3][3] = BLANK_BOARD;  // A 3x3 array like the game board.
    moveRCPair action;  // The action that lead to the game state in this node.
    std::list<MinimaxTreeNode*> successors;   // A list of pointers to child nodes.
};

class AIPlayerMinimax: public Player {
    public:
        // Max depth of search.  0 for the limit, which is 9.
        int depthLimit = 0;
        // The opponent
        Player* opponent;
        // A handy variable to hold the number of nodes in the minimax tree
        int treeSize = 0;

        // Constructor
        AIPlayerMinimax(int code, char mark, int depthLimit, Player* opponent): Player(code, mark) {
            this->depthLimit = (depthLimit <= 0) ? 9 : depthLimit;
            this->opponent = opponent;

            // Introduction
#if defined(VERBOSE)
            std::cout << "Introducing Player " << this->mark << ", who is a Minimax algorithm AI of search depth " << this->depthLimit << std::endl;
#endif  // VERBOSE
        };

        ~AIPlayerMinimax() {
            this->depthLimit = 0;
            this->opponent = NULL;
            this->treeSize = 0;
        }

        // Use minimax and a game tree to choose the best move.
        moveRCPair chooseMove(Game* game);

        /**
         * Perform minimax search on the game tree of the given root and depth.
         * Uses alpha-beta pruning.
         * Returns a pair containing the optimal move and its heuristic value.
         */
        std::pair<moveRCPair, int> minimaxSearch(MinimaxTreeNode* node, int depth, int alpha, int beta, bool maxPlayer, moveRCPair action);

        /**
         * Return a heuristic based on the node's game state.
         * Could always be better.
         */
        int evalFunction(MinimaxTreeNode* node);

        /**
         * Create a game tree of the given depth (layers = 2 * depth) 
         * and return the root node.  @param action is the initial action, 
         * and @param gameState is the initial game state.
         */
        MinimaxTreeNode* createGameTree(moveRCPair action, char gameState[3][3], int layer);

        /**
         * Delete the game tree.
         */
        void deleteTree(MinimaxTreeNode* root);

        void postOrderTraversal(MinimaxTreeNode* root, int layer);
};


#endif  // AIPLAYERMINIMAX