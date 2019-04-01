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
int map_maker(BOARD *board,int *map, int nXm,int nXm_square);
int row_constraint(BOARD *board, int *map, int row, int digit, int *ind, int nXm, int nXm_square);
int col_constraint(BOARD *board,int *map, int col, int digit, int *ind, int nXm, int nXm_square);
int cell_constraint(BOARD *board,int *map, int row,int col, int *ind, int nXm, int nXm_square);
int block_constraint(BOARD *board,int *map,int block_row, int block_col,int digit, int *ind,int nXm, int nXm_square);
int func(BOARD *board,int num_of_var,int *map);
int put_sol_in_board(BOARD *board,int *map, double *sol, int nXm,int nXm_square, double threshold);
int gurobi(BOARD *board,int num_of_var,int *map, int is_binary, double *solution);
void get_hint(int *map,double *sol, int x,int y,int nXm,int nXm_square,double *scores);
#endif /* LP_HANDLER_H_ */
