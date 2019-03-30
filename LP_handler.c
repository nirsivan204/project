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

int choose_value_by_probability(double *scores,int *values,int num_of_values){
	int i,total_sum = 0;
	double *partial_sum_array = (double*)calloc(num_of_values,sizeof(double));
	double random_number = get_rand_number(RAND_MAX)/RAND_MAX;
	print_array_double(scores,num_of_values);
	partial_sum_array[0] = scores[0];
	if(num_of_values > 1){
		for(i=1;i<num_of_values;i++){
			partial_sum_array[i] = partial_sum_array[i-1] + scores[i];
			total_sum += scores[i];
		}
		for(i=0;i<num_of_values;i++){
			if(partial_sum_array[i]/total_sum > random_number){
				free(partial_sum_array);
				return values[i];
			}
		}
	}
	free(partial_sum_array);
	return values[0];
}


void put_sol_in_board(BOARD *board,int *map, double *sol, int nXm,int nXm_square, double threshold){
	int i,j,k;
	int row_offset,col_offset,map_index;
	int val = 0;
	int num_of_legal_values = 0;
	double *legal_scores_array = (double *)calloc(nXm,sizeof(double));
	int *legal_values_array = (int *)calloc(nXm,sizeof(int));
	for(i = 0; i<nXm; i++){
		row_offset = i*nXm_square;
		for(j = 0; j<nXm; j++){
			col_offset = j*nXm;
			for(k = 1; k<=nXm; k++){
				map_index = map[row_offset+col_offset+k-1];
				if(map_index>-1){
					printf("i=%d j=%d k=%d score = %f\n",i,j,k,sol[map_index]);
					if(sol[map_index] >= threshold){
						if(i==0&&j==4){
							printf("k = %d,is valid =  %d\n",k,is_valid_insertion_to_empty_cell(board,j,i,k));
						}
						if(is_valid_insertion_to_empty_cell(board,j,i,k)){
							//printf("im here");
							legal_scores_array[num_of_legal_values] = sol[map_index];
							legal_values_array[num_of_legal_values] = k;
							num_of_legal_values++;
						}
					}
				}
			}
			if(num_of_legal_values > 0){
				printf("num_of_legal_values = %d\n",num_of_legal_values);
				val = choose_value_by_probability(legal_scores_array,legal_values_array,num_of_legal_values);
				printf("val = %d\n",val);
				set_element_to_board(board,j,i,val);
			}
			num_of_legal_values = 0;
		}
	}
	free(legal_scores_array);
	free(legal_values_array);
}

void get_hint(int *map,double *sol, int x,int y,int nXm,int nXm_square,double *scores){
	int offset = x*nXm_square + y*nXm;
	int map_index;
	int k;
	for (k=0;k<nXm;k++){
		map_index = map[offset+k];
		if(map_index == -1){
			scores[k] = 0;
		}else{
			scores[k] = sol[map_index];
		}
	}
}

