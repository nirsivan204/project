/*
 * LP_handler.h
 *
 *  Created on: 12 ���� 2019
 *      Author: nir
 */

#ifndef LP_HANDLER_H_
#define LP_HANDLER_H_
#include "BoardHandle.h"
#include "gurobi_c.h"

/*
 * This function initializes a map for a board. (see explenation in .c file);
 *
 * @param board - the board to make the map for.
 * @param map - a pointer to array of size (NXM)^3, at the end of successful run will hold the map.
 * @param nXm - NXM (dimensions of board).
 * @param nXm_square - (NXM)^2.
 *
 * return # of vars that are mapped (out of the (NXM)^3)
 */
int map_maker(BOARD *board,int *map, int nXm,int nXm_square);

/*
 * This function puts a solution made by the ILP/LP into the board. (see explenation in .c file);
 *
 * @param board - the board to put the solution in.
 * @param map - the map corresponds to the board.
 * @param sol - the array of solutions(scores) made by the ILP/LP.
 * @param nXm - NXM (dimensions of board).
 * @param nXm_square - (NXM)^2.
 * @param threshold - the threshold to cut all the scores below it.
 * return TRUE if the board was fully solved
 * 		  FALSE else
 */
int put_sol_in_board(BOARD *board,int *map, double *sol, int nXm,int nXm_square, double threshold);

/*
 * This function puts a solution made by the ILP/LP into the board. (see explenation in .c file);
 *
 * @param board - the board to solve.
 * @param num_of_var - # of vars (the result of map maker)
 * @param map - the map corresponds to the board.
 * @param is_binary - if true, its ILP, else LP.
 * @param solution - the array that will hold the solutions(scores) made by the ILP/LP.
 * return TRUE if the board was fully solved
 * 		  FALSE else
 */
int gurobi(BOARD *board,int num_of_vars,int *map, int is_binary, double *solution);
/*
 * This function is taking the map and the solution of the LP algorithm
 * in order to find the scores of legal values in a certain cell and build this array of score.
 *
 * @param map - the map corresponds to the board we want to get the hint for.
 * @param sol - the scores we got from the LP/ILP algorithm.
 * @param x - the col of the cell we want to get the hint for.
 * @param y - the row of the cell we want to get the hint for.
 * @param nXm - N*M (dimensions of board).
 * @param nXm_square - (N*M)^2 (dimensions of board).
 * @param scores - an array of size N*M that will hold for each value its score.
 *
 */
void get_hint(int *map,double *sol, int x,int y,int nXm,int nXm_square,double *scores);
#endif /* LP_HANDLER_H_ */
