/*
 * LP_handler.c
 *
 *  Created on: 12 במרס 2019
 *      Author: nir
 */
#include "LP_handler.h"

void mark_row(int *map,int row_offset,int k,int nXm){
	int j;
	for(j=0;j<nXm;j++){
		map[row_offset + nXm*j + k-1] = -1;
	}
}

void mark_col(int *map,int col_offset,int k,int nXm,int nXm_square){
	int i;
	for(i=0;i<nXm;i++){
		map[nXm_square*i + col_offset + k-1] = -1;
	}
}

void mark_cell(int *map,int row_offset,int col_offset,int nXm){
	int k;
	for(k=0;k<nXm;k++){
		map[row_offset + col_offset + k] = -1;
	}
}

void mark_block(int *map,int row,int col, int k,int n,int m,int nXm,int nXm_square){
	int first_row_of_block = (row/m)*m;
	int first_col_of_block = (col/n)*n;
	int row_offset,col_offset;
	int i,j;
	for(i=0;i<m;i++){
		row_offset = (first_row_of_block+i)*nXm_square;
		for (j=0;j<n;j++){
			col_offset = (first_col_of_block+j)*nXm;
			map[row_offset+col_offset+k-1] = -1;
		}
	}

}


int map_maker(BOARD *board,int *map, int nXm,int nXm_square){//assuming map is only zeros
	int i,j,k;
	int num_of_var = 0;
	int row_offset,col_offset;
	for(i=0;i<nXm;i++){
		row_offset = i*nXm_square;
		for(j=0;j<nXm;j++){
			col_offset = j*nXm;
			k=get_element_from_board(board,j,i);
			if(k!=0){
				mark_row(map,row_offset,k,nXm);
				mark_col(map,col_offset,k,nXm,nXm_square);
				mark_cell(map,row_offset,col_offset,nXm);
				mark_block(map,i,j,k,board->N,board->M,nXm,nXm_square);
			}
		}
	}
	for(i=0;i<nXm*nXm_square;i++){
		if(map[i]!=-1){
			map[i] = num_of_var;
			num_of_var++;
		}
	}
	return num_of_var;
}

int row_constraint(BOARD *board, int *map, int row, int digit,int *ind, int nXm, int nXm_square){
	int j=0;
	int row_index = nXm_square*row;
	int map_index;
	int counter = 0;
	if(is_valid_row(board,row,digit)){
		for (;j<nXm;j++){
			map_index = map[row_index+nXm*j+digit-1];
			if(map_index!=-1){
				ind[counter] = map_index;
				counter++;
			}
		}
	}
	return counter;
}

int col_constraint(BOARD *board,int *map, int col, int digit,int *ind, int nXm, int nXm_square){
	int i=0;
	int col_index = nXm*col;
	int map_index;
	int counter = 0;
	if(is_valid_column(board,col,digit)){
		for (;i<nXm;i++){
			map_index = map[nXm_square*i+col_index+digit-1];
			if(map_index!=-1){
				ind[counter] = map_index;
				counter++;
			}
		}
	}
	return counter;
}

int cell_constraint(BOARD *board,int *map, int row,int col,int *ind, int nXm, int nXm_square){
	int k=1;
	int col_index = nXm*col;
	int row_index = nXm_square*row;
	int map_index;
	int counter = 0;
	if(get_element_from_board(board,col,row)==0){
		for (;k<=nXm;k++){
				map_index = map[row_index+col_index+k-1];
				if(map_index!=-1){
					ind[counter] = map_index;
					counter++;
				}
			}
	}
	return counter;
}

int block_constraint(BOARD *board,int *map,int block_row, int block_col,int digit,int *ind,int nXm, int nXm_square){
	int map_index,i,j,counter = 0;
	int row_offset,col_offset;
	int m = board->M;
	int n = board->N;
	int first_row_index = block_row*m;
	int first_col_index = block_col*n;
	if(is_valid_block(board->blocks[block_row][block_col],digit)){
		for (i=0;i<m;i++){
			row_offset = (first_row_index+i)*nXm_square;
			for(j=0;j<n;j++){
				col_offset = (first_col_index+j)*nXm;
				map_index = map[row_offset + col_offset + digit-1];
				if(map_index!=-1){
					ind[counter] = map_index;
					counter++;
				}
			}
		}
	}
	return counter;
}


/* Copyright 2013, Gurobi Optimization, Inc. */

/* This example formulates and solves the following simple MIP model:

     maximize    x + 3 y + 2 z
     subject to  x + 2 y + 3 z <= 5
                 x +   y       >= 1
     x, y, z binary
*/



