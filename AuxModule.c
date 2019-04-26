#include "AuxModule.h"
/*
 * this function tells if a char is a whitespace or not.
 * @param c    - the char to check
 * return TRUE if c is whitespace
 * 	      FALSE else.
 *
 */
int is_white_space(char c){
	return c=='\n'|| c==' ' || (int)c==9 || (int)c==13;
}
/*
 * this function tells if a val is between min and max.
 * @param val    - the value to check
 * @param min    - the minimum of the range
 * @param max    - the maximum of the range
 * return TRUE if min<=val<=max
 * 	      FALSE else.
 *
 */
int is_in_range(int val,int min,int max){
	return val<=max && val>=min;
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
