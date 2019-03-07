#include "Game.h"

int validate_move(int result, int arg1, int arg2, int arg3) {
	if (!result) {
		print_invalid_move_error(arg1,arg2,arg3);
		return FALSE;
	}
	return TRUE;
}

/**
 * execute the "hint" command: gets the value of a cell in the current solution-board, and prints it as a hint for the user.
 *
 * @param solved_board - the current solution for the puzzle.
 * @param x - the required cells's column.
 * @param x - the required cells's row.
 *
 */
void hint(BOARD *solved_board, int x, int y) {
	printf("Hint: set cell to %d\n", get_element_from_board(solved_board, x, y));
}

int is_solvable() {
	return TRUE;
}

/**
 * execute the "validate" command: solves a copy of the game-board with the deterministic Back-tracking algorithm.
 * If a solution was found, it replaces the current solution-board. Finally, the function prints a success/failure message.
 *
 * @param board - the current puzzle.
 * @param solved_board - the current solution for the puzzle.
 *
 */
void validate(BOARD *board, BOARD *solved_board) {
	int is_solvable;
	BOARD *temp_board = NULL;
	temp_board = copy_board(board);
	is_solvable = build_board(temp_board,1);
	if(is_solvable == 1){
		solved_board = copy_board(temp_board);
		printf("Validation passed: board is solvable\n");
	}
	else{
		printf("Validation failed: board is unsolvable\n");
	}
}

/**
 * execute the "exit" command: free all resources, prints an exit message (and terminate the program).
 *
 */
void exit_game(BOARD *board, BOARD *fix_board, BOARD *solved_board, list *command_list) {
//	delete_list(command_list);
//	delete_board(board);
//	delete_board(fix_board);
//	delete_board(solved_board);
	printf("Goodbye! Thank you for playing Sudoku!\n");
}

/**
 * execute the "restart" command (start a new puzzle).
 * for now does nothing, maybe in future cases we will need it.
 *
 */
void restart_game() {
}

/*
 * checks if the puzzle is solved completely, and if so then prints a message of successful solution.
 *
 * @param board - the current puzzle.
 *
 * @return
 * 1 - if every cell of 'board' is full.
 * 0 - otherwise.
 */

int find_value_in_board(BOARD *board,int x) {
	assert(x==0||x==2);
	int i,j;
	for (i=0;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++) {
			if (get_element_from_board(board, i, j) == x) {
				return TRUE;
			}
		}
	}
	return FALSE;
}
/*
int puzzle_is_full_or_correct(BOARD *board,int x) {
	assert(x==0||x==2);
	int i;
	int j;
	for (i=0;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++) {
			if (get_element_from_board(board, i, j) == x) {
				return 0;
			}
		}
	}
	if(x == 0){
		printf("Puzzle is Full\n");
	}else{
		printf("Puzzle is correct\n");
	}
	return 1;
}
*/


/*
 * sets the value of a cell and prints the current puzzle.
 *
 * @param board - the current puzzle.
 * @param fix_board - a puzzle that contains only values that never change throughout the game.
 * @param x - the required cells's column.
 * @param y - the required cells's row.
 * @param z - the required cells's new given value.
 */
/*
void insert(BOARD *board,BOARD *fix_board, int x, int y, int z) {
	set_element_to_board(board,x,y,z);
	print_board(board, fix_board,1,1);
}
*/

/*
 * execute the "set" command: if the given parameters represent a valid insertion (the required cell isn't "fixed" and is_valid_insertion
 * returns 1), then the function calls insert. Otherwise, it prints the relevant error-message.
 * If an empty cell was filled, the function checks if calls puzzle_solved and returns its return value.
 * .
 *
 * @param board - the current puzzle.
 * @param fix_board - a puzzle that contains only values that never change throughout the game.
 * @param x - the required cells's column.
 * @param y - the required cells's row.
 * @param z - the required cells's new given value.
 *
 * @return
 * 1 - if the current puzzle has been completed (by filling the last empty cell).
 * 0 - otherwise.
 */
int set(BOARD *board, BOARD *fix_board, int x, int y, int z, int markErrors, int* mode, int* isValidBoard, int* isUpdatedBoard){
	int valid = 0;
	if(get_element_from_board(fix_board,x,y) == FIXED){
		print_invalid_move_error(3,x+1,y+1);
		return FALSE;
	}
	valid = is_valid_insertion(board,x,y,z);
	set_element_to_board(board,x,y,z);
	if (*isValidBoard == FALSE || valid == FALSE) {
		*isUpdatedBoard = FALSE;
	}
	print_board(board,fix_board,markErrors,*mode,isValidBoard,isUpdatedBoard);
	if (*mode == SOLVE && z > 0 && !find_value_in_board(board,0)) { /* board is full */
		if (*isValidBoard == TRUE) {
			printf("The puzzle was solved correctly!\n");
			*mode = INIT;
		}
		else {
			printf("The solution is erroneous!\n");
		}
	}
	return TRUE;
}

