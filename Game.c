#include "Game.h"

/*
 *  this function prints an announcement of the beginning of the game.
 */
void initialize_puzzle() {
	printf("Hello! Welcome to Sudoku!\n");
}

/*
 * if a given condition/result is FALSE, this function prints a relevant error message.
 *
 * @param result		- a number that determines whether a move is invalid and therefore an error message is to be printed (FALSE) or not (TRUE).
 * @param error_number 	- a number that represents a specific type of errors.
 * @param arg1 			- a parameter used for the error message.
 * @param arg2 			- a parameter used for the error message.
 * @param arg3 			- a parameter used for the error message.
 *
 * @return result.
 */
int validate_move(int result, int error_number, int arg1, int arg2, int arg3) {
	if (!result) {
		print_invalid_move_error(error_number,arg1,arg2,arg3);
	}
	return result;
}

/*
 * this function executes the 'hint' command: gets the value of a cell in the current solution-board, and prints it as a hint for the user.
 *
 * @param solution_board	- a pointer to a board that contains a solution to the current puzzle.
 * @param x 				- the required cells's column.
 * @param x 				- the required cells's row.
 *
 */
void hint(BOARD *solution_board, int x, int y) {
	printf("According to current board's solution, value of cell <%d,%d> should be %d.\n",x,y,get_element_from_board(solution_board,x-1,y-1));
}

/*
 * this function executes the 'guess_hint' command.
 *
 * @param map			- .
 * @param sol			- .
 * @param x 			- the required cells's column.
 * @param y 			- the required cells's row.
 * @param nXm			- a number that represents the number of cells in each row, column and block.
 * @param nXm_square	- a number that represents the total number of cells in the puzzle.
 *
 */
void guess_hint(int *map, double *sol, int x, int y, int nXm, int nXm_square) {
	int i;
	double *scores = (double*)calloc(nXm, sizeof(double));
	if(scores == NULL){
		print_system_error(1,"error in allocating an array for the scores");
	}
	get_hint(map,sol,x-1,y-1,nXm,nXm_square,scores);
	i = 0;
	while (i < nXm && scores[i] == 0) {
		i++;
	}
	if (i < nXm) {
		printf("Legal values of cell <%d,%d>:\n", x, y);
		for (i = 1; i<=nXm; i++) {
			if (scores[i-1] > 0) {
				printf("%d with score of: %f\n",i, scores[i-1]);
			}
		}
	}
	else {
		printf("Statistically, cell <%d,%d> has no legal values with score greater than 0.\n", x, y);
	}
	free(scores);
}

/**
 * this function executes the 'validate' command.
 * @param isSolvable - a number that determines whether the puzzle has a valid solution (if TRUE) or not (if FALSE).
 */
void validate(int isSolvable) {
	printf(isSolvable ? "Validation passed: board is solvable.\n" : "Validation failed: board is unsolvable.\n");
}

/**
 * this function executes the 'exit' command: prints an exit message and terminate the program.
 * @return -1.
 */
int exit_game() {
	printf("Goodbye! Thank you for playing Sudoku!\n");
	return TERMINATE;
}

/*
 *  this function counts the number of empty cells in the puzzle.
 *  @param board	- a pointer to the current puzzle.
 *
 *  @return that number.
 */
int count_empty_cells(BOARD* board) {
	int i, j, count = 0;
	for (i=0;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++){
			if (get_element_from_board(board,i,j) == 0) {
				count++;
			}
		}
	}
	return count;
}

/*
 *  this function updates the number of empty cells in the puzzle, checks if the board is full.
 *  If so, then it prints an appropriate message depends on whether the solution is correct or erroneous.
 *  @param board			- a pointer to the current puzzle.
 *  @param mode				- a pointer to a number that represents one of the 3 playable modes.
 *  @param isValidBoard		- a number that determines whether there are errors in the puzzle.
 *  @param numOfEmptyCells	- a pointer to a number that represents the current number of empty cells in the puzzle.
 */
void update_num_of_empty_cells(BOARD *board, int *mode, int isValidBoard, int *numOfEmptyCells) {
	*numOfEmptyCells = count_empty_cells(board);
	if (*mode == SOLVE && *numOfEmptyCells == 0) { /* board is full */
		if (isValidBoard) {
			printf("Well done! The puzzle was solved correctly! Please start a new puzzle, or exit.\n");
			*mode = INIT;
		}
		else {
			printf("The solution is erroneous! Try to correct erroneous cells.\n");
		}
	}
}

/*
 *  this function prints an opening message for future updates to follow.
 *  @param command	- a number that represents the 'undo', 'redo' or the 'autofill' command.
 */
void print_start_update(int command) {
	char *command_name = NULL;
	switch (command) {
	case Undo: command_name = "undo"; break;
	case Redo: command_name = "redo"; break;
	default: command_name = "autofill"; break;
	}
	printf("Board updates following '%s':\n", command_name);
}

