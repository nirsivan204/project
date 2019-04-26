#include "LP_handler.h"
/*
 * This function fills the map with -1 for variables
 * that will not be mapped because they are not legal in the row i.
 *
 * @param map - the map it is building.
 * @param i - the row in board. we have certain places in map(see map function)
 * @param k - the digit that is already in the row.
 */
void mark_row(large_array_struct *map,int i,int k){
	int j;
	for(j=0;j<map->nXm;j++){
		set_int_in_large_array(map,i,j,k-1,-1);
	}
}
/*
 * This function fills the map with -1 for variables
 * that will not be mapped because they are not legal in this col j.
 *
 * @param map - the map it is building.
 * @param j - the col in board. we have certain places in map(see map function)
 * @param k - the digit that is already in the col.
 */
void mark_col(large_array_struct *map,int j,int k){
	int i;
	for(i=0;i<map->nXm;i++){
		set_int_in_large_array(map,i,j,k-1,-1);
	}
}

/*
 * This function fills the map with -1 for variables
 * that will not be mapped because their cell is already occupied.
 * (the cell <j,i>)
 *
 * @param map - the map it is building.
 * @param i - the row in board. we have certain places in map(see map function)
 * @param j - the col in board. we have certain places in map(see map function)
 */
void mark_cell(large_array_struct *map,int i,int j){
	int k;
	for(k=0;k<map->nXm;k++){
		set_int_in_large_array(map,i,j,k,-1);
	}
}

/*
 * This function fills the map with -1 for variables
 * that will not be mapped because their block already contains their digit.
 * (the block that contains the cell <col,row>)
 *
 * @param map - the map it is building.
 * @param row - the row of the already-filled cell.
 * @param col - the col of the already-filled cell.
 * @param k - the digit that is already in this block.
 * @param n - N (dimensions of board).
 * @param m - M (dimensions of board).
 */
void mark_block(large_array_struct *map,int row,int col, int k,int n,int m){
	int first_row_of_block = (row/m)*m;
	int first_col_of_block = (col/n)*n;
	int i,j;
	for(i=0;i<m;i++){
		for (j=0;j<n;j++){
			set_int_in_large_array(map,i+first_row_of_block,j+first_col_of_block,k-1,-1);
		}
	}

}


/*
 * The map is an array of (NXM)^3 indexes, that will map the right vars into the vars array. We define the variable Xijk as the probability for the event that cell <j,i> will have the value k.
 * because we want to efficiently run the LP/ILP, we need to reduce our number of variables.
 * If we know that this Xijk for certain i,j,k is zero (e.g. this i row already has the digit k),
 * we want to ignore him. so our map will not map him to the vars array, and will give him the index -1.
 * else, we will give him his index in the vars array.
 * the place of the index for Xijk in the map, is: map[i][j][k-1]
 *
 * The map maker will run on all cells, and if it finds a non-empty cell, it will mark all the vars that will not be mapped with -1.
 * Afterwards, it will run on the map it built until now, and will give every legal variable its place in the vars array.
 * At the end, it will return the number of vars.
 *
 * @param board - the board we want to build the map for.
 * @param map - the map it is building.
 *
 * @return number of variables mapped.
 */
int map_maker(BOARD *board,large_array_struct *map){//assuming map is only zeros
	int i,j,k,index;
	int num_of_var = 0;
	for(i=0;i<map->nXm;i++){
		for(j=0;j<map->nXm;j++){
			k=get_element_from_board(board,j,i);
			if(k!=0){
				mark_row(map,i,k);
				mark_col(map,j,k);
				mark_cell(map,i,j);
				mark_block(map,i,j,k,board->N,board->M);
			}
		}
	}
	for(i=0;i<map->nXm;i++){
		for(j=0;j<map->nXm;j++){
			for(k=0;k<map->nXm;k++){
				index = get_int_from_large_array(map,i,j,k);
				if(index != -1){
					set_int_in_large_array(map,i,j,k,num_of_var);
					num_of_var++;
				}
			}
		}
	}
	return num_of_var;
}


