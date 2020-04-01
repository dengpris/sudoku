
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

extern int start[SIZE][SIZE];  // Erased 
extern int solved[SIZE][SIZE]; // Fully solved

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