/*
 *  this function prints a message describing a change in value of a cell.
 *  @param x	- the required cells's column.
 *  @param y	- the required cells's row.
 *  @param old	- the required cells's old value.
 *  @param new	- the required cells's new value.
 */
void print_cell_update(int x, int y, int old, int new) {
	printf("- value of cell <%d,%d> has changed from %d to %d.\n", x, y, old, new);
}

/*
 *  this function checks whether at least one cell has been updated following a move, and updates a given pointer accordingly if so.
 *  @param count			- the number of cells that has been updated.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 */
void update_count(int count, int* isUpdatedBoard) {
	if (count > 0) {
		*isUpdatedBoard = FALSE;
	}
}

/*
 *  this function notify the user about the number of updated cells following a move, and possibly about the current location in the undo/redo list.
 *  @param command			- a number that represents the 'undo', 'redo' or the 'autofill' command.
 *  @param character		- a letter that represents the last move executed on the current board.
 *  @param count			- the number of cells that has been updated.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 *
 *	calls the 'update_count' method and prints count as a message for the user.
 *	Then, if the command is 'undo' or 'redo', prints the last move executed on board (or that the board has no previous moves).
 */
void print_finish_update(int command, char character, int count, int *isUpdatedBoard) {
	char *command_name = NULL;
	update_count(count, isUpdatedBoard);
	printf("Number of updated cells in this move: %d. ", count);
	if (command != Autofill) {
		switch (character) {
		case 's': command_name = "set"; break;
		case 'a': command_name = "autofill"; break;
		case 'g': command_name = "guess"; break;
		case 'n': command_name = "generate"; break;
		default: printf("Board is now at its original state.\n"); return; /*character == 'o'*/
		}
		printf("Last move executed on board is '%s'.", command_name);
	}
	printf("\n");
}

/*
 *  this function updates a given board to be a copy of another board, with an option of notifying the user about all updated cells.
 *  @param copy_to_board	- a pointer to a board that needs to be updated.
 *  @param copy_from_board	- a pointer to a board that copy_to_board is updated according to.
 *  @param withOutput		- a number that determines whether to print updates (if TRUE) or not (if FALSE).
 *
 *  @return the number of cells that has changed their value to a different value.
 */
int update_changes_in_board(BOARD *copy_to_board, BOARD *copy_from_board, int withOutput) {
	int i, j, copy_to_val, copy_from_val, count = 0;
	for (i=0;i<copy_to_board->M*copy_to_board->N;i++){
		for (j=0;j<copy_to_board->M*copy_to_board->N;j++) {
			copy_to_val = get_element_from_board(copy_to_board, i, j);
			copy_from_val = get_element_from_board(copy_from_board, i, j);
			if (copy_to_val != copy_from_val) {
				count++;
				set_element_to_board(copy_to_board, i, j, copy_from_val);
				if (withOutput) {
					print_cell_update(i+1, j+1, copy_to_val, copy_from_val);
				}
			}
		}
	}
	return count;
}

/*
 *  this function executes the 'undo' and 'redo' commands.
 *  @param list				- a pointer to a list of the user's moves.
 *  @param board			- a pointer to the current puzzle.
 *  @param command			- a number that represents the 'undo' command or the 'redo' command.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 *
 *	calls the 'move_in_command_list' method. If it returns TRUE, then board is updated according to the current list move's board.
 *	Every change is printed.
 *
 *  @return 1 if the command was executed successfully ('move_in_command_list' returns TRUE), 0 otherwise.
 */
int undo_or_redo(list *list, BOARD *board, int command, int* isUpdatedBoard) {
	int count;
	BOARD *list_board = NULL;
	node *node = NULL;
	int res = move_in_command_list(list,command);
	if (!validate_move(res, 5, command, 0, 0)) {
		return FALSE;
	}
	node = list->current_command;
	list_board = get_curr_board(list);
	print_start_update(command);
	count = update_changes_in_board(board,list_board,TRUE);
	print_finish_update(command,get_curr_command(node),count, isUpdatedBoard);
	return TRUE;
}

/*
 *  this function executes the 'reset' command:
 *  @param list				- a pointer to a list of the user's moves.
 *  @param board			- a pointer to the current puzzle.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 *
 *	updates the pointer of the list to point on NULL (which means at the beginning of the list), and updates the board to be at its original state.
 */
void reset(list *list, BOARD *board, int* isUpdatedBoard) {
	if (list->current_command != NULL) {
		list->current_command = NULL;
		update_count(update_changes_in_board(board, list->original_board, FALSE), isUpdatedBoard);
	}
}

/*
 *  this function executes the 'set' command.
 *  @param board 			- a pointer to the current puzzle.
 *  @param x 				- the required cells's column.
 *  @param y 				- the required cells's row.
 *  @param z 				- the required cells's new given value.
 *  @param isValidBoard		- a pointer to a number that determines whether there are errors in the puzzle.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 */
