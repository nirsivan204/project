/*
 * MainAux.c
 *
 *  Created on: 12 בדצמ 2018
 *      Author: nir
 */
#include "MainAux.h"

int is_white_space(char c){
	return c=='\n'|| c==' ' || (int)c==9 || (int)c==13;
}

int is_in_range(int val,int min,int max){
	return val<=max && val>=min;
}

/*
 * (for debug only)
 * this function prints an array
 * @param arr    - the array to print
 * @param length - how many cells to print
 *
 */
void print_array_double(double *arr,int length){
	int i;
	for (i = 0;i<length;i++){
			printf("%f ",arr[i]);
	}
	printf("\n");
}

void print_array(int *arr,int length){ /*helper function for debug, prints an array*/
	int i;
	for (i = 0;i<length;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
}


/*
 * this function return a random number in a desired range
 * @param range    - the range
 *
 * @return a random number between 0 to range-1 (include)
 */
int get_rand_number(int range){ /*returning a random number between 0 to range-1*/
	return rand()%range;
}
