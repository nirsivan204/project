#include "LargeArray.h"
/*
 * this function defined in the .h file.
 */
int get_int_from_large_array(large_array_struct *arr,int i, int j,int k){
	return arr->int_values[i][j][k];
}
/*
 * this function defined in the .h file.
 */
void set_int_in_large_array(large_array_struct *arr,int i, int j,int k, int value){
	arr->int_values[i][j][k] = value;
}
/*
 * this function defined in the .h file.
 */
large_array_struct *init_large_array_struct(int nXm){
	int i,j;
	large_array_struct *res = (large_array_struct *)malloc(sizeof(large_array_struct));
	if(res == NULL){
		 print_system_error(1,"error in allocating memory for large_array_struct");
	}
	res->nXm = nXm;
	res->int_values = (int ***)malloc(res->nXm*sizeof(int**));
	if(res->int_values == NULL){
		 print_system_error(1,"error in allocating memory for large_array_struct");
	}
	for(i = 0;i<res->nXm ;i++){
		res->int_values[i] = (int **)malloc(res->nXm*sizeof(int*));
		if(res->int_values[i] == NULL){
			 print_system_error(1,"error in allocating memory for large_array_struct");
		}
		for(j = 0;j<res->nXm;j++){
			res->int_values[i][j] = (int *)calloc(res->nXm,sizeof(int));
			if(res->int_values[i][j] == NULL){
				 print_system_error(1,"error in allocating memory for large_array_struct");
			}
		}
	}
	return res;
}
/*
 * this function defined in the .h file.
 */
void delete_large_array(large_array_struct *arr){
	int i,j;
	if(arr !=NULL){
		for(i=0;i<arr->nXm;i++){
			if(arr->int_values[i] !=NULL){
				for(j=0;j<arr->nXm;j++){
					if(arr->int_values[i][j] !=NULL){
						free(arr->int_values[i][j]);
					}
				}
				free(arr->int_values[i]);
			}
		}
		free(arr->int_values);
	}
	free(arr);
}
