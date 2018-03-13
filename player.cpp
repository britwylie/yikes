/*
* Player class for Othello
* @author bwylie & tloverid
*/
//hello Zach
#include "player.hpp"
#include <iostream>
#include <limits>
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
         Move* best = all_moves[0];
         int depth;
         //getBestMove uses the heuristic, comment it out and uncomment the next line to use Minimax instead
         //Move* best = getBestMove(all_moves);
         //if (testingMinimax) {
           //best = miniMax(all_moves);
         //} else {
           depth = 4;
           recursiveMiniMax(b, my_side, best, depth);
         //}


         /*negaMax(-64, 64, 5);
         best = negaMaxMove;*/

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
  int score = std::numeric_limits<int>::min();
  Move* bestMove = moves[0];
  //set up an array for the heuristic (not used yet)
  int multiplier[8][8] = {
      {10, -50, 8, 8, 8, 8, -50, 10},
      {-50, -50, 1, 1, 1, 1, -50, -50},
      {8, 1, 1, 1, 1, 1, 1, 8},
      {8, 1, 1, 1, 1, 1, 1, 8},
      {8, 1, 1, 1, 1, 1, 1, 8},
      {8, 1, 1, 1, 1, 1, 1, 8},
      {-50, -50, 1, 1, 1, 1, -50, -50},
      {10, -50, 8, 8, 8, 8, -50, 10},
  };
  //go through the vector of possible moves, finding the best move
  std::vector<Move*>::iterator i;
  for(i = moves.begin(); i != moves.end(); i++) {
     maybe = b->copy();
     maybe->doMove(*i, my_side);
     int temp = maybe->count(my_side) - maybe->count(other_side);
     temp -= getFrontierSquares(my_side, b);
     temp += getFrontierSquares(other_side, b);
     //multiply by values to get modified possible score
     temp *= multiplier[(*i)->getY()][(*i)->getX()];
     //account for mobility
     if(getMoves(my_side).size() > getMoves(other_side).size())
     {
       temp *= 10;
     }
     if(getMoves(my_side).size() < getMoves(other_side).size())
     {
       temp *= -30;
     }

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

  return choice;

}


/*
* New attempt at recursive minimax
*/
int Player::recursiveMiniMax(Board *board, Side s, Move*& best, int depth) {
  Board *copy;
  int next_score, max;
  Side other_side = (s == BLACK) ? WHITE : BLACK;
  int multiplier[8][8] = {
      {10, -50, 8, 8, 8, 8, -50, 10},
      {-50, -50, 1, 1, 1, 1, -50, -50},
      {8, 1, 1, 1, 1, 1, 1, 8},
      {8, 1, 1, 1, 1, 1, 1, 8},
      {8, 1, 1, 1, 1, 1, 1, 8},
      {8, 1, 1, 1, 1, 1, 1, 8},
      {-50, -50, 1, 1, 1, 1, -50, -50},
      {10, -50, 8, 8, 8, 8, -50, 10},
  };
  std::vector<Move*> moves = getMoves(s);
  max = std::numeric_limits<int>::min();
  //base case
  if (depth == 0  ){//}|| moves.size() == 0) {
    int score = board->count(s) - board->count(other_side);
    score -= getFrontierSquares(s, board);
    score += getFrontierSquares(other_side, board);
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (board->get(s, i, j)) {
          score += multiplier[i][j];
        } else if (board->get(other_side, i , j)) {
          score -= multiplier[i][j];
        }
      }
    }

    if(getMoves(s).size() > getMoves(other_side).size())
    {
      score += 10;
    }
    if(getMoves(s).size() < getMoves(other_side).size())
    {
      score += -30;
    }


    return score;
  }

  //get all possible moves for this side

  //score = board->count(s) - board->count(other_side);
  //find the one that maximizes the score
  for(unsigned int i = 0; i < moves.size(); i++) {
    copy = board->copy();
    copy->doMove(moves[i], s);
    Move* temp = moves[i];

    next_score = -recursiveMiniMax(copy, other_side, temp, depth - 1);
    next_score *= multiplier[moves[i]->getY()][moves[i]->getX()];
    delete copy;
    if (next_score > max) {
      max = next_score;
      best = moves[i];
    }
  }
  for(unsigned int i = 0; i < moves.size(); i++) {
    if (moves[i] != best) {
      delete moves[i];
    }
  }

}

/*
* Evaluates the frontier squares available for a given side
*/
int Player::getFrontierSquares(Side s, Board* b) {
  Side other_side = (s == BLACK) ? WHITE : BLACK;
  int count = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (b->get(s, i, j)) {
        for (int x = -1; x < 2; x++) {
          for (int y = -1; y < 2; y++) {
            if ((i + x < 8 && i + x >= 0) && (j + y < 8 && j + y >= 0)) {
              if (!b->get(s, i + x, j + y) && !b->get(other_side, i + x, j + y)) {
                count ++;
              }
            }
          }
        }
      }
    }
  }
  return count;
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
