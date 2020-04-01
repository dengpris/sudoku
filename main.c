#include "backtrack_algorithm.h"

int start[SIZE][SIZE];  // Erased 
int solved[SIZE][SIZE]; // Fully solved


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