void set(BOARD *board, int x, int y, int z, int* isValidBoard, int* isUpdatedBoard){
	set_element_to_board(board,x,y,z);
	if (*isValidBoard == FALSE || !is_valid_insertion(board,x,y,z)) {
		*isUpdatedBoard = FALSE;
	}
}

/*
 *  this function checks if a given cell has exactly one legal value, and returns it if so.
 *  @param board	- a pointer to the current puzzle.
 *  @param x		- the required cells's column.
 *  @param y		- the required cells's row.
 *  @param nXm		- the number of cells in each row, column and block.
 *
 *	for every legal cell value in the puzzle, the function calls the 'is_valid_insertion_to_empty_cell' method.
 *	If it returns TRUE, the return value of this function is updated (so if it happens again, then it means the cell has more than one legal value).
 *
 *  @return 0 if the return value of this function has been updated more than once, or hasn't been updated at all.
 *  		Otherwise, returns the legal value of the cell.
 */
int get_only_legal_value(BOARD *board, int x, int y, int nXm) {
	int value = 0, digit = 1;
	while (digit <= nXm){
		if (is_valid_insertion_to_empty_cell(board,x,y,digit)) {
			if (value > 0) {
				return 0; /* more than one legal value */
			}
			value = digit;
		}
		digit++;
	}
	return value;
}

/*
 *  this function executes the 'autofill' command.
 *  @param board			- a pointer to the current puzzle.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 *  @param nXm				- the number of cells in each row, column and block.
 *
 *  for every empty cell in board, if it has only one legal value (using the 'get_only_legal_value' method),
 *  the function sets the value's complement to the cell (so future empty cells won't be affected by the update).
 *  Afterwards, for every updated (negative) cell, the function sets its value's (positive) complement to it.
 *  Every update is printed.
 */
void autofill(BOARD *board, int* isUpdatedBoard, int nXm) {
	int i, j, val, count = 0;
	print_start_update(Autofill);
	for (i=0;i<nXm;i++){
		for (j=0;j<nXm;j++){
			if (get_element_from_board(board,i,j) == 0) {
				val = get_only_legal_value(board,i,j,nXm);
				if (val > 0) {
					set_element_to_board(board, i, j, -val);
					count++;
					print_cell_update(i+1, j+1, 0, val);
				}
			}
		}
	}
	print_finish_update(Autofill, 0, count, isUpdatedBoard);
	for (i=0;i<nXm;i++){
		for (j=0;j<nXm;j++){
			val = get_element_from_board(board,i,j);
			if (val < 0) {
				set_element_to_board(board, i, j, -val);
			}
		}
	}
}

/*
 *  this function randomly selects a number from a given range.
 *  @param range - the top boundary.
 *  @return a random integer between 0 and range-1.
 */
int get_random_number(int range){ /*returning a random number between 0 to range-1*/
	return rand()%range;
}

/*
 *  this function fills a given array with all empty cells in the current puzzle.
 *  @param board		- a pointer to a copy of the current puzzle.
 *  @param empty_cells	- the given array.
 *  @param nXm			- the number of cells in each row, column and block.
 */
void fill_array_with_empty_cells(BOARD* board, int empty_cells[], int nXm) {
	int i, j, k=0;
	for (i=0;i<nXm;i++){
		for (j=0;j<nXm;j++){
			if (get_element_from_board(board,i,j) == 0) {
				empty_cells[k++] = j*nXm+i;
			}
		}
	}
}

/*
 *  this function randomly fills a given array with random values (representing empty cells) chosen from another array.
 *  @param copy_all				- an array of all empty cells in the puzzle.
 *  @param selected_empty_cells	- an array that will contain a specific number of empty cells, chosen randomly from copy_all.
 *  @param numOfEmptyCells		- a number that represents the current number of empty cells in the puzzle.
 *  @param x					- the number of empty cells chosen randomly from copy_all.
 */
void choose_random_empty_cells(int copy_all[], int selected_empty_cells[], int numOfEmptyCells, int x) {
	int i, rand_index;
	for (i=0;i<x;i++) {
		rand_index = get_random_number(numOfEmptyCells);
		selected_empty_cells[i] = copy_all[rand_index];
		copy_all[rand_index] = copy_all[--numOfEmptyCells];
	}
}

/*
 *  for each cell represented in a given array, this function empties it from the puzzle.
 *  @param board				- a pointer to a copy of the current puzzle.
 *  @param selected_empty_cells	- an array of cells.
 *  @param limit				- a number that determines how many cells to empty.
 *  @param nXm					- a number that represents the number of cells in each row, column and block.
 */
void emtpy_cells(BOARD* board, int selected_empty_cells[], int limit, int nXm) {
	int index, x, y, cell_num;
	for (index=0; index<limit; index++) {
		cell_num = selected_empty_cells[index];
		x = cell_num%nXm;
		y = cell_num/nXm;
		set_element_to_board(board, x, y, 0);
	}
}