int start_puzzle(char *path, BOARD *board, BOARD *fix_board, int *mode, int command_name){
	if (strlen(path) == 0) { /* command is 'edit', with no parameters */
		*board = *init_board(3,3);
		*fix_board = *init_board(3,3);
	}
	else if (!load_board(path, board, fix_board, command_name)) {
		return FALSE;
	}
	*mode = command_name == Edit ? EDIT : SOLVE; /*change mode to Edit or Solve, if the command is 'edit' or 'solve', respectively.*/
	return TRUE;
}

int get_only_legal_value(BOARD *board, int x, int y) {
	int value = 0, digit = 1;
	while (digit <= board->M*board->N){
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

int autofill(BOARD *board, BOARD *fix_board, int* isValidBoard, int* isUpdatedBoard) {
	int i, j, val, count = 0;
	if (!validate_move(is_valid_board(board, fix_board, isValidBoard, isUpdatedBoard),1,0,0)) {
		return FALSE;
	}
	printf("Auto-fill updates:\n");
	for (i=0;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++){
			if (get_element_from_board(board,i,j) == 0) {
				val = get_only_legal_value(board,i,j);
				if (val > 0) {
					set_element_to_board(board, i, j, -val);
					count++;
					printf("- value of cell <%d,%d> is now %d.\n", i+1, j+1, val);
				}
			}
		}
	}
	if (count > 0) {
		*isUpdatedBoard = FALSE;
	}
	printf("%d empty cells had a single legal value and were automatically filled.\n", count);
	for (i=0;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++){
			val = get_element_from_board(board,i,j);
			if (val < 0) {
				set_element_to_board(board, i, j, -val);
			}
		}
	}
	return TRUE;
}

int save(char *path, BOARD *board, BOARD *fix_board, int* isValidBoard, int* isUpdatedBoard, int mode) {
	if (mode == EDIT) {
		if (!validate_move(is_valid_board(board, fix_board, isValidBoard, isUpdatedBoard),1,0,0)) {
			return FALSE;
		}
		if (!validate_move(is_solvable(),2,0,0)) {
			return FALSE;
		}
	}
	return save_board(path, board, fix_board, mode);
}

/*
 * performs a move on the current puzzle, according to a specific given command.
 *
 * @param command - the array that represents a command:
 * the first element represents the command word and the rest of the elements represents the command parameters (if there are any).
 * @param board - the current puzzle.
 * @param fix_board - a puzzle that contains only values that never change throughout the game.
 * @param solved_board - the current solution for the puzzle.
 *
 * @return
 * 1 - if the function 'set' is called and returns 1 (if the current puzzle has been completed by filling the last empty cell).
 * 0 - otherwise.
 */
int execute_command(int command, BOARD *board, BOARD *fix_board, BOARD *solved_board, list *command_list, int *markErrors, \
		int* mode, int* isValidBoard, int* isUpdatedBoard, int* isNewBoard, int args[], char path[], float* threshold) {
	int printBoard = FALSE ,addToList = FALSE;
	*isNewBoard = FALSE;
	switch (command) {
	case Mark_errors:
		*markErrors = args[0];
		addToList = TRUE;
		break;
	case Guess:
		printBoard = TRUE;
		addToList = TRUE;
		break;
	case Hint:
		break;
	case Guess_hint:
		break;
	case Autofill:
		if (!autofill(board, fix_board, isValidBoard, isUpdatedBoard)) {
			return FALSE;
		}
		printBoard = TRUE;
		addToList = TRUE;
		break;
	case Print_board:
		printBoard = TRUE;
		break;
	case Set:
		if (!set(board, fix_board, args[0]-1, args[1]-1, args[2], *markErrors, mode, isValidBoard, isUpdatedBoard)) {
			return FALSE;
		}
		addToList = TRUE;
		break;
	case Validate:
		break;
	case Undo:
		printBoard = TRUE;
		//if file has been changed: *isNewBoard = TRUE; //
		break;
	case Redo:
		printBoard = TRUE;
		//if file has been changed: *isNewBoard = TRUE; //
		break;
	case Num_solutions:
		break;
	case Reset:
		printBoard = TRUE;
		break;
	case Save:
		return save(path, board, fix_board, isValidBoard, isUpdatedBoard, *mode);
	case Exit:
		exit_game(board, fix_board, solved_board, command_list);
		return TERMINATE;
	case Generate:
		printBoard = TRUE;
		addToList = TRUE;
		break;
	default: /* command is 'edit' or 'solve' */
		if (!start_puzzle(path, board, fix_board, mode, command)) {
			return FALSE;
		}
		printBoard = TRUE;
		*isNewBoard = TRUE;
	}
	if (printBoard) {
		print_board(board, fix_board, *markErrors, *mode, isValidBoard, isUpdatedBoard);
	}
	if (addToList) {

	}
	return TRUE;

}

