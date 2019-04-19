#include "Game.h"
#include <time.h>

void initialize_puzzle() {
	//init_boards(&board, &fix_board, 0, 0);
	//*command_list = *init_list(board);
	printf("Hello! Welcome to Sudoku!\n");
}

int validate_move(int result, int error_number, int arg1, int arg2, int arg3) {
	if (!result) {
		print_invalid_move_error(error_number,arg1,arg2,arg3);
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
void hint(BOARD *solution_board, int x, int y) {
	printf("According to current board's solution, value of cell <%d,%d> should be %d.\n",x,y,get_element_from_board(solution_board,x-1,y-1));
}

//void print_scores(int x, int y, int nXm, double *scores) {
//	int i;
//	printf("Legal values of cell <%d,%d>:\n", x+1, y+1);
//	for (i = 1; i<=nXm; i++) {
//		if (scores[i-1] > 0) {
//			printf("%d with score of: %f\n",i, scores[i-1]);
//		}
//	}
//}

void guess_hint(int *map, double *sol, int x, int y, int nXm, int nXm_square) {
	int i;
	double *scores = (double*)calloc(nXm, sizeof(double));
	get_hint(map,sol,x-1,y-1,nXm,nXm_square,scores);
	printf("Legal values of cell <%d,%d>:\n", x, y);
	for (i = 1; i<=nXm; i++) {
		if (scores[i-1] > 0) {
			printf("%d with score of: %f\n",i, scores[i-1]);
		}
	}
	free(scores);
}

void validate(int isSolvable) {
	printf(isSolvable ? "Validation passed: board is solvable.\n" : "Validation failed: board is unsolvable.\n");
}

void free_all(BOARD *board, BOARD *fix_board, list *command_list) {
	delete_list(command_list);
	delete_boards(board, fix_board);
}

/**
 * execute the "exit" command: free all resources, prints an exit message (and terminate the program).
 *
 */
int exit_game(BOARD *board, BOARD *fix_board, list *command_list) {
//	free_all(board, fix_board, command_list);
	printf("Goodbye! Thank you for playing Sudoku!\n");
	return TERMINATE;
}


//int find_value_in_board(BOARD *board,int x) {
//	assert(x==0||x==2);
//	int i,j;
//	for (i=0;i<board->M*board->N;i++){
//		for (j=0;j<board->M*board->N;j++) {
//			if (get_element_from_board(board, i, j) == x) {
//				return TRUE;
//			}
//		}
//	}
//	return FALSE;
//}


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

void update_count(int count, int* isUpdatedBoard) {
	if (count > 0) {
		*isUpdatedBoard = FALSE;
	}
}

void print_finish_update(int command, char character, int count, int *isUpdatedBoard) {
	char *command_name;
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

int update_changes_in_board(BOARD *copy_to_board, BOARD *copy_from_board, int withOutput) {
	int i, j, copy_to_val, copy_from_val, count;
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

int undo_or_redo(list *list, BOARD *board, int command, int* isUpdatedBoard) {
	int count;
	BOARD *list_board = NULL;
	node *node = NULL;
	node = command == Undo ? list->current_command : move_in_command_list(list, Redo);
	if (!validate_move(node != NULL, 5, command, 0, 0)) {
		return FALSE;
	}
	if (command == Undo) {
		node = move_in_command_list(list, Undo);
	}
	list_board = get_curr_board(list);
	print_start_update(command);
	count = update_changes_in_board(board,list_board,TRUE);
	print_finish_update(command,get_curr_command(node),count, isUpdatedBoard);
	return TRUE;
}

void reset(list *list, BOARD *board, int* isUpdatedBoard) {
	if (list->current_command != NULL) {
		list->current_command = NULL;
		update_count(update_changes_in_board(board, list->original_board, FALSE), isUpdatedBoard);
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
void set(BOARD *board, int x, int y, int z, int* isValidBoard, int* isUpdatedBoard){
	set_element_to_board(board,x,y,z);
	if (*isValidBoard == FALSE || !is_valid_insertion(board,x,y,z)) {
		*isUpdatedBoard = FALSE;
	}
}


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


int get_random_number(int range){ /*returning a random number between 0 to range-1*/
	return rand()%range;
}

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

void choose_random_empty_cells(int copy_all[], int selected_empty_cells[], int numOfEmptyCells, int x) {
	int i, rand_index;
	for (i=0;i<x;i++) {
		rand_index = get_random_number(numOfEmptyCells);
		selected_empty_cells[i] = copy_all[rand_index];
		copy_all[rand_index] = copy_all[--numOfEmptyCells];
	}
}

void emtpy_cells(BOARD* board, int selected_empty_cells[], int limit, int nXm) {
	int index, x, y, cell_num;
	for (index=0; index<limit; index++) {
		cell_num = selected_empty_cells[index];
		x = cell_num%nXm;
		y = cell_num/nXm;
		assert(y*nXm+x == cell_num);
		set_element_to_board(board, x, y, 0);
	}
}

int fill_cell_with_random_legal_value(BOARD* board, int cell_num, int digits[], int nXm) {
	int i, x, y, rand_index, rand_digit;
	x = cell_num%nXm;
	y = cell_num/nXm;
	for (i = 0; i < nXm; i++) {
		digits[i] = i+1;
	}
	//choose random digit, if is_valid_insertion: set and return TRUE. else- delete digit from array and repeat.
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
	map= (int*)calloc(nXm*nXm_square, sizeof(int));
	num_of_vars = map_maker(board, map, nXm, nXm_square);
	sol = (double*)calloc(num_of_vars, sizeof(double));

	if (gurobi(board,num_of_vars,map,TRUE,sol) != TRUE || !put_sol_in_board(board,map,sol,nXm,nXm_square,0)) {
		emtpy_cells(board, selected_empty_cells, x, nXm);
		gurobi_result = FALSE;
	}
	free(map);
	free(sol);
	return gurobi_result;
//	res = gurobi(board, num_of_vars, map, TRUE, sol);
//	if(res == -1){
//		printf("error in girobi fill x in cells");
//		return FALSE;
//	}
//	if (res==FALSE) {
//		emtpy_cells(board, selected_empty_cells, x);
//		return FALSE;
//	}
//	return TRUE;
}

void empty_all_but_y_cells(BOARD* board, int y, int nXm, int curr_num_of_cells) {
//choose (nXm - y) cells and empty them
	int i, rand_index, rand_cell_num, all_cells[curr_num_of_cells];
	for (i = 0; i < curr_num_of_cells; i++) {
		all_cells[i] = i;
	}
	while (curr_num_of_cells > y) {
		rand_index = get_random_number(curr_num_of_cells);
		rand_cell_num = all_cells[rand_index];
		set_element_to_board(board, rand_cell_num%nXm, rand_cell_num/nXm, 0);
		all_cells[rand_index] = all_cells[--curr_num_of_cells];
	}

}

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

void num_solutions(BOARD *board) {
	printf("This board has %d solutions.\n",exhaustive_backtracking(board));
}

int start_puzzle(char *path,BOARD **board,BOARD **fix_board,int *mode,int command_name,int *nXm,list **command_list){
	BOARD *copy_game = NULL, *copy_fix = NULL;
	if (strlen(path) > 0) { /* command has a parameter */
		if (!load_board(path, &copy_game, &copy_fix, command_name)) {
			delete_board(copy_game);
			delete_board(copy_fix);
			return FALSE;
		}
	}
	if(*command_list==NULL){
		printf("nnn");
	}
	delete_list(*command_list);
	delete_board(*board);
	delete_board(*fix_board);
//	free_all(board, fix_board, command_list);
	if (strlen(path) == 0) { /* command is 'edit', with no parameters */
		init_boards(board, fix_board, 3, 3);
	}
	else { /* command has a parameter, copy_game and copy_fix*/
		*board = copy_board(copy_game);
		*fix_board = copy_board(copy_fix);
	}
	*command_list = init_list(*board);
	//print_list(command_list,FALSE);
	*mode = command_name == Edit ? EDIT : SOLVE; /*change mode to Edit or Solve, if the command is 'edit' or 'solve', respectively.*/
	*nXm = (*board)->N*(*board)->M;
	delete_board(copy_game);
	delete_board(copy_fix);
	return TRUE;
}

//void execute_move(int command, BOARD *board, BOARD *solution_board, int args[], float threshold, int* isValidBoard, int* isUpdatedBoard) {
//	switch(command) {
//	case Set: set(board, args[0]-1, args[1]-1, args[2], isValidBoard, isUpdatedBoard); return;
//	case Autofill: autofill(board, isUpdatedBoard); return;
//	default: /* command is 'generate' or 'guess'*/
//		update_count(update_changes_in_board(board, solution_board, FALSE), isUpdatedBoard);
//	}
//}

//void execute_after_validation(int command, BOARD *board, BOARD *solution_board, int* isValidBoard, int* isUpdatedBoard, \
//		int args[], float threshold, list *command_list) {
//	switch (command) {
//	case Num_solutions: num_solutions(board); return;
//	case Hint: hint(solution_board, args[0], args[1]); break;
//	case Guess_hint: break;
//	default: /* command is either 'autofill', 'generate, 'guess' or 'set' */
//		execute_move(command, board, solution_board, args, threshold, isValidBoard, isUpdatedBoard);
//		add_command(command_list, board, command);
//		if (command == Set || command == Autofill) {
//			return;
//		}
//	}
////	delete_board(solution_board);
//}

int execute_solution_based_command(int command, BOARD *board, int *args, float threshold, int numOfEmptyCells, int nXm, int *isUpdatedBoard) {
	BOARD *solution_board = NULL;
	int isSolvable, isExecuted, x, y, nXm_square, *map, num_of_vars, gurobi_result;
	double *sol;
	solution_board = copy_board(board);
	x = args[0], y = args[1], isExecuted = TRUE, nXm_square =nXm*nXm;
	map= (int*)calloc(nXm*nXm_square, sizeof(int));
	num_of_vars = map_maker(board, map, nXm, nXm_square);
	sol = (double*)calloc(num_of_vars, sizeof(double));
	gurobi_result = gurobi(board, num_of_vars, map, command != Guess_hint && command != Guess, sol);
	if (gurobi_result == TERMINATE) {
		print_system_error(2);
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

//	int res,is_full;
//	double *scores;
//	int x = args[0]-1;
//	int y = args[1]-1;
//	int *map,result,num_of_vars;
//	double *sol;
//	int nXm_square =nXm*nXm;
//	*solution_board = *copy_board(board);
//	map= (int*)calloc(nXm*nXm_square, sizeof(int));
//	num_of_vars = map_maker(board, map, nXm, nXm_square);
//	sol = (double*)calloc(num_of_vars, sizeof(double));
//	res = gurobi(board, num_of_vars, map, command != Guess_hint && command != Guess, sol);
//	if (res != -1) {
//		if (command == Validate) {
//			print_validation_status(res);
////			delete_board(solution_board);
//			free(sol);
//			free(map);
//			return TRUE;
//		}
//		if(res==0){
//			printf("not feasible");
//			return FALSE;
//		}
//		is_full = put_sol_in_board(solution_board,map,sol,nXm,nXm_square,threshold);
//		if (command == Guess) {
//				free(sol);
//				free(map);
//				return TRUE;
//		}
//		if (!validate_move(is_full,2,0,0,command)) {
//				free(sol);
//				free(map);
//				return FALSE;
//		}
//		if (command == Guess_hint) {
//			scores = (double*)calloc(nXm, sizeof(double));
//			get_hint(map,sol,x,y,nXm,nXm_square,scores);
//			print_scores(x, y, nXm, scores);
//			free(scores);
//			free(sol);
//			free(map);
//		}
//		if (command == Generate && !generate(board, solution_board, args[0], args[1], numOfEmptyCells, nXm)) {
//					free(sol);
//					free(map);
//					return FALSE;
//		}
//		return TRUE;
//	}else{
//
//		printf("error in girobi");
//		return FALSE;
//	}
}

int validate_cell(int command, BOARD *board, BOARD *fix_board, int x, int y) {
	if (!validate_move(get_element_from_board(fix_board,x,y) != FIXED, 3, x+1, y+1, 0)) {
		return FALSE;
	}
	if (command != Set && !validate_move(get_element_from_board(board,x,y) == 0, 4, x+1, y+1, 0)) {
		return FALSE;
	}
	return TRUE;
}

int execute_if_valid_board(int command, BOARD *board, BOARD *fix_board, int mode, int* isValidBoard, int* isUpdatedBoard, \
		int *args, char *path, float threshold, int numOfEmptyCells, int nXm) {
	int result = TRUE;
	if (command != Save || mode == EDIT) {
		if (command != Set && (!validate_move(is_valid_board(board, fix_board, isValidBoard, isUpdatedBoard),1,0,0,command))) {
			return FALSE;
		}
		if (command == Set || command == Hint || command == Guess_hint) {
			if (!validate_cell(command, board, fix_board, args[0]-1, args[1]-1)) {
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
	case Save: return save_board(path, board, fix_board, mode);
	default: break;
	}
	return result;
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
int execute_command(int command, BOARD **board, BOARD **fix_board, list **command_list, int *markErrors, int* mode, \
		int* isValidBoard, int* isUpdatedBoard, int* nXm, int* numOfEmptyCells, int *args, char *path, float threshold) {
	int result = TRUE;
	if (command == Undo || command == Redo) {
		if (!undo_or_redo(*command_list, *board, command, isUpdatedBoard)) {
			return FALSE;
		}
	}
	else if (command == Edit || command == Solve){
		if (!start_puzzle(path, board, fix_board, mode, command, nXm, command_list)){
			return FALSE;
		}
	}
	else {
		switch (command) {
		case Mark_errors: *markErrors = args[0]; return TRUE;
		case Print_board: break;
		case Reset: reset(*command_list, *board, isUpdatedBoard); break;
		case Exit: return exit_game(*board, *fix_board, *command_list);
		default:
			result = execute_if_valid_board(command,*board,*fix_board,*mode,isValidBoard,isUpdatedBoard,args,path,threshold,*numOfEmptyCells,*nXm);
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
	print_board(*board, *fix_board, *markErrors, *mode, isValidBoard, isUpdatedBoard);
	update_num_of_empty_cells(*board, mode, *isValidBoard, numOfEmptyCells);
	printf("\n");
	return result;

//	int moveExecuted = FALSE;
//	switch (command) {
//	case Mark_errors:
//		*markErrors = args[0];
//		return TRUE;
//	case Hint:
//		//
//		return TRUE;
//	case Guess_hint:
//		//
//		return TRUE;
//	case Print_board:
//		break;
//	case Validate:
//		return validate(board, fix_board, command, isValidBoard, isUpdatedBoard);
//	case Num_solutions:
//		return num_solutions(board, fix_board, isValidBoard, isUpdatedBoard);
//	case Reset:
//		reset(command_list, board, isUpdatedBoard);
//		break;
//	case Save:
//		return save(path, board, fix_board, isValidBoard, isUpdatedBoard, *mode);
//	case Exit:
//		return exit_game(board, fix_board, command_list);
//	default:
//		if (command == Undo || command == Redo) {
//			if (!undo_or_redo(command_list, board, command, isUpdatedBoard)) {
//				return FALSE;
//			}
//		}
//		else if (command == Edit || command == Solve){
//			if (!start_puzzle(path, board, fix_board, mode, command, nXm, command_list)){
//				return FALSE;
//			}
//		}
//		else { /* command is either 'autofill', 'generate, 'guess' or 'set' */
//			if (!execute_move(command, board, fix_board, args, threshold, isValidBoard, isUpdatedBoard)) {
//				return FALSE;
//			}
//			moveExecuted = TRUE;
//		}
//	}
//	print_board(board, fix_board, *markErrors, *mode, isValidBoard, isUpdatedBoard);
//	if (moveExecuted) {
//		add_command(command_list, board, command);
//	}
//	update_num_of_empty_cells(board, mode, *isValidBoard, numOfEmptyCells);
//	printf("\n");
////	print_list(command_list,1);
//	return TRUE;

}

