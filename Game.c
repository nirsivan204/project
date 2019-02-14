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
int puzzle_solved(BOARD *board) {
	int i;
	int j;
	for (i=0;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++) {
			if (get_element_from_board(board, i, j) == 0) {
				return 0;
			}
		}
	}
	printf("Puzzle solved successfully\n");
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
	print_board(board, fix_board);
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
int set(BOARD *board,BOARD *fix_board, int x, int y, int z){
	int fix_val = get_element_from_board(fix_board,x,y);
	if(fix_val!=0){
		printf("Error: cell is fixed\n");
		return 0;
	}
	if(z==0){
		insert(board,fix_board,x,y,z);
		return 0;
	}
	if(is_valid_insertion(board,x,y,z)==1){
		insert(board,fix_board,x,y,z);
		return puzzle_solved(board);
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
int execute_command(int *command, BOARD *board, BOARD *fix_board, BOARD *solved_board) {
	switch (command[0]) {
	case SET:
		return set(board, fix_board, --command[1], --command[2], command[3]);
	case HINT:
		hint(solved_board, --command[1], --command[2]);
		break;
	case VALIDATE:
		validate(board, solved_board);
		break;
	case RESTART:
		restart_game();
		break;
	case EXIT:
		exit_game();
		break;
	}
	return 0;

}

