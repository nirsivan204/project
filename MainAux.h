/*
 * MainAux.h
 *
 *  Created on: 12 בדצמ 2018
 *      Author: nir
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#define TRUE 1
#define FALSE 0

enum Mode {INIT = 1,EDIT = 2,SOLVE = 3};

int is_white_space(char c);
int is_in_range(int val,int min,int max);


#endif /* MAINAUX_H_ */