/*
 *  this function fills an empty cell with a random legal value.
 *  @param board	- a pointer to a copy of the current puzzle.
 *  @param cell_num	- a number that represents an empty cell.
 *  @param digits	- an empty array, will be used to storage all legal cell values.
 *  @param nXm		- a number that represents the number of cells in each row, column and block.
 *
 *  first, the function fills digits with all legal cell values.
 *  Then, it randomly chooses an index in digits. If the selected element is a legal value for the cell, it is filled with that value in board.
 *  Otherwise, it sets the final element in digits instead of the selected value, and the process repeats itself, when each time the number of indices that could be selected from the array decreases by 1.
 *
 *  @return 1 if the cell has been successfully filled, 0 otherwise (if the cell has no legal values).
 */
int fill_cell_with_random_legal_value(BOARD* board, int cell_num, int digits[], int nXm) {
	int i, x, y, rand_index, rand_digit;
	x = cell_num%nXm;
	y = cell_num/nXm;
	for (i = 0; i < nXm; i++) {
		digits[i] = i+1;
	}
	while (nXm > 0) {
		rand_index = get_random_number(nXm);
		rand_digit = digits[rand_index];
		if (is_valid_insertion_to_empty_cell(board,x,y,rand_digit)) {
			set_element_to_board(board,x,y,rand_digit);
			return TRUE;
		}
		digits[rand_index] = digits[--nXm]; /*delete digit from array and repeat*/
	}
	return FALSE;
}

/*
 *  this function randomly fills empty cells with legal values and then solves the puzzle with ILP.
 *  @param board				- a pointer to a copy of the current puzzle.
 *  @param all_empty_cells		- an array of all empty cells in the puzzle.
 *  @param copy_all				- an empty array, will be used as a copy of all_empty_cells.
 *  @param selected_empty_cells	- an empty array, will be used to storage every randomly selected empty cell.
 *  @param digits				- an empty array, will be used to storage all legal cell values.
 *  @param x					- the number of empty cells needed to be filled before solving the puzzle.
 *  @param numOfEmptyCells		- a number that represents the current number of empty cells in the puzzle.
 *  @param nXm					- a number that represents the number of cells in each row, column and block.
 *  @param nXm_square			- a number that represents the total number of cells in the puzzle.
 *
 *  first, the function copies all_empty_cells to copy_all.
 *  Then, it fills selected_empty_cells with x randomly chosen empty cells (using the 'choose_random_empty_cells' method).
 *  Then, for each chosen cell it calls the 'fill_cell_with_random_legal_value' method, and if a call returns FALSE, all changes made are canceled.
 *  Otherwise, if all cells has been successfully filled, the function allocates memory for every variable needed for solving the puzzle, and then
 *  it calls the 'gurobi' and 'put_sol_in_board' methods to get a solution with ILP. If both methods return TRUE, then the puzzle is solvable and board contains its solution.
 *  Otherwise, all changes made are canceled.
 *
 *  @return 1 if all cells has been successfully filled and the puzzle was solved, 0 otherwise (if at any point, all changes made are canceled with the 'emtpy_cells' method).
 */
int fill_x_cells_and_solve(BOARD* board, int all_empty_cells[], int copy_all[], int selected_empty_cells[], \
		int digits[], int x, int numOfEmptyCells, int nXm, int nXm_square) {
	double *sol = NULL;
	int index, *map = NULL ,num_of_vars, gurobi_result;
	gurobi_result = TRUE;
	for (index=0; index<numOfEmptyCells; index++) {
		copy_all[index] = all_empty_cells[index];
	}
	choose_random_empty_cells(copy_all, selected_empty_cells, numOfEmptyCells, x);
	for (index=0; index<x; index++) {
		if (!fill_cell_with_random_legal_value(board, selected_empty_cells[index], digits, nXm)) {
			emtpy_cells(board, selected_empty_cells, index, nXm);
			return FALSE;
		}
	}
	map = (int*)calloc(nXm*nXm_square, sizeof(int));
	if(map == NULL){
		print_system_error(1,"error in allocating memory for map");
	}
	num_of_vars = map_maker(board, map, nXm, nXm_square);
	sol = (double*)calloc(num_of_vars, sizeof(double));
	if(sol == NULL){
		print_system_error(1,"error in allocating memory for scores");
	}
	if (gurobi(board,num_of_vars,map,TRUE,sol) != TRUE || !put_sol_in_board(board,map,sol,nXm,nXm_square,0)) {
		emtpy_cells(board, selected_empty_cells, x, nXm);
		gurobi_result = FALSE;
	}
	free(map);
	free(sol);
	return gurobi_result;
}

