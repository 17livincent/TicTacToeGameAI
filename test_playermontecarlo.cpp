/**
 * @file test_playermontecarlo.cpp
 * @author Vincent Li
 * Test functionalities of playermontecarlo.cpp.
 */

#include "playermontecarlo.h"


#include <iostream>
#include <assert.h>

void test_createNode() {
    char bb[3][3] = BLANK_BOARD;
    MonteCarloTreeNode* a = createNode(SELF, bb, std::make_pair(-1, -1), NULL, 0);
    assert(a->player == SELF);
    for(int i = 0; i < 9; i++) {
        assert(a->gameState[(int)(i / 3)][i % 3] == bb[(int)(i / 3)][i % 3]);
    }
    assert(a->action == std::make_pair(-1, -1));
    assert(a->predecessor == NULL);
    assert(a->successors.empty());
    assert(a->untriedActions.size() == 9);

    delete a;
}

void test_selection() {
    AIPlayerMonteCarlo playerX = AIPlayerMonteCarlo(PLAYER_X_CODE, PLAYER_X_MARK, 1, NULL);

    char bb[3][3] = BLANK_BOARD;
    MonteCarloTreeNode* a = createNode(SELF, bb, std::make_pair(-1, -1), NULL, 0);
    MonteCarloTreeNode* b = createNode(OPPONENT, bb, std::make_pair(-1, -1), a, 1);
    a->successors.push_back(b);
    assert(playerX.selection(a, &ucb) == b);

    delete a;
    delete b;
}

void test_expansion() {
    AIPlayerMonteCarlo playerX = AIPlayerMonteCarlo(PLAYER_X_CODE, PLAYER_X_MARK, 1, NULL);
    AIPlayerMonteCarlo playerO = AIPlayerMonteCarlo(PLAYER_O_CODE, PLAYER_O_MARK, 1, NULL);
    playerX.opponent = &playerO;
    
    char bb[3][3] = BLANK_BOARD;

    MonteCarloTreeNode* a = createNode(SELF, bb, std::make_pair(-1, -1), NULL, 0);

    playerX.expansion(a);
    std::cout << "A" << std::endl;
    assert(a->untriedActions.size() == 0);
    std::cout << "A" << std::endl;
    assert(a->successors.size() == 9);
    std::cout << "A" << std::endl;
    assert(a->successors.front()->player != a->player);
    std::cout << "A" << std::endl;
    assert(a->successors.front()->predecessor == a);
    std::cout << "A" << std::endl;
    assert(a->successors.front()->untriedActions.size() == 8);
    std::cout << "A" << std::endl;
    delete a;
    std::cout << "A" << std::endl;
}