int gurobi(BOARD *board,int num_of_var,int *map, int is_binary, double *sol)
{
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int       error = 0;
  //double    *sol = (double*)malloc(num_of_var*sizeof(double));
  int       *ind = (int*)malloc(board->N*board->M*sizeof(int));
  double    *val = (double*)malloc(num_of_var*sizeof(double));
  double    *obj = (double*)malloc(num_of_var*sizeof(double));
  char      *vtype = (char*)malloc(num_of_var*sizeof(char));
  int       optimstatus;
  double    objval;
  int i,j,k;
  int nXm = board->M*board->N;
  int nXm_square = nXm*nXm;
  int constraint_len;
//  /* Create environment - log file is mip1.log */
  error = GRBloadenv(&env, "mip1.log");
  if (error) {
	  printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /*Create an empty model named "mip1"*/
  error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
  if (error) {
	  printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

//  /* Add variables */
//
//

  /* variable types - for xijk */
  /* other options: GRB_INTEGER, GRB_CONTINUOUS */
  for(i=0;i<num_of_var;i++){
  	  if(is_binary){
  		  vtype[i] = GRB_BINARY;
  	  }
  	  else{
  		  obj[i] = get_rand_number(nXm)+1;
  		  vtype[i] = GRB_CONTINUOUS;
  	  }
  	  val[i] = 1;
  }
  print_array_double(obj,num_of_var);

  /* add variables to model */
  error = GRBaddvars(model, num_of_var, 0, NULL, NULL, NULL, obj, NULL, val, vtype, NULL); // val is also upper bound of 1
  if (error) {
	  printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

/* Change objective sense to maximization*/
  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

///*   update the model - to integrate new variables*/

  error = GRBupdatemodel(model);
  if (error) {
	  printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  for (k=1;k<=nXm;k++){
	  for(i=0;i<nXm;i++){
		  constraint_len = row_constraint(board,map,i,k,ind,nXm,nXm_square);
		  if(constraint_len>0){
			  error = GRBaddconstr(model, constraint_len , ind, val, GRB_EQUAL, 1, NULL);
			  if (error) {
				  printf("ERROR %d GRBaddconstr() for row %d, digit %d: %s\n", error,i,k,GRBgeterrormsg(env));
				  return -1;
			  }
		  }
		  //print_array(ind,constraint_len);
	  }
	  for(j=0;j<nXm;j++){
		  constraint_len = col_constraint(board,map,j,k,ind,nXm,nXm_square);
		  //print_array(ind,constraint_len);
		  if(constraint_len>0){
			  error = GRBaddconstr(model, constraint_len , ind, val, GRB_EQUAL, 1, NULL);
			  if (error) {
				  printf("ERROR %d GRBaddconstr() for col %d, digit %d: %s\n", error,j,k,GRBgeterrormsg(env));
				  return -1;
			  }
		  }

	  }

  }
  for(i=0;i<nXm;i++){
	  for(j=0;j<nXm;j++){
		  constraint_len = cell_constraint(board,map,i,j,ind,nXm,nXm_square);
		  if(constraint_len>0){
			  error = GRBaddconstr(model, constraint_len , ind, val, GRB_EQUAL, 1, NULL);
			  if (error) {
				  printf("ERROR %d GRBaddconstr() for cell (%d,%d): %s\n", error,j,i,GRBgeterrormsg(env));
				  return -1;
			  }
		  }
  	  }
  }
  for(i=0;i<board->N;i++){
	  for(j=0;j<board->M;j++){
		  for(k=1;k<=nXm;k++){
			  constraint_len = block_constraint(board,map,i,j,k,ind,nXm,nXm_square);
			  //print_array(ind,constraint_len);
			  if(constraint_len>0){
				  error = GRBaddconstr(model, constraint_len , ind, val, GRB_EQUAL, 1, NULL);
				  if (error) {
					  printf("ERROR %d GRBaddconstr() for block (%d,%d): %s\n", error,j,i,GRBgeterrormsg(env));
					  return -1;
				  }
			  }
		  }
	  }
  }
//  /* Optimize model - need to call this before calculation */
  error = GRBoptimize(model);
  if (error) {
	  printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

/*   Write model to 'mip1.lp' - this is not necessary but very helpful*/
  error = GRBwrite(model, "mip1.lp");
  if (error) {
	  printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

/*   Get solution information*/

  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
  if (error) {
	  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

/*   get the objective -- the optimal result of the function*/
  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
  if (error) {
	  printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

/*   get the solution - the assignment to each variable*/
/*   number of variables, the size of "sol" should match*/
  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, num_of_var, sol);
  if (error) {
	  printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

/*   print results*/
  printf("\nOptimization complete\n");

/*   solution found*/
  if (optimstatus == GRB_OPTIMAL) {
    printf("Optimal objective: %.4e\n", objval);
  }
/*   no solution found*/
  else if (optimstatus == GRB_INF_OR_UNBD) {
    printf("Model is infeasible or unbounded\n");
  }
/*   error or calculation stopped*/
  else {
    printf("Optimization was stopped early\n");
  }

/*   IMPORTANT !!! - Free model and environment*/
  GRBfreemodel(model);
  GRBfreeenv(env);
  free(ind);
  free(obj);
  free(vtype);
  free(val);
  return 0;
}




