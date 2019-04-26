/**
 * LargeArray module - This module contains a struct that will replace large continues memory allocation with smaller memory allocations. This module supports the following functions:
 *
 * get_int_from_large_array    - returning an element from the array struct
 * set_int_in_large_array      - setting an element to the array struct
 * init_large_array_struct     - initializing a large array struct.
 * delete_large_array          - deleting and releasing memory of a large array struct.
 */
#include "Errors.h"
#ifndef LARGEARRAY_H_
#define LARGEARRAY_H_

typedef struct large_array_struct{ /*a struct that will hold large array in non continues place in memory*/
	int ***int_values;
	int nXm;
}large_array_struct;


/*
 * This function is getting an element from the array struct
 *
 * @param arr - the struct.
 * @param i - first index.
 * @param j - second index.
 * @param k - third index.
 *
 * @return the element in array[i][j][k]
 *
 */
int get_int_from_large_array(large_array_struct *arr,int i, int j,int k);

/*
 * This function is setting an element in the array struct
 *
 * @param arr - the struct.
 * @param i - first index.
 * @param j - second index.
 * @param k - third index.
 * @param value - the value to put in struct.
 *
 *
 */
void set_int_in_large_array(large_array_struct *arr,int i, int j,int k, int value);

/*
 * This function is initializing an large_array_struct. this struct will hold (N*M)^3 elements, in 3D.
 *
 * @param nXm - the size of each dimension.
 *
 * @return the initalized struct.
 */
large_array_struct *init_large_array_struct(int nXm);

/*
 * This function is deleting an large_array_struct and releasing all its memory.
 *
 * @param arr - the struct to delete.
 *
 */
void delete_large_array(large_array_struct *arr);


#endif /* LARGEARRAY_H_ */