/*
 *  this function randomly chooses cells from a solved puzzle and empty them.
 *  @param board			 - a pointer to a board that contains a solution to the current puzzle.
 *  @param y				 - the number of non-empty cells the puzzle needs to contain eventually.
 *  @param nXm				 - a number that represents the number of cells in each row, column and block.
 *  @param curr_num_of_cells - a number that represents the total number of cells in the puzzle.
 *
 *  first, the function creates an array of all cells in the puzzle.
 *  Then, it randomly chooses an index in the array, empties the selected element from board and sets the final element in the array instead of the selected one.
 *  The process described last line repeats itself nXm-y times, when each time the number of indices that could be selected from the array decreases by 1.
 */
void empty_all_but_y_cells(BOARD* board, int y, int nXm, int curr_num_of_cells) {
	int i, rand_index, rand_cell_num, all_cells[curr_num_of_cells];
	for (i = 0; i < curr_num_of_cells; i++) {
		all_cells[i] = i;
	}
	while (curr_num_of_cells > y) { /*choose (nXm - y) cells and empty them*/
		rand_index = get_random_number(curr_num_of_cells);
		rand_cell_num = all_cells[rand_index];
		set_element_to_board(board, rand_cell_num%nXm, rand_cell_num/nXm, 0);
		all_cells[rand_index] = all_cells[--curr_num_of_cells];
	}
}

/*
 *  this function executes the 'generate' command.
 *  @param board			- a pointer to the current puzzle.
 *  @param solution_board	- a pointer to a board that contains a solution to the current puzzle.
 *  @param x				- the number of empty cells needed to be filled before solving the puzzle.
 *  @param y				- the number of non-empty cells the puzzle needs to contain eventually.
 *  @param numOfEmptyCells	- a number that represents the current number of empty cells in the puzzle.
 *  @param nXm				- a number that represents the number of cells in each row, column and block.
 *  @param nXm_square		- a number that represents the total number of cells in the puzzle.
 *
 *  first, the function copies the current puzzle to solution_board (the solution is no longer needed) and calls the 'fill_array_with_empty_cells' method.
 *  Then, it tries to solve the puzzle after successfully fill x random empty cells with legal values (using the 'fill_x_cells_and_solve' method).
 *  If within 1000 attempts a solution has been found, then all cells are emptied from the solution except y random cells (using the 'empty_all_but_y_cells' method).
 *  Otherwise, the generation fails and an error message is printed.
 *
 *  @return 1 if a puzzle was generated successfully, 0 otherwise.
 */
int generate(BOARD *board, BOARD *solution_board, int x, int y, int numOfEmptyCells, int nXm, int nXm_square) {
	int times, all_empty_cells[numOfEmptyCells], copy_all[numOfEmptyCells], selected_empty_cells[x], digits[nXm];
	update_changes_in_board(solution_board, board, FALSE);
	fill_array_with_empty_cells(solution_board, all_empty_cells, nXm);
	srand(time(0));
	for (times=0; times<1000; times++){
		if (fill_x_cells_and_solve(solution_board,all_empty_cells,copy_all,selected_empty_cells,digits,x,numOfEmptyCells,nXm,nXm_square)) {
			empty_all_but_y_cells(solution_board, y, nXm, nXm_square);
			return TRUE;
		}
	}
	printf("Error: Puzzle generation failed with this board.\n");
	return FALSE;
}

/*
 *  this function executes the 'num_solutions' command. It calls the 'exhaustive_backtracking' method and prints a description of its result.
 *  @param board - a pointer to the current puzzle.
 */
void num_solutions(BOARD *board) {
	printf("This board has %d solutions.\n",exhaustive_backtracking(board));
}

/*
 *  this function executes the 'solve' and 'edit' commands: It loads a new puzzle, updates the 'mode', 'nXm' and 'command_list' parameters and deletes every non-saved changes in the previous puzzle (if there is such puzzle).
 *  @param path				- a string that is a path of a file (not necessary valid).
 *  @param board			- a pointer to the current puzzle.
 *  @param marking_board	- a pointer to a table that contains values which represent each of the puzzle cells' status: normal, fixed or erroneous.
 *  @param mode				- a pointer to a number that represents one of the 3 playable modes.
 *  @param command_name		- a number that represents the 'solve' command or the 'edit' command.
 *  @param nXm				- a pointer to a number that represents the number of cells in each row, column and block.
 *  @param command_list		- a pointer to a list of the user's moves.
 *
 *  a short summary of each command's implementation:
 *  'solve'				- calls to the 'load_board' method. If it returns TRUE, the previous puzzle is deleted and the new puzzle is loaded in SOLVE mode.
 *  'edit' (path != "")	- calls to the 'load_board' method. If it returns TRUE, the previous puzzle is deleted and the new puzzle is loaded in EDIT mode.
 *  'edit' (path == "")	- the previous puzzle is deleted. A new "standard Sudoku" puzzle (3X3) is loaded in EDIT mode.
 *
 *  @return 1 if a new puzzle was loaded successfully ('load_board' returned TRUE or path == ""), 0 otherwise ('load_board' returned FALSE).
 */
