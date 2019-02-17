/*
 * ReadingAux.c
 *
 *  Created on: Feb 14, 2019
 *      Author: ebay
 */
#include "ReadingAux.h"

int is_white_space(char c){
	return c=='\n'|| c==' ' || (int)c==9 || (int)c==13;
}
