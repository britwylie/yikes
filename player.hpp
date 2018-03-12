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
<<<<<<< HEAD
    Move *recurseMiniMax(std::vector<Move*> moves);
    Move *miniMax(std::vector<Move*> moves);
=======
    Move *miniMax(std::vector<Move*> moves, time_t oldtimer);
    Move *recursiveMiniMax(Board *board, int depth, int score, Move *move, time_t oldtimer);
>>>>>>> 3c77500fcb1cc00e9acadf7edb6471960b519901
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
