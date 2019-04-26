/*
 * LP_handler.h
 *
 *  Created on: 12 במרס 2019
 *      Author: nir
 */

#ifndef LP_HANDLER_H_
#define LP_HANDLER_H_
#include "BoardHandle.h"
#include "gurobi_c.h"
#include "LargeArray.h"

/*
 * This function initializes a map for a board. (see explenation in .c file);
 *
 * @param board - the board to make the map for.
 * @param map - a pointer to array of size (NXM)^3, at the end of successful run will hold the map.
 *
 * return # of vars that are mapped (out of the (NXM)^3)
 */
int map_maker(BOARD *board,large_array_struct *map);

/*
 * This function puts a solution made by the ILP/LP into the board. (see explenation in .c file);
 *
 * @param board - the board to put the solution in.
 * @param map - the map corresponds to the board.
 * @param sol - the array of solutions(scores) made by the ILP/LP.
 * @param threshold - the threshold to cut all the scores below it.
 * return TRUE if the board was fully solved
 * 		  FALSE else
 */
int put_sol_in_board(BOARD *board,large_array_struct *map, double *sol, double threshold);

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
int gurobi(BOARD *board,int num_of_vars,large_array_struct *map, int is_binary, double *solution);
/*
 * This function is taking the map and the solution of the LP algorithm
 * in order to find the scores of legal values in a certain cell and build this array of score.
 *
 * @param map - the map corresponds to the board we want to get the hint for.
 * @param sol - the scores we got from the LP/ILP algorithm.
 * @param x - the col of the cell we want to get the hint for.
 * @param y - the row of the cell we want to get the hint for.
 * @param scores - an array of size N*M that will hold for each value its score.
 *
 */
void get_hint(large_array_struct *map,double *sol, int x,int y,double *scores);
#endif /* LP_HANDLER_H_ */
