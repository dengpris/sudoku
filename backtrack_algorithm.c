// Sudoku backtracking algorithm for 9x9 grid
#include "backtrack_algorithm.h"

int refresh = 0;

/*************** Function Definitions **************/
// Initialize empty 9x9 matrix
void create_grid(){
	for (int row = 0; row < SIZE; row++){
		for (int col = 0; col < SIZE; col++){
			start[row][col] = UNASSIGNED;
		}
	}
}

// Check if sudoku is fully solved
bool check_full(int grid[SIZE][SIZE]){
	for (int row = 0; row < SIZE; row++){
		for (int col = 0; col < SIZE; col++){
			if (grid[row][col] == 0){
				return false;
			}
		}
	}
	return true;
}


// Shuffle the contents of an array
void shuffle(int arr[SIZE]){
	// srand(clock()); // CPUlator does not support clock()
	srand(time(NULL) + refresh);
	int n = SIZE;
	for (int i = n-1; i > 0; i--){
		// Random integer from 0 to i
		int j = rand() % (i+1);
		swap(&arr[i], &arr[j]);
	}
	refresh++;
}

// Swap two integer values
void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}


// Backtracking algorithm
// Used to test if starting numbers can generate unique solution
void sudoku_solver(int grid[SIZE][SIZE]){
	int row, col;
	
	for (int i = 0; i < GRID; i++){
		row = i / SIZE;
		col = i % SIZE;
		// If position is unassigned, assign it to a number between 1-9
		if (grid[row][col] == UNASSIGNED){
			for (int number = 1; number <= SIZE; number++){
				if (is_legal(grid, row, col, number)){
					grid[row][col] = number;
					if (check_full(grid)){
						soln_count++;
						break;
					} else {
					// Recursively call this function
						sudoku_solver(grid);
					}
				}
			}
			// All numbers in this position is invalid; grid must be redrawn
			break;
		}
	}
	// Redraw this square
	grid[row][col] = UNASSIGNED;
}

// Checks if number to be inserted is legal (check row, column, subgrid)
bool is_legal(int grid[SIZE][SIZE], int row, int col, int number){
	// Check if number has already occured in its row
	for (int COL = 0; COL < SIZE; COL++){
		if (grid[row][COL] == number){
			return false;
		}
	}
	// Next check if number has occured in its column
	for (int ROW = 0; ROW < SIZE; ROW++){
		if (grid[ROW][col] == number){
			return false;
		}
	}
	// Finally, check if number has occured in its subgrid
	int subgrid_row_start = row - (row % 3);
	int subgrid_col_start = col - (col % 3);
	for (int ROW = subgrid_row_start; ROW < subgrid_row_start + 3; ROW++){
		for (int COL = subgrid_col_start; COL < subgrid_col_start + 3; COL++){
			if (grid[ROW][COL] == number){
				return false;
			}
		}
	}
	// All conditions passed
	return true;
}
// debugging
void print_grid(int grid[SIZE][SIZE]){
	for (int row=0; row<9; row++){
		for (int i=0; i<11; i++){
			if (i<3){
				int c = grid[row][i];
				if (c == UNASSIGNED) printf(" ");
				else printf("%d", c);
			}
			else if (i>3 && i<7){
			    int c = grid[row][i-1];
			    if (c == UNASSIGNED) printf(" ");
				else printf("%d", c);
			}
			else if (i>7){
			    int c = grid[row][i-2];
			    if (c == UNASSIGNED) printf(" ");
				else printf("%d", c);
			}
			else if (i == 3 || i==7) { printf("|");}
		}
		printf("\n");
		if (row == 2 || row == 5){
		printf("-----------\n");
		}
	}
}

// Create randomized grid of full solution
bool fill_grid(int grid[SIZE][SIZE]){
    int row, col;
	int numbers[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	for (int i = 0; i < GRID; i++){
		row = i / 9;
		col = i % 9;

		if (grid[row][col] == UNASSIGNED){
			shuffle(numbers);
			for (int j = 0; j < SIZE; j++){
				// If value can be used
				if (is_legal(grid, row, col, numbers[j])){
					grid[row][col] = numbers[j];
					if (check_full(grid)){
						return true;
					} else {
					// Recursively call this function
						if (fill_grid(grid)) return true;
					}
				}
			}
			// All numbers in this position is invalid; grid must be redrawn
			break;
		}
	}
	grid[row][col] = UNASSIGNED;
	return false;
}

// Checks if playing grid is same as solved grid
bool check_win(int grid[SIZE][SIZE]){
	for (int row = 0; row < SIZE; row++){
		for (int col = 0; col < SIZE; col++){
			if (grid[SIZE][SIZE] != solved[SIZE][SIZE]) return false;
		}
	}
	return true;
}