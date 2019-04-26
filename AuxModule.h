/**
 * AuxModule module - This module contains all the auxiliary functions that all the other modules need. This module supports the following functions:
 *
 * is_white_space    - tells if a char is a whitespace or not.
 * is_in_range       - tells if a val is between min and max.
 * get_rand_number   - return a random number in a desired range.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifndef MAINAUX_H_
#define MAINAUX_H_

#define TRUE 1
#define FALSE 0
#define TERMINATE -1

enum Command {Mark_errors = 1, Guess = 2, Hint = 3, Guess_hint = 4, Autofill = 5, Print_board = 6, Set = 7, Validate = 8, \
	Undo = 9, Redo = 10, Num_solutions = 11, Reset = 12, Save = 13, Solve = 14, Edit = 15, Exit = 16, Generate = 17};
enum Mode {INIT = 1,EDIT = 2,SOLVE = 3};
enum Cell_Status {NORMAL, FIXED, ERRONEOUS};
/*
 * this function tells if a char is a whitespace or not.
 * @param c    - the char to check
 * return TRUE if c is whitespace
 * 	      FALSE else.
 *
 */
int is_white_space(char c);
/*
 * this function tells if a val is between min and max.
 * @param val    - the value to check
 * @param min    - the minimum of the range
 * @param max    - the maximum of the range
 * return TRUE if min<=val<=max
 * 	      FALSE else.
 *
 */
int is_in_range(int val,int min,int max);
/*
 * this function return a random number in a desired range
 * @param range    - the range
 *
 * @return a random number between 0 to range-1 (include)
 */
int get_rand_number(int range);
#endif /* MAINAUX_H_ */
