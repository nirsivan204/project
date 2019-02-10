/*
 * Solver module - finds a solution for an board of sudoku.
 *
 *initialize_puzzle - initializes a solvable puzzle to game_board, and storing the solution.
 *build_board 		- can build a new a random board or check deterministicly if it is solvable, and storing the solution.
 *
 */

//#include "BoardHandle.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


/*
 * This function makes a new sudoku board, a suitable fixed board (a copy of the game-board) and
 * also saving its solution. It uses random backtracking algorithem, to find a random solved board, and fixes a specified
 * number of cells to make the game board.
 * After this initializations, it prints the game board.
 *
 * @param fix_num - num of fixed cells needed in the new board.
 * @param game_board - the board that will be used for storing the new game. At start will be a copy of fix_board
 * @param fix_board - the board that will be used for storing the new fixed board.
 * @param solved_board - the board that will be used for storing the new game solution.
 *
 *
 */
void initialize_puzzle (int fix_num,BOARD game_board ,BOARD fix_board, BOARD solved_board);
/*
 *
 * This function is building a board (by solving it).
 * It can use random or deterministic brute force solving.
 * At the end, it returns if the build was successful.
 *
 * @param is_determin - 0 if randomly board building is needed, 1 if deterministic validation is needed
 * @param board - the board that will be used for storing the solution of the game.
 *
 * @return 1 if build/validation was successful, 0 if not.
 *
 */
int build_board(BOARD board, int is_determin);