int func(BOARD *board,int num_of_var,int *map)
{
 //GRBenv   *env   = NULL;
  //GRBmodel *model = NULL;
 // int       error = 0;
 // double    *sol = (int*)malloc(num_of_var*sizeof(int));
  int       *ind = (int*)malloc(board->N*board->M*sizeof(int));
 // double    *val = (int*)malloc(num_of_var*sizeof(int));
 // double    *obj = (int*)malloc(num_of_var*sizeof(int));
 // char      *vtype = (int*)malloc(num_of_var*sizeof(int));
 // int       optimstatus;
 // double    objval;
  int i,j,k;
  int nXm = board->M*board->N;
  int nXm_square = nXm*nXm;
  int constraint_len;
  /* Create environment - log file is mip1.log */
  /*error = GRBloadenv(&env, "mip1.log");
  if (error) {
	  printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

   Create an empty model named "mip1"
  error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
  if (error) {
	  printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }*/

  /* Add variables */

  /* coefficients - for Xijk */
 // for(i=0;i<num_of_var;i++){
//	  obj[i] = 1;
//	  val[i] = 1; //for binary!!!!!!
  //}

  /* variable types - for x,y,z (cells 0,1,2 in "vtype") */
  /* other options: GRB_INTEGER, GRB_CONTINUOUS */
  //for(i=0;i<num_of_var;i++){
  //	  vtype[i] = GRB_BINARY;
  //}

  /* add variables to model */
/*  error = GRBaddvars(model, num_of_var, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
  if (error) {
	  printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

   Change objective sense to maximization
  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

   update the model - to integrate new variables

  error = GRBupdatemodel(model);
  if (error) {
	  printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }*/


  for (k=1;k<=nXm;k++){
	  for(i=0;i<nXm;i++){
		  constraint_len = row_constraint(board,map,i,k,ind,nXm,nXm_square);
		  print_array(ind,constraint_len);
		  //error = GRBaddconstr(model, constraint_len , ind, val, GRB_LESS_EQUAL, 1, NULL);
		  //if (error) {
		//	  printf("ERROR %d GRBaddconstr() for row %d, digit %d: %s\n", error,i,k,GRBgeterrormsg(env));
		 // 	  return -1;
		  //}
	  }
	  for(j=0;j<nXm;j++){
		  constraint_len = col_constraint(board,map,j,k,ind,nXm,nXm_square);
		  print_array(ind,constraint_len);

		  //error = GRBaddconstr(model, constraint_len , ind, val, GRB_LESS_EQUAL, 1, NULL);
		  //if (error) {
		   //   printf("ERROR %d GRBaddconstr() for col %d, digit %d: %s\n", error,j,k,GRBgeterrormsg(env));
		  //	  return -1;
		  //}
	  }

  }
  for(i=0;i<nXm;i++){
	  for(j=0;j<nXm;j++){
		  constraint_len = cell_constraint(board,map,i,j,ind,nXm,nXm_square);
		  print_array(ind,constraint_len);

  		  //error = GRBaddconstr(model, constraint_len , ind, val, GRB_LESS_EQUAL, 1, NULL);
  		  //if (error) {
  		//	  printf("ERROR %d GRBaddconstr() for cell (%d,%d): %s\n", error,j,i,GRBgeterrormsg(env));
  		 // 	  return -1;
  		 // }
  	  }
  }
  for(i=0;i<board->M;i++){
	  for(j=0;j<board->N;j++){
		  for(k=1;k<=nXm;k++){
			  constraint_len = block_constraint(board,map,i,j,k,ind,nXm,nXm_square);
			  print_array(ind,constraint_len);
			 // error = GRBaddconstr(model, constraint_len , ind, val, GRB_LESS_EQUAL, 1, NULL);
			  //if (error) {
				//  printf("ERROR %d GRBaddconstr() for block (%d,%d): %s\n", error,j,i,GRBgeterrormsg(env));
				  //return -1;
			  //}
		  }
	  }
  }


//
//
//  /* variables x,y,z (0,1,2) */
//  ind[0] = 0; ind[1] = 1; ind[2] = 2;
//  /* coefficients (according to variables in "ind") */
//  //val[0] = 1; val[1] = 2; val[2] = 3;
//
//  /* add constraint to model - note size 3 + operator GRB_LESS_EQUAL */
//  /* -- equation value (5.0) + unique constraint name */
//  error = GRBaddconstr(model, , ind, val, GRB_LESS_EQUAL, 1, "c0");

  /* Optimize model - need to call this before calculation */
  /*error = GRBoptimize(model);
  if (error) {
	  printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

   Write model to 'mip1.lp' - this is not necessary but very helpful
  error = GRBwrite(model, "mip1.lp");
  if (error) {
	  printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

   Get solution information

  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
  if (error) {
	  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

   get the objective -- the optimal result of the function
  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
  if (error) {
	  printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

   get the solution - the assignment to each variable
   3-- number of variables, the size of "sol" should match
  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, num_of_var, sol);
  if (error) {
	  printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

   print results
  printf("\nOptimization complete\n");

   solution found
  if (optimstatus == GRB_OPTIMAL) {
    printf("Optimal objective: %.4e\n", objval);
    print_array(sol,num_of_var);
  }
   no solution found
  else if (optimstatus == GRB_INF_OR_UNBD) {
    printf("Model is infeasible or unbounded\n");
  }
   error or calculation stopped
  else {
    printf("Optimization was stopped early\n");
  }

   IMPORTANT !!! - Free model and environment
  GRBfreemodel(model);
  GRBfreeenv(env);
*/
  return 0;
}




