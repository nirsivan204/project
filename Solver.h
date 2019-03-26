/*
 * Solver module - finds a solution for an board of sudoku.
 *
 *initialize_puzzle - initializes a solvable puzzle to game_board, and storing the solution.
 *build_board 		- can build a new a random board or check deterministicly if it is solvable, and storing the solution.
 *
 */

#include "backtracking_stack.h"
#include <stdlib.h>
#include <stdio.h>


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
int build_board(BOARD *board, int is_determin);


int exhaustive_backtracking(BOARD *board);