void test_isTerminalNode() {
    char bb[3][3] = BLANK_BOARD;
    MonteCarloTreeNode* a = createNode(SELF, bb, std::make_pair(-1, -1), NULL, 0);
    assert(isTerminalNode(a) == false);

    char fullBoard[3][3] = {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'};
    MonteCarloTreeNode* b = createNode(SELF, fullBoard, std::make_pair(-1, -1), NULL, 0);
    assert(isTerminalNode(b) == true);

    char partialBoard[3][3] = {'X', 'X', 'X', 'X', 'X', 'X', CLEAR, CLEAR, CLEAR};
    MonteCarloTreeNode* c = createNode(SELF, partialBoard, std::make_pair(-1, -1), NULL, 0);
    assert(isTerminalNode(c) == true);

    delete a;
    delete b;
    delete c;
}

void test_getNodeResult() {
    AIPlayerMonteCarlo playerX = AIPlayerMonteCarlo(PLAYER_X_CODE, PLAYER_X_MARK, 1, NULL);
    AIPlayerMonteCarlo playerO = AIPlayerMonteCarlo(PLAYER_O_CODE, PLAYER_O_MARK, 1, NULL);
    playerX.opponent = &playerO;

    char win[3][3] = {PLAYER_X_MARK, PLAYER_X_MARK, PLAYER_X_MARK, PLAYER_X_MARK, PLAYER_X_MARK, PLAYER_X_MARK, PLAYER_X_MARK, PLAYER_X_MARK, PLAYER_X_MARK};
    MonteCarloTreeNode* a = createNode(SELF, win, std::make_pair(-1, -1), NULL, 0);
    assert(playerX.getNodeResult(a) == 1);

    char loss[3][3] = {PLAYER_O_MARK, PLAYER_O_MARK, PLAYER_O_MARK, PLAYER_O_MARK, PLAYER_O_MARK, PLAYER_O_MARK, PLAYER_O_MARK, PLAYER_O_MARK, PLAYER_O_MARK};
    MonteCarloTreeNode* b = createNode(SELF, loss, std::make_pair(-1, -1), NULL, 0);
    assert(playerX.getNodeResult(b) == -1);

    char draw[3][3] = {PLAYER_O_MARK, PLAYER_X_MARK, PLAYER_O_MARK, PLAYER_O_MARK, PLAYER_X_MARK, PLAYER_O_MARK, PLAYER_X_MARK, PLAYER_O_MARK, PLAYER_X_MARK};
    MonteCarloTreeNode* c = createNode(SELF, draw, std::make_pair(-1, -1), NULL, 0);
    assert(playerX.getNodeResult(c) == 0);

    delete a;
    delete b;
    delete c;
}

void test_simulation() {
    AIPlayerMonteCarlo playerX = AIPlayerMonteCarlo(PLAYER_X_CODE, PLAYER_X_MARK, 1, NULL);
    AIPlayerMonteCarlo playerO = AIPlayerMonteCarlo(PLAYER_O_CODE, PLAYER_O_MARK, 1, NULL);
    playerX.opponent = &playerO;

    char willWin[3][3] = {PLAYER_X_MARK, CLEAR, PLAYER_X_MARK, CLEAR, CLEAR, CLEAR, PLAYER_X_MARK, CLEAR, PLAYER_X_MARK};
    MonteCarloTreeNode* a = createNode(OPPONENT, willWin, std::make_pair(-1, -1), NULL, 0);
    assert(playerX.simulation(a, &lightPlayout) == 1);

    char willLose[3][3] = {PLAYER_O_MARK, CLEAR, PLAYER_O_MARK, CLEAR, CLEAR, CLEAR, PLAYER_O_MARK, CLEAR, PLAYER_O_MARK};
    MonteCarloTreeNode* b = createNode(OPPONENT, willLose, std::make_pair(-1, -1), NULL, 0);
    assert(playerX.simulation(b, &lightPlayout) == -1);

    char willDraw[3][3] = {PLAYER_X_MARK, PLAYER_O_MARK, PLAYER_X_MARK, PLAYER_X_MARK, PLAYER_O_MARK, PLAYER_X_MARK, PLAYER_O_MARK, CLEAR, PLAYER_O_MARK};
    MonteCarloTreeNode* c = createNode(OPPONENT, willDraw, std::make_pair(-1, -1), NULL, 0);
    assert(playerX.simulation(c, &lightPlayout) == 0);

    delete a;
    delete b;
    delete c;
}

void test_backpropagation() {
    char bb[3][3] = BLANK_BOARD;
    MonteCarloTreeNode* a = createNode(OPPONENT, bb, std::make_pair(-1, -1), NULL, 0);
    MonteCarloTreeNode* b = createNode(SELF, bb, std::make_pair(-1, -1), a, 1);

    AIPlayerMonteCarlo playerX = AIPlayerMonteCarlo(PLAYER_X_CODE, PLAYER_X_MARK, 1, NULL);
    b->minSimMovesToWin = 5;
    playerX.backpropagation(b, 1);
    assert(b->numOfVisits == 1);
    assert(b->numOfWins == 1);
    assert(a->numOfVisits == 1);
    assert(a->numOfWins == 1);
    assert(a->minSimMovesToWin == 5);

    delete a;
    delete b;
}

void test_chooseMove() {
    AIPlayerMonteCarlo playerX = AIPlayerMonteCarlo(PLAYER_X_CODE, PLAYER_X_MARK, 100, NULL);
    AIPlayerMonteCarlo playerO = AIPlayerMonteCarlo(PLAYER_O_CODE, PLAYER_O_MARK, 1, NULL);
    playerX.opponent = &playerO;
    playerO.opponent = &playerX;

    Game game;

    moveRCPair move = playerX.chooseMove(&game);
    std::cout << move.row << " " << move.column << std::endl;
}

int main(int argc, char** argv) {
    test_createNode();
    test_selection();
    test_expansion();
    test_isTerminalNode();
    test_getNodeResult();
    test_simulation();
    test_backpropagation();
    test_chooseMove();

    return 0;
}
