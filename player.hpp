#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <vector>
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    Board* b;
    Side my_side;
    Side other_side;


    Move *doMove(Move *opponentsMove, int msLeft);
    Move *getBestMove(vector<Move*> moves);
    std::vector<Move*> getMoves(Side s);

    Move *miniMax(std::vector<Move*> moves);

    int recursiveMiniMax(Board *board, Side s, Move*& best, int depth);
    int getFrontierSquares(Side s, Board* b);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

    //to keep track of the move associated with the best score produced by negaMax
    Move* negaMaxMove;

    int negaMax(int depth, int alpha, int beta);
};

#endif
