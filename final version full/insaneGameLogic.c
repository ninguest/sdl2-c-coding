#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "insaneGameLogic.h"

bool any_moves_left(char board[3][3]);
int evaluate(char board[3][3]);
int minimax(char board[3][3], int depth, bool ismax, int alpha, int beta);
struct move find_best_move(char board[3][3]);

// Structure: represent the AI's move
struct move
{
    int row, col;
};

// x is the maximiser and o is the minimizer
char computer = 2, player = 1;

int insaneComputerTurn(char board[3][3])
{
    int numberreturn = 0;

    struct move best_move = find_best_move(board);

    numberreturn = best_move.row * 3 + best_move.col;

    return numberreturn;
}

// Function: check if there are any empty cells on the board
bool any_moves_left(char board[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == 0)
            {
                return true;
            }
        }
    }
    return false;
}

// Function: evaluate the current state of the board and returns the points
int evaluate(char board[3][3])
{
    // Check rows for either x or o's victory
    for (int row = 0; row < 3; row++)
    {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2])
        {
            if (board[row][0] == computer)
            {
                return +10;
            }

            else if (board[row][0] == player)
            {
                return -10;
            }
        }
    }

    // Check columns for either x or y's victory
    for (int col = 0; col < 3; col++)
    {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col])
        {
            if (board[0][col] == computer)
            {
                return +10;
            }

            else if (board[0][col] == player)
            {
                return -10;
            }
        }
    }

    // Check diagonals for either x or y's victory
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        if (board[0][0] == computer)
        {
            return +10;
        }
        else if (board[0][0] == player)
        {
            return -10;
        }
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        if (board[0][2] == computer)
        {
            return +10;
        }
        else if (board[0][2] == player)
        {
            return -10;
        }
    }

    // if if non of the conditions are met above, the game is tied
    return 0;
}

// Function: minimax
int minimax(char board[3][3], int depth, bool ismax, int alpha, int beta)
{
    // check if game is at terminal state
    int score = evaluate(board);

    if (score == 10)
    {
        return score - depth; // to account for the depth
    }

    if (score == -10)
    {
        return score + depth; // to account for dept
    }

    if (any_moves_left(board) == false)
    {
        return 0;
    }

    // check if it is max player's turn
    if (ismax)
    {
        int best = INT_MIN;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = computer;
                    int current_score = minimax(board, depth + 1, !ismax, alpha, beta);
                    best = best > current_score ? best : current_score;
                    board[i][j] = 0;

                    alpha = alpha > best ? alpha : best; // update alpha
                    if (beta <= alpha)
                    {
                        return best; // prune
                    }
                }
            }
        }
        return best;
    }

    else
    {
        int best = INT_MAX;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = player;
                    int current_score = minimax(board, depth + 1, !ismax, alpha, beta);
                    best = best < current_score ? best : current_score;
                    board[i][j] = 0;

                    beta = beta < best ? beta : best; // update beta

                    if (beta <= alpha)
                    {
                        return best; // prune
                    }
                }
            }
        }
        return best;
    }
}

// Function: returns the optimal move for the AI bot

// Function: returns the optimal move for the AI bot
struct move find_best_move(char board[3][3])
{
    int best_val = INT_MIN;
    struct move bestmove;
    bestmove.col = -1;
    bestmove.row = -1;

    // run through all cells and return the cell with the highest value
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == 0)
            {
                // computer makes a move
                board[i][j] = computer;
                // calculate score for this move
                int move_val = minimax(board, 0, false, INT_MIN, INT_MAX);
                // undo the move
                board[i][j] = 0;
                // if valule of current move is more than best value, update best_val to move_val
                if (move_val > best_val)
                {
                    bestmove.col = j;
                    bestmove.row = i;
                    best_val = move_val;
                }
            }
        }
    }
    return bestmove;
}