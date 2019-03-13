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

void print_array(int *arr,int length){ /*helper function for debug, prints an array*/
	int i;
	for (i = 0;i<length;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
}