int start_puzzle(char *path,BOARD **board,BOARD **marking_board,int *mode,int command_name,int *nXm,list **command_list){
	BOARD *copy_game = NULL, *copy_marking = NULL;
	if (strlen(path) > 0) { /* command has a parameter */
		if (!load_board(path, &copy_game, &copy_marking, command_name)) {
			delete_board(copy_game);
			delete_board(copy_marking);
			return FALSE;
		}
	}
	delete_list(*command_list);
	delete_board(*board);
	delete_board(*marking_board);
	if (strlen(path) == 0) { /* command is 'edit', with no parameters */
		init_boards(board, marking_board, 3, 3);
	}
	else { /* command has a parameter, copy_game and copy_marking now contain the new puzzle*/
		*board = copy_board(copy_game);
		*marking_board = copy_board(copy_marking);
	}
	*command_list = init_list(*board);
	*mode = command_name == Edit ? EDIT : SOLVE; /*change mode to Edit or Solve, if the command is 'edit' or 'solve', respectively.*/
	*nXm = (*board)->N*(*board)->M;
	delete_board(copy_game);
	delete_board(copy_marking);
	return TRUE;
}

/*
 *  this function performs a move that requires solving the current puzzle with LP or with ILP, depends on the given command.
 *  @param command			- a number that represents one of the 6 legal command names that can be used for this function.
 *  @param board			- a pointer to the current puzzle.
 *  @param args				- an array of integers which are used as parameters for some of the commands.
 *  @param threshold		- a float that represents the threshold used in the 'gurobi' method.
 *  @param numOfEmptyCells	- a number that represents the current number of empty cells in the puzzle.
 *  @param nXm				- a number that represents the number of cells in each row, column and block.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 *
 *  first, the function creates a copy of the current board and allocates memory for every variable needed for solving the puzzle.
 *  Then, it calls the 'gurobi' and 'put_sol_in_board' methods to get a solution with LP (in 'guess' or 'guess_hint') or with ILP (otherwise).
 *  If both methods return TRUE, then the puzzle is solvable and the copy board contains its solution.
 *  A summary of each command's following implementation:
 *  'guess'			- this command has already been executed at this point, since the 'put_sol_in_board' method has determined which cells to fill (according to the threshold).
 *  				  Therefore, the function simply updates changes that occurred in the puzzle.
 *  'hint'			- if the puzzle is solvable, calls to the 'hint' method. Otherwise, the execution fails.
 *  'guess_hint'	- if the puzzle is solvable, calls to the 'guess_hint' method. Otherwise, the execution fails.
 *  'validate'		- calls to the 'validate' method.
 *  'save' 			- if the puzzle is solvable, does nothing. Otherwise, the execution fails.
 *  'generate'		- if the puzzle is solvable, calls to the 'generate' method. If it returns TRUE, updates changes that occurred in the puzzle.
 *  				  Otherwise (if the puzzle is unsolvable or 'generate' doesn't return TRUE), the execution fails.
 *
 *  @return 1 if the command was executed successfully, 0 if failed, and -1 if the program needs to be terminated.
 */
int execute_solution_based_command(int command, BOARD *board, int *args, float threshold, int numOfEmptyCells, int nXm, int *isUpdatedBoard) {
	BOARD *solution_board = NULL;
	int isSolvable, isExecuted, x, y, nXm_square, *map = NULL, num_of_vars, gurobi_result;
	double *sol = NULL;
	solution_board = copy_board(board);
	x = args[0], y = args[1], isExecuted = TRUE, nXm_square =nXm*nXm;
	map= (int*)calloc(nXm*nXm_square, sizeof(int));
	if(map == NULL){
		print_system_error(1,"error in allocating memory for map");
	}
	num_of_vars = map_maker(board, map, nXm, nXm_square);
	sol = (double*)calloc(num_of_vars, sizeof(double));
	if(sol == NULL){
		print_system_error(1,"error in allocating memory for scores");
	}
	gurobi_result = gurobi(board, num_of_vars, map, command != Guess_hint && command != Guess, sol);
	if (gurobi_result == TERMINATE) {
		print_system_error(2,NULL);
		isExecuted = TERMINATE;
	}
	else {
		isSolvable = gurobi_result == TRUE && put_sol_in_board(solution_board,map,sol,nXm,nXm_square,threshold) == TRUE;
		switch (command) {
		case Guess: update_count(update_changes_in_board(board, solution_board, FALSE), isUpdatedBoard); break;
		case Validate: validate(isSolvable); break;
		default:
			if (!validate_move(isSolvable,2,0,0,command)) {
				isExecuted = FALSE;
				break;
			}
			/* else - solution_board contains a correct solution for board: */
			switch (command) {
			case Hint: hint(solution_board, x, y); break;
			case Guess_hint: guess_hint(map, sol, x, y, nXm, nXm_square); break;
			case Save: break;
			default: /* command is 'generate' */
				isExecuted = generate(board, solution_board, x, y, numOfEmptyCells, nXm, nXm_square);
				if (isExecuted == TRUE) {
					update_count(update_changes_in_board(board, solution_board, FALSE), isUpdatedBoard);
				}
			}

		}
	}
	delete_board(solution_board);
	free(sol);
	free(map);
	return isExecuted;
}