/*
 * This function is building a row constraint,
 * and putting the indexes of the vars that are participating (from the vars_array)
 * in ind array. (see constraint_helper for full explanation)
 *
 * @param board - the board we want to build the row_constraint for.
 * @param map - the map corresponds to the board.
 * @param row - the index of the row we want to build the constraint for.
 * @param digit - the digit we want to build the constraint for.
 * @param ind - the array that will represent the constraint.
 *
 * @return number of non zero coefficients in this constraint.
 */
int row_constraint(BOARD *board, large_array_struct *map, int row, int digit,int *ind){
	int j=0;
	int map_index;
	int counter = 0;
	if(is_valid_row(board,row,digit)){
		for (;j<map->nXm;j++){
			map_index = get_int_from_large_array(map,row,j,digit-1);
			if(map_index!=-1){
				ind[counter] = map_index;
				counter++;
			}
		}
	}
	return counter;
}
/*
 * This function is building a col constraint,
 * and putting the indexes of the vars that are participating (from the vars_array)
 * in ind array. (see constraint_helper for full explanation)
 *
 * @param board - the board we want to build the col_constraint for.
 * @param map - the map corresponds to the board.
 * @param col - the index of the col we want to build the constraint for.
 * @param digit - the digit we want to build the constraint for.
 * @param ind - the array that will represent the constraint.
 *
 * @return number of non zero coefficients in this constraint.
 */
int col_constraint(BOARD *board,large_array_struct *map, int col, int digit,int *ind){
	int i=0;
	int map_index;
	int counter = 0;
	if(is_valid_column(board,col,digit)){
		for (;i<map->nXm;i++){
			map_index = get_int_from_large_array(map,i,col,digit-1);
			if(map_index!=-1){
				ind[counter] = map_index;
				counter++;
			}
		}
	}
	return counter;
}
/*
 * This function is building a cell constraint,
 * and putting the indexes of the vars that are participating (from the vars_array)
 * in ind array. (see constraint_helper for full explanation)
 *
 * @param board - the board we want to build the col_constraint for.
 * @param map - the map corresponds to the board.
 * @param row - the index of the row we want to build the constraint for.
 * @param col - the index of the col we want to build the constraint for.
 * @param ind - the array that will represent the constraint.
 *
 * @return number of non zero coefficients in this constraint.
 */
int cell_constraint(BOARD *board,large_array_struct *map, int row,int col,int *ind){
	int k=1;
	int map_index;
	int counter = 0;
	if(get_element_from_board(board,col,row)==0){
		for (;k<=map->nXm;k++){
				map_index = get_int_from_large_array(map,row,col,k-1);
				if(map_index!=-1){
					ind[counter] = map_index;
					counter++;
				}
			}
	}
	return counter;
}
/*
 * This function is building a block constraint,
 * and putting the indexes of the vars that are participating (from the vars_array)
 * in ind array. (see constraint_helper for full explanation)
 *
 * @param board - the board we want to build the col_constraint for.
 * @param map - the map corresponds to the board.
 * @param block_row - the index of the row of the block we want to build the constraint for.
 * @param block_col - the index of the col of the block we want to build the constraint for.
 * @param digit - the digit we want to build the constraint for.
 * @param ind - the array that will represent the constraint.
 *
 * @return number of non zero coefficients in this constraint.
 */
int block_constraint(BOARD *board,large_array_struct *map,int block_row, int block_col,int digit,int *ind){
	int map_index,i,j,counter = 0;
	int m = board->M;
	int n = board->N;
	int first_row_index = block_row*m;
	int first_col_index = block_col*n;
	if(is_valid_block(board->blocks[block_row][block_col],digit)){
		for (i=0;i<m;i++){
			for(j=0;j<n;j++){
				map_index = get_int_from_large_array(map,first_row_index+i,first_col_index+j,digit-1);
				if(map_index!=-1){
					ind[counter] = map_index;
					counter++;
				}
			}
		}
	}
	return counter;
}

