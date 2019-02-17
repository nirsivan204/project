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