/*
 *  this function checks if a given cell is valid for a given command.
 *  @param command			- a number that represents one of the 3 legal command names that can be used for this function.
 *  @param board			- a pointer to the current puzzle.
 *  @param marking_board	- a pointer to a table that contains values which represent each of the puzzle cells' status: normal, fixed or erroneous.
 *  @param x				- the required cells's column.
 *  @param y				- the required cells's row.
 *
 *  a short summary of each command's implementation:
 *  'hint'			- if the cell is fixed (according to marking_board) or empty, then the cell is invalid.
 *  'guess_hint'	- if the cell is fixed (according to marking_board) or empty, then the cell is invalid.
 *  'set'			- if the cell is fixed (according to marking_board), then the cell is invalid.
 *
 *  @return 0 if the cell is invalid, 1 otherwise.
 */
int validate_cell(int command, BOARD *board, BOARD *marking_board, int x, int y) {
	if (!validate_move(get_element_from_board(marking_board,x,y) != FIXED, 3, x+1, y+1, 0)) {
		return FALSE;
	}
	if (command != Set && !validate_move(get_element_from_board(board,x,y) == 0, 4, x+1, y+1, 0)) {
		return FALSE;
	}
	return TRUE;
}

/*
 *  this function performs a move that requires certain validations on the current puzzle, according to a specific given command.
 *  @param command			- a number that represents one of the 9 legal command names that can be used for this function.
 *  @param board			- a pointer to the current puzzle.
 *  @param marking_board	- a pointer to a table that contains values which represent each of the puzzle cells' status: normal, fixed or erroneous.
 *  @param mode				- a number that represents one of the 3 playable modes.
 *  @param isValidBoard		- a pointer to a number that determines whether there are errors in the puzzle.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 *  @param args				- an array of integers which are used as parameters for some of the commands.
 *  @param path				- a string that is used as a parameter for some of the commands.
 *  @param threshold		- a float that is used as a parameter for one of the commands.
 *  @param numOfEmptyCells	- a number that represents the current number of empty cells in the puzzle.
 *  @param nXm				- a number that represents the number of cells in each row, column and block.
 *
 *  a short summary of each command's implementation:
 *  'guess'			- if board is valid, calls to the 'execute_solution_based_command' method and returns its result.
 *  'hint'			- if board is valid, calls to the 'validate_cell' method. If it returns TRUE, calls to 'execute_solution_based_command' method and returns its result.
 *  'guess_hint'	- if board is valid, calls to the 'validate_cell' method. If it returns TRUE, calls to 'execute_solution_based_command' method and returns its result.
 *  'autofill'		- if board is valid, calls to the 'autofill' method.
 *  'set'			- calls to the 'validate_cell' method. If it returns TRUE, calls to the 'set' method.
 *  'validate'		- if board is valid, calls to the 'execute_solution_based_command' method and returns its result.
 *  'num_solution'	- if board is valid, calls to the 'num_solution' method.
 *  'save' (EDIT mode)	- if board is valid, calls to the 'execute_solution_based_command' method. If it returns TRUE, calls to the 'save_board' method and returns its result.
 *  'save' (SOLVE mode)	- calls to the 'save_board' method and returns its result.
 *  'generate'		- if board is valid, calls to the 'execute_solution_based_command' method and returns its result.
 *
 *  @return 1 if the command was executed successfully, 0 if failed, and -1 if the program needs to be terminated.
 */
int execute_if_valid_board(int command, BOARD *board, BOARD *marking_board, int mode, int* isValidBoard, int* isUpdatedBoard, \
		int *args, char *path, float threshold, int numOfEmptyCells, int nXm) {
	int result = TRUE;
	if (command != Save || mode == EDIT) {
		if (command != Set && (!validate_move(is_valid_board(board, marking_board, isValidBoard, isUpdatedBoard),1,0,0,command))) {
			return FALSE;
		}
		if (command == Set || command == Hint || command == Guess_hint) {
			if (!validate_cell(command, board, marking_board, args[0]-1, args[1]-1)) {
				return FALSE;
			}
		}
		if (command == Save || command == Validate || command == Generate || command == Hint || command == Guess_hint || command == Guess) {
			result = execute_solution_based_command(command,board,args,threshold,numOfEmptyCells,nXm,isUpdatedBoard);
			if (result != TRUE) {
				return result;
			}
		}
	}
	switch (command) {
	case Set: set(board, args[0]-1, args[1]-1, args[2], isValidBoard, isUpdatedBoard); break;
	case Autofill: autofill(board, isUpdatedBoard, nXm); break;
	case Num_solutions: num_solutions(board); break;
	case Save: return save_board(path, board, marking_board, mode);
	default: break;
	}
	return result;
}

