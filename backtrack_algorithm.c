// Sudoku backtracking algorithm for 9x9 grid

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define UNASSIGNED 0
#define SIZE 	   9
#define SUBSIZE    3
#define GRID 	   81

// How it works:
// We solve the sudoku by assigning one number at a time the empty cells
// Check for safety, if valid, assign the number.
// If invalid, repeat with another number.
// Recursively check to see if it leads to a solution.
// In this way, we will generate a full board with a unique solution
// Then, we will remove numbers one at a time, while calling the algorithm again.
// This ensures that the player can come to an unique solution using a partial board
// We then use this to determine win state

/**************** Global Variables ****************/
int start[SIZE][SIZE];  // Erased 
int solved[SIZE][SIZE]; // Fully solved

/************** Function Declarations *************/
void create_grid();							// Creates empty 9x9 grid
bool check_full(int grid[SIZE][SIZE]);		// Return true if grid is full
int sudoku_solver(int grid[SIZE][SIZE]);	// Backtrack algorithm
bool is_legal(int grid[SIZE][SIZE], int row, int col, int number);
bool fill_grid(int grid[SIZE][SIZE]);		// Create random grid of numbers
void shuffle(int arr[SIZE]);				// Shuffle array of numbers
void swap(int *a, int *b);					// Swap two variables
void print_grid(int grid[SIZE][SIZE]);	

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
	srand(clock());
	int n = SIZE;
	for (int i = n-1; i > 0; i--){
		// Random integer from 0 to i
		int j = rand() % (i+1);
		swap(&arr[i], &arr[j]);
	}
}

// Swap two integer values
void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}


// Backtracking algorithm
// Used to test if starting numbers can generate unique solution
int sudoku_solver(int grid[SIZE][SIZE]){
	int row, col;
	int counter = 0;
	for (int i = 0; i < GRID; i++){
		row = i / SIZE;
		col = i % SIZE;
		// If position is unassigned, assign it to a number between 1-9
		if (grid[row][col] == UNASSIGNED){
			for (int number = 1; number <= SIZE; number++){
				if (is_legal(grid, row, col, number)){
					grid[row][col] = number;
					if (check_full(grid)){
						counter++;
						break;
					} else {
					// Recursively call this function
						return sudoku_solver(grid);
					}
				}
			}
			// All numbers in this position is invalid; grid must be redrawn
			break;
		}
	}
	// Redraw this square
	grid[row][col] = UNASSIGNED;
	return counter;
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
		if (row == 2 || row ==5){
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

// debugging
int main(){
    create_grid();
    fill_grid(start);
    
    // Create a copy of the completed array
    for (int row = 0; row < SIZE; row++){
    	for (int col = 0; col < SIZE; col++){
    		solved[row][col] = start[row][col];
    	}
    }

    int erase = 5 * SIZE; // Number of positions that will be blank
    int row, col;
    int temp;				// Keeps a backup of a position

    while (erase > 0){
    	// Choose random position to erase
    	row = rand() % SIZE;
    	col = rand() % SIZE;
    	// If this position is already empty, choose another
    	while (start[row][col] == UNASSIGNED){
    		row = rand() % SIZE;
    		col = rand() % SIZE;
    	}
    	temp = start[row][col];
    	start[row][col] = UNASSIGNED;

    	// Copy this grid
    	int grid_copy[SIZE][SIZE];
    	for (int row = 0; row < SIZE; row++){
    		for (int col = 0; col < SIZE; col++){
    			grid_copy[row][col] = start[row][col];
    		}
    	}
    	// Check for unique solution, if not, reroll a different number
    	if (sudoku_solver(grid_copy) != 1) start[row][col] = temp;
 		else erase--;
    }
    ///////////////////////////////////
    printf("Starting grid: \n");
    print_grid(start);
    printf("Fully solved grid: \n");
    print_grid(solved);

    return 0;
}