/*
 * MainAux.h
 *
 *  Created on: 12 בדצמ 2018
 *      Author: nir
 */
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAINAUX_H_
#define MAINAUX_H_

#define TRUE 1
#define FALSE 0
#define TERMINATE -1

enum Command {Mark_errors = 1, Guess = 2, Hint = 3, Guess_hint = 4, Autofill = 5, Print_board = 6, Set = 7, Validate = 8, \
	Undo = 9, Redo = 10, Num_solutions = 11, Reset = 12, Save = 13, Solve = 14, Edit = 15, Exit = 16, Generate = 17};
enum Mode {INIT = 1,EDIT = 2,SOLVE = 3};
enum Cell_Status {NORMAL, FIXED, ERRONEOUS};

int is_white_space(char c);
int is_in_range(int val,int min,int max);
void print_array(int *arr,int length); /*helper function for debug, prints an array*/
void print_array_double(double *arr,int length);

#endif /* MAINAUX_H_ */
