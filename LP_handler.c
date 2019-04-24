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
	double random_number = get_rand_number(RAND_MAX)/RAND_MAX;
	double *partial_sum_array = (double*)calloc(num_of_values,sizeof(double));
	if(partial_sum_array == NULL){
		print_system_error(1,"error in allocating memory for partial_sum_array");
	}
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


int put_sol_in_board(BOARD *board,int *map, double *sol, int nXm,int nXm_square, double threshold){
	int i,j,k;
	int row_offset,col_offset,map_index;
	int val = 0;
	int num_of_legal_values = 0;
	int res = TRUE, *legal_values_array = NULL;
	double *legal_scores_array = (double *)calloc(nXm,sizeof(double));
	if(legal_scores_array == NULL){
		print_system_error(1,"error in allocating memory for legal_scores_array");
	}
	legal_values_array = (int *)calloc(nXm,sizeof(int));
	if(legal_values_array == NULL){
		print_system_error(1,"error in allocating memory for legal_values_array");
	}
	for(i = 0; i<nXm; i++){
		row_offset = i*nXm_square;
		for(j = 0; j<nXm; j++){
			col_offset = j*nXm;
			for(k = 1; k<=nXm; k++){
				map_index = map[row_offset+col_offset+k-1];
				if(map_index>-1){
					//printf("i=%d j=%d k=%d score = %f\n",i,j,k,sol[map_index]);
					if(sol[map_index] >= threshold){
						if(is_valid_insertion_to_empty_cell(board,j,i,k)){
							legal_scores_array[num_of_legal_values] = sol[map_index];
							legal_values_array[num_of_legal_values] = k;
							num_of_legal_values++;
						}
					}
				}
			}
			if(num_of_legal_values > 0){
				//printf("num_of_legal_values = %d\n",num_of_legal_values);
				val = choose_value_by_probability(legal_scores_array,legal_values_array,num_of_legal_values);
				//printf("val = %d\n",val);
				set_element_to_board(board,j,i,val);
			}else{
				if (map_index != -1) {
					res = FALSE;
				}
			}
			num_of_legal_values = 0;
		}
	}
	int r = 0;
	print_board(board,board,0,0,&r,&r);
	free(legal_scores_array);
	free(legal_values_array);
	return res;
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

void make_new_model(GRBenv **env,GRBmodel **model,char *error_string){
	int error = GRBloadenv(env, "mip1.log");/* Create environment - log file is mip1.log */
	if (error) {
	  sprintf(error_string,"ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(*env));
	  print_system_error(2,error_string);
	}
	error = GRBsetintparam(*env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
	  sprintf(error_string,"ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(*env));
	  print_system_error(2,error_string);
	}
	error = GRBnewmodel(*env, model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);/*Create an empty model named "mip1"*/
	if (error) {
	  sprintf(error_string,"ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(*env));
	  print_system_error(2,error_string);
	}
}

void initialize_model(GRBenv *env,GRBmodel *model,char *error_string,char *vtype,double *obj,double *val,int num_of_var){
	int error = GRBaddvars(model, num_of_var, 0, NULL, NULL, NULL, obj, NULL, val, vtype, NULL); /* add variables to model */
	if (error) {
	  sprintf(error_string,"ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
	  printf("nir");
	  print_system_error(2,error_string);
	}
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);/* Change objective sense to maximization*/
	if (error) {
	  sprintf(error_string,"ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  print_system_error(2,error_string);
	}
	error = GRBupdatemodel(model);/*   update the model - to integrate new variables*/
	if (error) {
	  sprintf(error_string,"ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
	  print_system_error(2,error_string);
	}
}


void initialize_arrays_for_gurobi(char *vtype,double *obj,double *val,int num_of_var,int is_binary,int nXm){
	int i;
	for(i=0;i<num_of_var;i++){  /* variable types - for xijk */
	  	  if(is_binary){
	  		  obj[i] = 1;
	  		  vtype[i] = GRB_BINARY;
	  	  }
	  	  else{
	  		  obj[i] = get_rand_number(nXm)+1;
	  		  vtype[i] = GRB_CONTINUOUS;
	  	  }
	  	  val[i] = 1;
	  }
}

void constraint_helper(GRBmodel *model,GRBenv *env,BOARD *board,int *map,int k, int *ind,double *val,int nXm, int nXm_square, int row_col_or_cell,char *error_string){
	int i ,constraint_len ,error = 0;
	char *type;
	for(i=0;i<nXm;i++){
		if(row_col_or_cell == 1){
			constraint_len = row_constraint(board,map,i,k,ind,nXm,nXm_square);
			type = "row";
		}
		if(row_col_or_cell == 2){
			constraint_len = col_constraint(board,map,i,k,ind,nXm,nXm_square);
			type = "col";
		}
		if(row_col_or_cell == 3){
			constraint_len = cell_constraint(board,map,i,k,ind,nXm,nXm_square);
			type = "cell";
		}

		if(constraint_len>0){
			error = GRBaddconstr(model, constraint_len , ind, val, GRB_EQUAL, 1, NULL);
			if (error) {
				if(row_col_or_cell == 3){
					sprintf(error_string,"ERROR %d GRBaddconstr() for cell (%d,%d): %s\n", error,i,k,GRBgeterrormsg(env));
				}
				else{
					sprintf(error_string,"ERROR %d GRBaddconstr() for %s %d, digit %d: %s\n", error,type,i,k,GRBgeterrormsg(env));
				}
				print_system_error(2,error_string);
			}
		}
	}
}

void put_constraints_in_model(GRBmodel *model,GRBenv *env,BOARD *board,int *map, int *ind,double *val,int nXm, int nXm_square){
	int i,j,k,constraint_len = 0,error = 0;
	char error_string[1000];
	for (k=1;k<=nXm;k++){
		constraint_helper(model,env,board,map,k,ind,val,nXm,nXm_square,1,error_string);
		constraint_helper(model,env,board,map,k,ind,val,nXm,nXm_square,2,error_string);
		constraint_helper(model,env,board,map,k-1,ind,val,nXm,nXm_square,3,error_string);
		for(i=0;i<board->N;i++){
			for(j=0;j<board->M;j++){
				constraint_len = block_constraint(board,map,i,j,k,ind,nXm,nXm_square);
				if(constraint_len>0){
					error = GRBaddconstr(model, constraint_len , ind, val, GRB_EQUAL, 1, NULL);
					if (error) {
						sprintf(error_string,"ERROR %d GRBaddconstr() for block (%d,%d): %s\n", error,j,i,GRBgeterrormsg(env));
						print_system_error(2,error_string);
					}
				}
			}
		}
	}
}

void optimize_model(GRBmodel *model,GRBenv *env,char *error_string){
	int error = GRBoptimize(model);/* Optimize model - need to call this before calculation */
	if (error) {
		sprintf(error_string,"ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		print_system_error(2,error_string);
	}
	error = GRBwrite(model, "mip1.lp");/*Write model to 'mip1.lp' - this is not necessary but very helpful*/
	if (error) {
		sprintf(error_string,"ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		print_system_error(2,error_string);
	}
}

int gurobi(BOARD *board,int num_of_var,int *map, int is_binary, double *sol)
{
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int       error = 0, optimstatus = 0, constraint_len = 0,res;
  char 		error_string[1000];
  int 		i,j,k,nXm = board->M*board->N;
  int 		nXm_square = nXm*nXm;
  int       *ind = (int*)malloc(board->N*board->M*sizeof(int));
  double    *val = (double*)malloc(num_of_var*sizeof(double));
  double    *obj = (double*)malloc(num_of_var*sizeof(double));
  char      *vtype = (char*)malloc(num_of_var*sizeof(char));
  double    objval = 0;
  if(ind==NULL || val == NULL ||obj ==NULL || vtype==NULL){
	  print_system_error(1,"error in allocating memory for gurobi arrays");
  }
  make_new_model(&env,&model,error_string);
  initialize_arrays_for_gurobi(vtype,obj,val,num_of_var,is_binary,nXm);
  initialize_model(env,model,error_string,vtype,obj,val,num_of_var);
  put_constraints_in_model(model,env,board,map,ind,val,nXm,nXm_square);
  optimize_model(model,env,error_string);
  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);/*   Get solution information*/
  if (error) {
	  sprintf(error_string,"ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
	  print_system_error(2,error_string);
  }
  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
  	  printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
  	  return -1;
    }
  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, num_of_var, sol);/*   get the solution - the assignment to each variable*/
  if (error) {
	  sprintf(error_string,"ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
	  print_system_error(2,error_string);
  }
  if (optimstatus == GRB_OPTIMAL) {/*   solution found*/
	  res = TRUE;
  }
  else if (optimstatus == GRB_INF_OR_UNBD) {/* Model is infeasible or unbounded, no solution found*/
	  res = FALSE;
  }
  else {/*error or calculation stopped*/
	  print_system_error(2,"Optimization was stopped early");
  }
  GRBfreemodel(model);
  GRBfreeenv(env);
  free(ind);
  free(obj);
  free(vtype);
  free(val);
  return res;
}




