/*
 * Errors.h
 *
 *  Created on: Mar 3, 2019
 *      Author: ebay
 */
#include "MainAux.h"

#ifndef ERRORS_H_
#define ERRORS_H_

void print_system_error(int error_number);

void print_invalid_command_error(int error_number, int arg1, int arg2, int arg3, int arg4);

void print_invalid_file_error(int error_number);

void print_invalid_move_error(int error_number, int arg1, int arg2);

#endif /* ERRORS_H_ */
