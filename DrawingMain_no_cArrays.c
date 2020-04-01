#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

//Include Background C array
//Include newNumbers C array
	
int SCREEN_WIDTH = 320; //X
int SCREEN_HEIGHT = 240; //Y
int X1 = 8;
int X2 = 33;
int X3 = 57;
int X4 = 82;
int X5 = 106;
int X6 = 130;
int X7 = 155;
int X8 = 179;
int X9 = 203;

volatile int pixelBufferStart; // global variable
void drawBackground();
void drawNewNumbers(int num, int xStart, int yStart);
void clearScreen();
void plotPixel(int x, int y, short int lineColor);



int main(void){
    volatile int * pixelCtrlPtr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixelBufferStart = *pixelCtrlPtr;
	clearScreen();
	drawBackground();
	drawNewNumbers(1,X1,X4);
	drawNewNumbers(2,X2,X4);
   	drawNewNumbers(3,X3,X4);
	drawNewNumbers(4,X4,X4);
	drawNewNumbers(5,X5,X4);
	drawNewNumbers(6,X6,X4);
	drawNewNumbers(7,X7,X4);
	drawNewNumbers(8,X8,X4);
	drawNewNumbers(9,X9,X4);
}

// code not shown for clearScreen() and drawLine() subroutines

void plotPixel(int x, int y, short int lineColor){
    *(short int *)(pixelBufferStart + (y << 10) + (x << 1)) = lineColor;
}

void clearScreen(){
	short int black = 0;
	for(int i = 0; i < SCREEN_WIDTH; i++){
        for(int j = 0; j  < SCREEN_HEIGHT; j++){
            plotPixel(i, j, black);
        }
    }
}

void drawNewNumbers(int num, int xStart, int yStart){
	int colourIdx = 0;
	for(int y = yStart; y < 20+yStart; y++){
        for(int x = xStart; x < 21+xStart; x++){
            plotPixel(x, y, newNumbers[num][colourIdx]);
			colourIdx++;
		}
    }
}

void drawBackground(){
	int colourIdx = 0;
	for(int y = 0; y < 240; y++){
        for(int x = 0; x < 320; x++){
            plotPixel(x, y, Background[colourIdx]);
			colourIdx++;
		}
    }
}