/*
 * This function is building all row/col/cell constraints for a specific value (if row/col) or specific col (if cell).
 * and putting them in the model.
 * @param model - the model to add the constraints to.
 * @param env - the environment of the model.
 * @param board - the board we want to build the constraints for.
 * @param map - the map corresponds to the board.
 * @param k - in the case of row/col, represents the value of the cell in the row/col we want to build the constraint for.
 * 			  in the case of cell, represents the col of the cell we ant to build the constrain for.
 * @param ind - the array that will represent the constraint.
 * @param val - array of coefficients of the variables in each constraint.
 * @param row_col_or_cell - 1 if row, 2 if col, 3 if cell.
 * @param error_string - a buffer to put the error message in.
 *
 */
void constraint_helper(GRBmodel *model,GRBenv *env,BOARD *board,large_array_struct *map,int k, int *ind,double *val, int row_col_or_cell,char *error_string){
	int i ,constraint_len ,error = 0;
	char *type;
	for(i=0;i<map->nXm;i++){
		if(row_col_or_cell == 1){
			constraint_len = row_constraint(board,map,i,k,ind);
			type = "row";
		}
		if(row_col_or_cell == 2){
			constraint_len = col_constraint(board,map,i,k,ind);
			type = "col";
		}
		if(row_col_or_cell == 3){
			constraint_len = cell_constraint(board,map,i,k,ind);
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

/*
 * This function is getting an array of values,
 * and array of the probabilities,and chooses randomly
 * between this values, with weights corresponds to their probability to be choose.
 * for instance, if values = {1,2,3},scores = {0.5,0.4,0.1},
 * this algorithm will choose in average "1" half of the times,
 * "2" 4 out every 10 times, and "3" 1 out of every 10 times.
 *
 * The algorithm:
 * First pick a random number in [0, 1): r (uniformly at random).
 * Now, going through the array,
 * find the current cumulative sum as partial_sum_array[i] = partial_sum_array[i-1] + scores[i],
 * with partial_sum_array[0] = scores[0].
 * Also we will find the sum of all the elements in the array: total_sum.
 * Pick the first element i where, partial_sum_array[i]/total_sum > r.
 *
 * Analysis of Correctness: (PSA = partial_sum_array)
 * Event: i is picked <=> PSA[i-1]/total_sum <= r < PSA[i]/total_sum
 * Thus, Prob(i is picked)=
 * = Prob(PSA[i-1]/total_sum <= r < PSA[i]/total_sum) =
 * = Prob(r < PSA[i]/total_sum) - Prob(r < PSA[i-1]/total_sum) =
 * = (PSA[i]-PSA[i-1])/total_sum = scores[i]/total_sum
 * which is what was desired.
 *
 *
 * @param scores - the probabilities of each value to be chosen.
 * @param values - the values to choose from.
 * @param num_of_values - the num of values to choose from.
 *
 * @return the value that has been chosen.
 */


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

/*
 * This function is putting a solution that was found by LP/ILP into the board.
 * for each row i, and col j:
 * 	for each value k, it will run on map array, to find a mapped variable.
 * 	if his score is >= threshold , check if it is still legal (it is possible that the algorithm previously chose this value in his row,col or block)
 * 	if yes, add it to legal_values_array and his score to legal_scores_array.
 * now between the legal values, choose one by their scores and put in board (see choose_value_by_probability).
 * if one of the conditions wasnt meet, the board was failed to be filled.
 * else, the board was legally filled.
 *
 *
 * @param board - the board we want to fill.
 * @param map - the map corresponds to the board.
 * @param sol - the scores we got from the LP/ILP algorithm.
 * @param threshold - the threshold to cut all the scores below it.
 *
 * @return TRUE if board is fully filled
 * 		   FALSE else.
 */

int put_sol_in_board(BOARD *board,large_array_struct *map, double *sol, double threshold){
	int i,j,k;
	int map_index;
	int val = 0;
	int num_of_legal_values = 0;
	int res = TRUE, *legal_values_array = NULL;
	double *legal_scores_array = (double *)calloc(map->nXm,sizeof(double));
	if(legal_scores_array == NULL){
		print_system_error(1,"error in allocating memory for legal_scores_array");
	}
	legal_values_array = (int *)calloc(map->nXm,sizeof(int));
	if(legal_values_array == NULL){
		print_system_error(1,"error in allocating memory for legal_values_array");
	}
	for(i = 0; i<map->nXm; i++){
		for(j = 0; j<map->nXm; j++){
			for(k = 1; k<=map->nXm; k++){
				map_index = get_int_from_large_array(map,i,j,k-1);
				if(map_index>-1){
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
				val = choose_value_by_probability(legal_scores_array,legal_values_array,num_of_legal_values);
				set_element_to_board(board,j,i,val);
			}else{
				if (map_index != -1) {
					res = FALSE;
				}
			}
			num_of_legal_values = 0;
		}
	}
	free(legal_scores_array);
	free(legal_values_array);
	return res;
}

/*
 * this function defined in the .h file.
 */
void get_hint(large_array_struct *map,double *sol, int x,int y,double *scores){
	int map_index;
	int k;
	for (k=0;k<map->nXm;k++){
		map_index = get_int_from_large_array(map,y,x,k);
		if(map_index == -1){
			scores[k] = 0;
		}else{
			scores[k] = sol[map_index];
		}
	}
}
/*
 * This function is making the environment and the model for the gurobi run.
 *
 * @param model - the model to build.
 * @param env - the environment to build.
 * @param error_string - a buffer to put the error message in.
 *
 */
void make_new_model(GRBmodel **model, GRBenv **env, char *error_string){
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

/*
 * This function is initializing the arrays needed for the gurobi.
 *
 * @param vtype - array of types of each variables.
 * @param obj - array of coefficients of the variables in the obj function.
 * @param val - array of coefficients of the variables in each constraint. (in our case, it is only 1 for all vars)
 * @param num_of_vars - number of variables in this model.
 * @param is_binary - if TRUE, all variables will be 0 or 1, else continues between 0 to 1.
 * @param nXm - N*M (dimensions of board).
 */
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

/*
 * This function is initializing the environment and the model for the gurobi run.
 *
 * @param model - the model to build.
 * @param env - the environment to build.
 * @param error_string - a buffer to put the error message in.
 * @param vtype - array of types of each variables.
 * @param obj - array of coefficients of the variables in the obj function.
 * @param val - array of coefficients of the variables in each constraint.
 * @param num_of_vars - number of variables in this model.
 */
void initialize_model(GRBmodel *model,GRBenv *env,char *error_string,char *vtype,double *obj,double *val,int num_of_var){
	int error = GRBaddvars(model, num_of_var, 0, NULL, NULL, NULL, obj, NULL, val, vtype, NULL); /* add variables to model */
	if (error) {
	  sprintf(error_string,"ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
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
/*
 * This function is putting the constraints into the model for the gurobi run.
 *
 * @param model - the model to build.
 * @param env - the environment to build.
 * @param board - the board we are solving.
 * @param error_string - a buffer to put the error message in.
 * @param map - the map corresponds to this board.
 * @param ind - the array that will represent the constraint.
 * @param val - array of coefficients of the variables in each constraint.
 */
void put_constraints_in_model(GRBmodel *model,GRBenv *env,BOARD *board,char *error_string,large_array_struct *map, int *ind,double *val){
	int i,j,k,constraint_len = 0,error = 0;
	for (k=1;k<=map->nXm;k++){
		constraint_helper(model,env,board,map,k,ind,val,1,error_string);
		constraint_helper(model,env,board,map,k,ind,val,2,error_string);
		constraint_helper(model,env,board,map,k-1,ind,val,3,error_string);
		for(i=0;i<board->N;i++){
			for(j=0;j<board->M;j++){
				constraint_len = block_constraint(board,map,i,j,k,ind);
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
/*
 * This function is optimizing the model, and returning the status of the run "optimstatus".
 *
 * @param model - the model.
 * @param env - the environment.
 * @param error_string - a buffer to put the error message in.
 *
 * @return the status of the run.
 */
int optimize_model(GRBmodel *model,GRBenv *env,char *error_string){
	int error = GRBoptimize(model);/* Optimize model - need to call this before calculation */
	int optimstatus = 0;
	if (error) {
		sprintf(error_string,"ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		print_system_error(2,error_string);
	}
	error = GRBwrite(model, "mip1.lp");/*Write model to 'mip1.lp' - this is not necessary but very helpful*/
	if (error) {
		sprintf(error_string,"ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		print_system_error(2,error_string);
	}
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);/*   Get solution information*/
	if (error) {
		sprintf(error_string,"ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		print_system_error(2,error_string);
	}
	return optimstatus;
}

/*
 * This function is putting the constraints into the model for the gurobi run.
 *
 * @param model - the model.
 * @param env - the environment.
 * @param sol - an array to hold the scores we got from the LP/ILP algorithm.
 * @param error_string - a buffer to put the error message in.
 * @param num_of_vars - number of variables in this model.
 *
 */
void get_solution(GRBmodel *model,GRBenv *env, double *sol , char *error_string,int num_of_var){
	double objval = 0;
	int error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error) {
	  sprintf(error_string,"ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
	  print_system_error(2,error_string);
	}
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, num_of_var, sol);/*   get the solution - the assignment to each variable*/
	if (error) {
	  sprintf(error_string,"ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
	  print_system_error(2,error_string);
	}
}

/*
 * This function is getting the board, and his map,
 * and using gurobi library to run LP/ILP
 * algorithm in order to get a solution to a board.
 *
 *
 * @param board - the board we want to solve.
 * @param num_of_vars - number of variables in this model.
 * @param map - the map corresponds to the board.
 * @param is_binary - TRUE for ILP, FALSE for LP.
 * @param sol - the array to hold the scores if the run was a success (if the model has a solution).
 *
 * @return TRUE if solution was found
 * 		   FALSE else
 */

int gurobi(BOARD *board,int num_of_vars,large_array_struct *map, int is_binary, double *sol)
{
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int       error = 0, optimstatus = 0, constraint_len = 0,res;
  char 		error_string[1000];
  int 		i,j,k;
  int       *ind = (int*)malloc(map->nXm*sizeof(int));
  double    *val = (double*)malloc(num_of_vars*sizeof(double));
  double    *obj = (double*)malloc(num_of_vars*sizeof(double));
  char      *vtype = (char*)malloc(num_of_vars*sizeof(char));
  double    objval = 0;
  if(ind==NULL || val == NULL ||obj ==NULL || vtype==NULL){
	  print_system_error(1,"error in allocating memory for gurobi arrays");
  }
  make_new_model(&model,&env,error_string);
  initialize_arrays_for_gurobi(vtype,obj,val,num_of_vars,is_binary,map->nXm);
  initialize_model(model,env,error_string,vtype,obj,val,num_of_vars);
  put_constraints_in_model(model,env,board,error_string,map,ind,val);
  optimstatus = optimize_model(model,env,error_string);
  if(optimstatus != GRB_OPTIMAL){ /*no solution*/
	  res = FALSE;
  }else{
	  res = TRUE;
	  get_solution(model,env,sol,error_string,num_of_vars);
  }
  GRBfreemodel(model);
  GRBfreeenv(env);
  free(ind);
  free(obj);
  free(vtype);
  free(val);
  return res;
}




