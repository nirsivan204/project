#include "Game.h"

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
 * execute the "exit" command: prints an exit message (and terminate the program).
 *
 */
void exit_game() {
	/* free resources...? */
	printf("Exiting...\n");
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


/*
 * sets the value of a cell and prints the current puzzle.
 *
 * @param board - the current puzzle.
 * @param fix_board - a puzzle that contains only values that never change throughout the game.
 * @param x - the required cells's column.
 * @param y - the required cells's row.
 * @param z - the required cells's new given value.
 */
void insert(BOARD *board,BOARD *fix_board, int x, int y, int z) {
	set_element_to_board(board,x,y,z);
	print_board(board, fix_board,1,1);
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
int set(BOARD *board,BOARD *fix_board, int x, int y, int z, int markErrors,int mode){
	int valid = 0;
	if(!is_in_range(x,0,board->N*board->M-1)){
		printf("x is out of bounds");
		return 0;
	}
	if(!is_in_range(y,0,board->N*board->M-1)){
		printf("y is out of bounds");
		return 0;
	}
	if(!is_in_range(z,0,board->N*board->M)){
		printf("z isn't legal");
		return 0;
	}
	int fix_val = get_element_from_board(fix_board,x,y);
	if(fix_val!=0 && mode == SOLVE){
		printf("Error: cell is fixed and you are in solve mode\n");
		return 0;
	}
	set_element_to_board(board,x,y,z);
	if(z==0){
		return 0;
	}
	valid = is_valid_insertion(board,x,y,z);
	set_element_to_board(board,x,y,z);
	if(valid == 1){
		print_board(board,fix_board,markErrors,mode);
		return puzzle_is_full_or_correct(board,0);
	}
	if(valid == 0){
		set_element_to_board(fix_board,x,y,2);
		print_board(board,fix_board,markErrors,mode);
		return -1;
	}
	printf("Error: value is invalid\n");
	return 0;
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
		int* mode, int args[], char path[], float* threshold) {
	int res;
	switch (command) {
	case Mark_errors:
		*markErrors = args[0];
		break;
	case Guess:
		break;
	case Hint:
		break;
	case Guess_hint:
		break;
	case Autofill:
		break;
	case Print_board:
		print_board(board,fix_board , *markErrors,*mode);
		break;
	case Set:
		res = set(board,fix_board,args[0]-1,args[1]-1,args[2],*markErrors,*mode);
		if(res == 1){
			if(puzzle_is_full_or_correct(fix_board,2)){
				printf("the puzzle was solved correctly");
			}else{
				printf("the solution is erroneous");
			}
		}
		break;
	case Validate:
		break;
	case Undo:
		break;
	case Redo:
		break;
	case Num_solutions:
		break;
	case Reset:
		break;
	case Exit:
		break;
	case Save:
		break;
	case Solve:
		break;
	case Edit:
		break;
	case Generate:
		break;
	}
	return 0;

}

