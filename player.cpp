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
         Move* best;
         //getBestMove uses the heuristic, comment it out and uncomment the next line to use Minimax instead
         //Move* best = getBestMove(all_moves);
         /*if (testingMinimax == true) {
           best = miniMax(all_moves);
         } else {
           time_t timer;
           time(&timer);

           int depth = 13;
           int score = this->b->count(my_side) - this->b->count(other_side);
           best = recursiveMiniMax(this->b, depth, score, all_moves[0], timer);
         }*/

         negaMax(-64, 64, 5);
         best = negaMaxMove;

         b->doMove(best, this->my_side);
         return best;
       }
     }
     return nullptr;
}

/*
* returns a list of all possible moves for a particular side
* s - the side to find moves for
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
* moves - an array of the possible moves
*/
Move *Player::getBestMove(std::vector<Move*> moves) {
  //sets up a test board
  Board *maybe = new Board();
  int score = -64;
  Move* bestMove = moves[0];
  //set up an array for the heuristic (not used yet)
  int multiplier[8][8] = {
      {3, -2, 2, 2, 2, 2, -2, 3},
      {-2, -3, 1, 1, 1, 1, -3, -2},
      {2, 1, 1, 1, 1, 1, 1, 2},
      {2, 1, 1, 1, 1, 1, 1, 2},
      {2, 1, 1, 1, 1, 1, 1, 2},
      {2, 1, 1, 1, 1, 1, 1, 2},
      {-2, -3, 1, 1, 1, 1, -3, -2},
      {3, -2, 2, 2, 2, 2, -2, 3},
  };
  //go through the vector of possible moves, finding the best move
  std::vector<Move*>::iterator i;
  for(i = moves.begin(); i != moves.end(); i++) {
     maybe = b->copy();
     maybe->doMove(*i, my_side);
     int temp = maybe->count(my_side) - maybe->count(other_side);
     //multiply by values to get modified possible score
     temp *= multiplier[(*i)->getY()][(*i)->getX()];

     if (temp > score) {
       score = temp;
       bestMove = *i;
     }
  }
  return bestMove;
}

/*
* miniMax - uses a simple heuristic and the miniMax algorithm to find the
* best move
*/
Move *Player::miniMax(std::vector<Move*> moves){
  vector<Move*> yourMoves = getMoves(my_side);
  vector<Move*> opponentMoves;
  Board *copy;
  int score = 0;
  vector<int> scores;
  int minScore = 64;
  vector<int> minScores;
  int maxScore = -64;
  Move* choice;

  /*if(yourMoves.size() == 1)
  {
    choice = yourMoves[0];
  }*/

  //go through our list of possible moves, apply one to the board each time
  for(unsigned int i = 0; i < yourMoves.size(); i++)
  {
    minScore = 64;
    copy = this->b->copy();
    copy->doMove(yourMoves[i], my_side);
    opponentMoves = getMoves(other_side);
    //once we have applied a move, go through the opponent's list of possible
    //moves and calculate our simple score
    for(unsigned int j = 0; j < opponentMoves.size(); j++)
    {
      copy->doMove(opponentMoves[j], other_side);
      score = copy->count(my_side) - copy->count(other_side);
      scores.push_back(score);
    }
    //now go through all the possible scores and find the minimum score
    for(unsigned int k = 0; k < scores.size(); k++)
    {
      if(scores[k] < minScore)
      {
        minScore = scores[k];
      }
    }
    minScores.push_back(minScore);
  }

  //once we have found the minimum scores for all the possible combinations of
  //our moves and our opponent's moves, we find the maximum of those scores and
  //make the corresponding move
  for(unsigned int x = 0; x < minScores.size(); x++)
  {
    if(minScores[x] > maxScore)
    {
      maxScore = minScores[x];
      choice = yourMoves[x];
    }
  }

  /*time_t timer;
  time(&timer);

  if(difftime(oldtimer, timer) < 4)
  {
    copy->doMove(choice, my_side);
    return miniMax(moves, timer);
  }*/
  return choice;

}

Move *Player::recursiveMiniMax(Board *board, int depth, int score, Move *move, time_t oldtimer)
{
  //changed to count down instead

  Board *copy;
  int oldScore;
  Move *choice;

  if (depth == 0)
  {
    return move;
  }

  time_t timer;
  time(&timer);
  //timer commented out if not working
/*
  if(difftime(oldtimer, timer) > 4)
  {
    return move;
  }
*/
  //gets an array of possible next moves for this color
  std::vector<Move*> possibleMoves = getMoves(my_side);

  for(unsigned int i = 0; i < possibleMoves.size(); i++)
  {
    copy = board->copy();
    copy->doMove(possibleMoves[i], my_side);
    //counts the score after my possible move has been made

    oldScore = copy->count(my_side) - copy->count(other_side);
    recursiveMiniMax(copy, depth - 1, -oldScore, move, timer);
    if(score > oldScore)
    {
      oldScore = score;
      choice = move;
    }
  }
  return choice;
}

/*
* first attempt at alpha-beta pruning
*/

int Player::negaMax(int depth, int alpha, int beta)
{
  std::vector<Move*> possibleMoves = getMoves(my_side);
  int score;
  Move* choice;

  if(depth == 0)
  {
    score = this->b->count(my_side) - this->b->count(other_side);
    return score;
  }

  for(unsigned int i = 0; i < possibleMoves.size(); i++)
  {
    score = -negaMax(depth - 1, -beta, -alpha);

    if(score > alpha)
    {
      alpha = score;
      choice = possibleMoves[i];
    }
    if(score >= beta)
    {
      return beta;
    }
  }

  negaMaxMove = choice;
  return alpha;
}