/*
 *  this function performs a move on the current puzzle, according to a specific given command.
 *  @param command			- a number that represents one of the 17 legal command names.
 *  @param board			- a pointer to the current puzzle.
 *  @param marking_board	- a pointer to a table that contains values which represent each of the puzzle cells' status: normal, fixed or erroneous.
 *  @param command_list		- a pointer to a list of the user's moves.
 *  @param markErrors		- a pointer to a number that determines whether errors in the puzzle are displayed.
 *  @param mode				- a pointer to a number that represents one of the 3 playable modes.
 *  @param isValidBoard		- a pointer to a number that determines whether there are errors in the puzzle.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 *  @param nXm				- a pointer to a number that represents the number of cells in each row, column and block.
 *  @param numOfEmptyCells	- a pointer to a number that represents the current number of empty cells in the puzzle.
 *  @param args				- an array of integers which are used as parameters for some of the commands.
 *  @param path				- a string that is used as a parameter for some of the commands.
 *  @param threshold		- a float that is used as a parameter for one of the commands.
 *
 *  a short summary of each command's implementation:
 *  'mark_errors'	- the value of markErrors is changed to args[0].
 *  'guess'			- calls to the 'execute_if_valid_board' method. If it returns TRUE, the move is added to command_list and the board is printed.
 *  'hint'			- calls to the 'execute_if_valid_board' method and returns its result.
 *  'guess_hint'	- calls to the 'execute_if_valid_board' method and returns its result.
 *  'autofill'		- calls to the 'execute_if_valid_board' method. If it returns TRUE, the move is added to command_list and the board is printed.
 *  'print_board'	- the board is printed.
 *  'set'			- calls to the 'execute_if_valid_board' method. If it returns TRUE, the move is added to command_list and the board is printed.
 *  'validate'		- calls to the 'execute_if_valid_board' method and returns its result.
 *  'undo'			- calls to the 'undo_or_redo' method. If it returns TRUE, the board is printed.
 *  'redo'			- calls to the 'undo_or_redo' method. If it returns TRUE, the board is printed.
 *  'num_solution'	- calls to the 'execute_if_valid_board' method and returns its result.
 *  'reset'			- calls to the 'reset' method. The board is printed.
 *  'save'			- calls to the 'execute_if_valid_board' method and returns its result.
 *  'solve'			- calls to the 'start_puzzle' method. If it returns TRUE, the board is printed.
 *  'edit'			- calls to the 'start_puzzle' method. If it returns TRUE, the board is printed.
 *  'exit'			- calls to the 'exit_game' method and returns its result.
 *  'generate'		- calls to the 'execute_if_valid_board' method. If it returns TRUE, the move is added to command_list and the board is printed.
 *
 *  if the board is printed, then the 'update_num_of_empty_cells' method is called.
 *
 *  @return 1 if the command was executed successfully, 0 if failed, and -1 if the program needs to be terminated.
 */
int execute_command(int command, BOARD **board, BOARD **marking_board, list **command_list, int *markErrors, int* mode, \
		int* isValidBoard, int* isUpdatedBoard, int* nXm, int* numOfEmptyCells, int *args, char *path, float threshold) {
	int result = TRUE;
	if (command == Undo || command == Redo) {
		if (!undo_or_redo(*command_list, *board, command, isUpdatedBoard)) {
			return FALSE;
		}
	}
	else if (command == Edit || command == Solve){
		if (!start_puzzle(path, board, marking_board, mode, command, nXm, command_list)){
			return FALSE;
		}
	}
	else {
		switch (command) {
		case Mark_errors: *markErrors = args[0]; return TRUE;
		case Print_board: break;
		case Reset: reset(*command_list, *board, isUpdatedBoard); break;
		case Exit: return exit_game();
		default:
			result = execute_if_valid_board(command,*board,*marking_board,*mode,isValidBoard,isUpdatedBoard,args,path,threshold,*numOfEmptyCells,*nXm);
			if (result != TRUE) {
				return result;
			}
			if (command == Set || command == Autofill || command == Guess || command == Generate) {
				add_command(*command_list, *board, command);
				break;
			}
			return TRUE; /* else (command ends without printing the board) */
		}
	}
	print_board(*board, *marking_board, *markErrors, *mode, isValidBoard, isUpdatedBoard);
	update_num_of_empty_cells(*board, mode, *isValidBoard, numOfEmptyCells);
	printf("\n");
	return result;
}
