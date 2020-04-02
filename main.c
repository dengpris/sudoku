#include "backtrack_algorithm.h"

int start[SIZE][SIZE];  // Erased 
int solved[SIZE][SIZE]; // Fully solved
bool given[SIZE][SIZE]; // Determines if the numbers were given
int soln_count;

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
        soln_count = 0;
        sudoku_solver(grid_copy);
    	if (soln_count != 1) start[row][col] = temp;
 		else erase--;
    }

    // Copy into given grid
    for (int row = 0; row < SIZE; row++){
        for (int col = 0; col < SIZE; col++){
            if (start[row][col] == UNASSIGNED) given[row][col] = false;
            else given[row][col] = true;
        }
    }

    ///////////////////////////////////
    printf("Starting grid: \n");
    print_grid(start);
    printf("Fully solved grid: \n");
    print_grid(solved);

    // Start to play
    clock_t start_t = clock();
    bool win = false;

    int x, y;
    int num;

    while (!win){
        printf("Input a position (e.g.: 3 3): ");
        scanf("%d %d", &x, &y);

        printf("What number would you like to input? ");
        scanf("%d", &num);

        printf("You've inputed the number %d at position %d, %d\n", num, x, y);
        if (!given[x][y]) start[x][y] = num;
        else printf("You cannot change the value of a starting number!\n");

        win = check_win(start);
        print_grid(start);
    }

    return 0;
}