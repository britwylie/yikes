/*
* Player class for Othello
* @author bwylie & tloverid
*/
//hello Zach
#include "player.hpp"
#include <iostream>
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     b = new Board();
     my_side = side;
     //do this better somehow, not sure if necessary
     other_side = (side == BLACK) ? WHITE : BLACK;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

     b->doMove(opponentsMove, this->other_side);

     if (!this->b->isDone()) {
       if(b->hasMoves(this->my_side)) {
         std::vector<Move*> all_moves = getMoves(this->my_side);
         Move* best = getBestMove(all_moves);
         b->doMove(best, this->my_side);
         return best;

       }
     }
     return nullptr;
}

/*
* returns a list of all possible moves for a particular side
*/
std::vector<Move*> Player::getMoves(Side s) {
  std::vector<Move*> all_moves;
  Move* possible = new Move(0, 0);
  for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
          possible = new Move(i, j);
          if (b->checkMove(possible, s)) {
            all_moves.push_back(possible);
          }
      }
  }
  return all_moves;
}

/*
* Uses a simple heuristic to sort through the possible moves and get the one
* that maximizes the score of the move
*/
Move *Player::getBestMove(std::vector<Move*> moves) {
  //sets up a test board
  Board *maybe = new Board();
  int score = -64;
  Move* bestMove = moves[0];
  //set up an array for the heuristic (not used yet)
  int multiplier[8][8] = {
      {3, -3, 3, 3, 3, 3, -3, 3},
      {-3, -3, 1, 1, 1, 1, -3, -3},
      {3, 1, 1, 1, 1, 1, 1, 3},
      {3, 1, 1, 1, 1, 1, 1, 3},
      {3, 1, 1, 1, 1, 1, 1, 3},
      {3, 1, 1, 1, 1, 1, 1, 3},
      {-3, -3, 1, 1, 1, 1, -3, -3},
      {3, -3, 3, 3, 3, 3, -3, 3},
  };
  //go through the vector of possible moves, finding the best move
  std::vector<Move*>::iterator i;
  for(i = moves.begin(); i != moves.end(); i++) {
     maybe = b->copy();
     maybe->doMove(*i, my_side);
     int temp = maybe->count(my_side) - maybe->count(other_side);
     //multiply by values to get modified possible score
     if((*i)->getY() == 0 || (*i)->getY() == 7 || (*i)->getX() == 0 || (*i)->getX() == 7) {
       temp *= 3;
     }
     if((*i)->getY() == 1 || (*i)->getY() == 6 || (*i)->getX() == 1 || (*i)->getX() == 6) {
       temp *= -3;
     }

     if (temp > score) {
       score = temp;
       bestMove = *i;
     }
  }
  return bestMove;
}
