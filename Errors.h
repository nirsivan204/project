/**
 * Errors module -  contains error messages printed for the user. This module supports the following functions:
 *
 * print_system_error    		- prints an error that was caused by a system error.
 * print_invalid_command_error 	- prints an error that was caused by an invalid input given by the user.
 * print_invalid_file_error 	- prints an error that was caused by problems in file handling.
 * print_invalid_move_error 	- prints an error that was caused by invalid moves.
 */
#include "MainAux.h"
#include <errno.h>

#ifndef ERRORS_H_
#define ERRORS_H_

/*
 * this function prints a relevant system error message.
 *
 * @param error_number 	- a number that represents a specific type of errors.
 * @param function_name - a string that describes a specific problem.
 *
 */
void print_system_error(int error_number, char* function_name);

/*
 * this function prints an error that was caused by an invalid input given by the user.
 *
 * @param error_number 	- a number that represents a specific type of errors.
 * @param arg1 			- a parameter used for the error message.
 * @param arg2 			- a parameter used for the error message.
 * @param arg3 			- a parameter used for the error message.
 * @param arg4 			- a parameter used for the error message.
 */
void print_invalid_command_error(int error_number, int arg1, int arg2, int arg3, int arg4);

/*
 * this function prints an error that was caused by problems in file handling.
 *
 * @param error_number 	- a number that represents a specific type of errors.
 */
void print_invalid_file_error(int error_number);

/*
 * this function prints an error that was caused by invalid moves.
 *
 * @param error_number 	- a number that represents a specific type of errors.
 * @param arg1 			- a parameter used for the error message.
 * @param arg2 			- a parameter used for the error message.
 * @param arg3 			- a parameter used for the error message.
 */
void print_invalid_move_error(int error_number, int arg1, int arg2, int arg3);

#endif /* ERRORS_H_ */
