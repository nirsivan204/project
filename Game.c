#include "Game.h"

int validate_move(int result, int arg1, int arg2, int arg3) {
	if (!result) {
		print_invalid_move_error(arg1,arg2,arg3);
	}
	return result;
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

void free_all(BOARD *board, BOARD *fix_board, BOARD *solved_board, list *command_list) {
	delete_list(command_list);
	delete_board(board);
	delete_board(fix_board);
	delete_board(solved_board);
}

/**
 * execute the "exit" command: free all resources, prints an exit message (and terminate the program).
 *
 */
int exit_game(BOARD *board, BOARD *fix_board, BOARD *solved_board, list *command_list) {
//	free_all(board, fix_board, solved_board, command_list);
	printf("Goodbye! Thank you for playing Sudoku!\n");
	return TERMINATE;
}


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

void check_if_full(BOARD *board, int *mode, int isValidBoard) {
	if (*mode == SOLVE && !find_value_in_board(board,0)) { /* board is full */
		if (isValidBoard) {
			printf("Well done! The puzzle was solved correctly! Please start a new puzzle, or exit.\n");
			*mode = INIT;
		}
		else {
			printf("The solution is erroneous! Try to use 'undo' or 'set' to correct erroneous cells.\n");
		}
	}
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

void print_start_update(int command) {
	char *command_name;
	switch (command) {
	case Undo: command_name = "undo"; break;
	case Redo: command_name = "redo"; break;
	default: command_name = "autofill"; break;
	}
	printf("Board updates following '%s':\n", command_name);
}

void print_cell_update(int x, int y, int old, int new) {
	printf("- value of cell <%d,%d> has changed from %d to %d.\n", x, y, old, new);
}

void print_finish_update(int command, char character, int count, int *isUpdatedBoard) {
	char *command_name;
	if (count > 0) {
		*isUpdatedBoard = FALSE;
	}
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

int update_changes_in_board(BOARD *game_board, BOARD *list_board, int withOutput) {
	int i, j, game_val, list_val, count;
	for (i=0;i<game_board->M*game_board->N;i++){
		for (j=0;j<game_board->M*game_board->N;j++) {
			game_val = get_element_from_board(game_board, i, j);
			list_val = get_element_from_board(list_board, i, j);
			if (game_val != list_val) {
				count++;
				set_element_to_board(game_board, i, j, list_val);
				if (withOutput) {
					print_cell_update(i+1, j+1, game_val, list_val);
				}
			}
		}
	}
	return count;
}

int undo_or_redo(list *list, BOARD *board, int command, int* isUpdatedBoard) {
	BOARD *list_board;
	node *node;
	node = command == Undo ? list->current_command : move_in_command_list(list, Redo);
	if (!validate_move(node != NULL, 5, command, 0)) {
		return FALSE;
	}
	if (command == Undo) {
		node = move_in_command_list(list, Undo);
	}
	list_board = get_curr_board(list);
	print_start_update(command);
	print_finish_update(command,get_curr_command(node),update_changes_in_board(board,list_board,TRUE),isUpdatedBoard);
	return TRUE;
}

void reset(list *list, BOARD *board) {
	if (list->current_command != NULL) {
		list->current_command = NULL;
		update_changes_in_board(board, list->original_board, FALSE);
	}
}

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
int set(BOARD *board, BOARD *fix_board, int x, int y, int z, int* isValidBoard, int* isUpdatedBoard){
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
	print_start_update(Autofill);
	for (i=0;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++){
			if (get_element_from_board(board,i,j) == 0) {
				val = get_only_legal_value(board,i,j);
				if (val > 0) {
					set_element_to_board(board, i, j, -val);
					count++;
					print_cell_update(i+1, j+1, 0, val);
				}
			}
		}
	}
	print_finish_update(Autofill, 0, count, isUpdatedBoard);
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

int guess(BOARD *board, float threshold) {
	return TRUE;
}

int generate(BOARD *board, int x, int y) {
	return TRUE;
}

int execute_move(int command, BOARD *board, BOARD *fix_board, int args[], float threshold, int* isValidBoard, int* isUpdatedBoard) {
	switch(command) {
	case Set: return set(board, fix_board, args[0]-1, args[1]-1, args[2], isValidBoard, isUpdatedBoard);
	case Autofill: return autofill(board, fix_board, isValidBoard, isUpdatedBoard);
	case Guess: return guess(board, threshold);
	default: return generate(board, args[0], args[1]); /*command is 'generate'*/
	}
}

int num_solutions(BOARD *board, BOARD *fix_board, int* isValidBoard, int* isUpdatedBoard) {
	if (!validate_move(is_valid_board(board, fix_board, isValidBoard, isUpdatedBoard),1,0,0)) {
		return FALSE;
	}
	printf("This board has %d solutions.\n",exhaustive_backtracking(board));
	return TRUE;
}

int start_puzzle(char *path, BOARD *board, BOARD *fix_board, int *mode, int command_name, int *N, int *M, list *command_list){
	if (strlen(path) == 0) { /* command is 'edit', with no parameters */
		*board = *init_board(3,3);
		*fix_board = *init_board(3,3);
	}
	else if (!load_board(path, board, fix_board, command_name)) {
		return FALSE;
	}
	*mode = command_name == Edit ? EDIT : SOLVE; /*change mode to Edit or Solve, if the command is 'edit' or 'solve', respectively.*/
	*N = board->N;
	*M = board->M;
	if (command_list != NULL) {
//		delete_list(command_list);
		*command_list = *init_list(board);
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
		int* mode, int* isValidBoard, int* isUpdatedBoard, int* N, int *M, int args[], char path[], float threshold) {
	int moveExecuted = FALSE;
	switch (command) {
	case Mark_errors:
		*markErrors = args[0];
		return TRUE;
	case Hint:
		//
		return TRUE;
	case Guess_hint:
		//
		return TRUE;
	case Print_board:
		break;
	case Validate:
		//
		return TRUE;
	case Num_solutions:
		return num_solutions(board, fix_board, isValidBoard, isUpdatedBoard);
	case Reset:
		reset(command_list, board);
		break;
	case Save:
		return save(path, board, fix_board, isValidBoard, isUpdatedBoard, *mode);
	case Exit:
		return exit_game(board, fix_board, solved_board, command_list);
	default:
		if (command == Undo || command == Redo) {
			if (!undo_or_redo(command_list, board, command, isUpdatedBoard)) {
				return FALSE;
			}
		}
		else if (command == Edit || command == Solve){
			if (!start_puzzle(path, board, fix_board, mode, command, N, M, command_list)){
				return FALSE;
			}
		}
		else { /* command is either 'autofill', 'generate, 'guess' or 'set' */
			if (!execute_move(command, board, fix_board, args, threshold, isValidBoard, isUpdatedBoard)) {
				return FALSE;
			}
			moveExecuted = TRUE;
		}
	}
	print_board(board, fix_board, *markErrors, *mode, isValidBoard, isUpdatedBoard);
	if (moveExecuted) {
		add_command(command_list, board, command);
		check_if_full(board, mode, *isValidBoard);
	}
	printf("\n");
//	print_list(command_list,1);
	return TRUE;

}

