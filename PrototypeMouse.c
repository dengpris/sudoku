#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

//THE FOLLOWING ARRAY IS THE ARRAY OF IMAGES OF COLOURS 
//USE THE WEBSITE: http://www.rinkydinkelectronics.com/t_imageconverter565.php

//Keyboard variables
void HEX_PS2(char, char, char);
void colourScreen(short int colour);

//Highlighting on VGA
int SCREEN_WIDTH = 320; //X
int SCREEN_HEIGHT = 240; //Y
int mouseX=160;
int mouseY=120;
char XMovement = 0x00;
char YMovement = 0x00;

volatile int pixelBufferStart; // global variable

void clearScreen();
void plotPixel(int x, int y, short int lineColor);
void drawMouse(int x, int y, short int colour);

int main(void){
    //Drawing Variables
    volatile int * pixelCtrlPtr = (int *)0xFF203020;
    pixelBufferStart = *pixelCtrlPtr;
	
	clearScreen();
	
	//CALL BACK FUNCTIONS FOR THE KEY BOARD
	volatile int * PS2_ptr = (int *)0xff200100;
	int PS2_data, RVALID;
	
	/*  Background Information: Each key on the keyboard has a specific data input. PS2_data is that data input
	    This next part gets the individual bytes from that data and prints it onto the hex displays so that you can see the data input*/
	    
	char byte1 = 0, byte2 = 0, byte3 = 0;
	*(PS2_ptr) = 0xFF; // reset
	
	//Always checking for keyboard input
	while (1) {
	    
		PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
		RVALID = PS2_data & 0x8000; // extract the RVALID field (Checks if there was a change in keys? I think)
		//Intial mouse
		drawMouse(mouseX,mouseY, 0xffff);
		
		if (RVALID) {
		    
			/* shift the next data byte into the HEX display */
			byte1 = byte2;
			byte2 = byte3;
			byte3 = PS2_data & 0xFF;
			
			//Goes into hexdisplay function (this is where I tell the program what to do depending on which key is pressed)
			HEX_PS2(byte1, byte2, byte3);
			
			//I honestly dont know what this does. it might just be for mouse stuff. You can try deleting it
			if ((byte2 == (char)0xAA) && (byte3 == (char)0x00)){
				// mouse inserted; initialize sending of data
				*(PS2_ptr) = 0xF4;
			}
		}
	}
	
}

void drawMouse(int x, int y, short int colour){
	
	for(int i = x; i<4+x; i++){
		for(int j = y ; j<4+y; j++){
			plotPixel(i, j, colour);
		}
	}
}
void clearMouse(int x, int y){
	
	for(int i = x; i<4+x; i++){
		for(int j = y ; j<4+y; j++){
			plotPixel(i, j, 0x0000);
		}
	}
}

void HEX_PS2(char b1, char b2, char b3) {
    
    //Declaring hex pointers
	volatile int * HEX3_HEX0_ptr = (int *)0xff200020;
	volatile int * HEX5_HEX4_ptr = (int *)0xff200030;
	
	/* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
	* a single 7-seg display in the DE1-SoC Computer, for the hex digits 0 - F
	*/
	unsigned char seven_seg_decode_table[] = {
		0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
		0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	
	unsigned char hex_segs[] = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned int shift_buffer, nibble;
	unsigned char code;
	int i;
	
	
	//THE SHIFT BUFFER IS THE DATA INPUT. THIS IS THE IMPORTANT ONE. 
	shift_buffer = (b1 << 16) | (b2 << 8) | b3;
	printf("Testing b1: %x and b2: %x, and b3: %x \n", b1,b2,b3);
	
	unsigned int clicked = b1 & 0x0f;
	unsigned int currXMovement = b2;
	unsigned int currYMovement = b3;
	//Checks sign of x
	unsigned int Sign = b1 & 0x10;
	
	//If left mouse click
	if( b1 == 0x09){ //Pressed 1
		//And then I draw the number 1 at that position
		printf("BUTTON PRESSED!\n");
	}
	
	//Moving to the right (need to check for overflow later)
	//if((currXMovement > XMovement ) ){
	printf("Sign = %x, b1 = %x\n",Sign, b1);
	//if((Sign == 0x00) ){
	if((currXMovement > XMovement) && (b1 == 0x08 || b1 == 0x28 || b1 == 0x48|| b1 == 0x68|| b1 == 0x88|| b1 == 0xa8|| b1 == 0xc8|| b1 == 0xe8) ){
		//int change = currXMovement - XMovement;
		printf("\nRIGHT!\n");
		if((mouseX)>=0 && mouseX != 320){
			clearMouse(mouseX,mouseY);
			//printf("MOUSE: b2: %x, and b3: %x mouseX: %d change: %d\n", b1,b2,b3, mouseX, change);
		}
		if((mouseX+2)<320){
			mouseX = mouseX +2;		
			drawMouse(mouseX,mouseY, 0xffff);
		}
	}
	else if(((currXMovement > XMovement) && (b1 == 0x18 || b1 == 0x38|| b1 == 0x58|| b1 == 0x78 || b1 == 0x98|| b1 == 0xb8|| b1 == 0xd8|| b1 == 0xf8 ))){
		printf("LEFT! prv: %d, curr: %d\n", XMovement, currXMovement);
		if((mouseX)<320 && mouseX != 0){
			clearMouse(mouseX,mouseY);
		}
		if((mouseX-2)>=0){
			mouseX = mouseX-2;
			drawMouse(mouseX,mouseY, 0x96bf);
		}
	}
	
	else if((currYMovement > YMovement) && (b1 == 0x08 || b1 == 0x18 || b1 == 0x48|| b1 == 0x58|| b1 == 0x88|| b1 == 0x98|| b1 == 0xa8|| b1 == 0xb8) ){
		//int change = currXMovement - XMovement;
		printf("\nUP!\n");
		if((mouseY)>=0 && mouseY != 240){
			clearMouse(mouseX,mouseY);
		}
		if((mouseY+2)<240){
			mouseY = mouseY+2;
			drawMouse(mouseX,mouseY, 0x96bf);
		}
	}
	else if(((currYMovement > YMovement) && (b1 == 0x28 || b1 == 0x38|| b1 == 0x68|| b1 == 0x78 || b1 == 0xc8|| b1 == 0xd8|| b1 == 0xe8|| b1 == 0xf8 ))){
		if((mouseY)<9 && mouseY != 0){
			clearMouse(mouseX,mouseY);
		}
		if((mouseY-2)>=0){
			mouseY = mouseY-2;
			drawMouse(mouseX,mouseY, 0xC4BF);
		}
	}
	XMovement = currXMovement;
	YMovement = currYMovement;
	
	
	//This sets up the hex displays so that you can see the code from the shift_buffer
	for (i = 0; i < 6; ++i) {
		nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
		code = seven_seg_decode_table[nibble];
		hex_segs[i] = code;
		shift_buffer = shift_buffer >> 4;
	}
	
	/* drive the hex displays */
	*(HEX3_HEX0_ptr) = *(int *)(hex_segs);
	*(HEX5_HEX4_ptr) = *(int *)(hex_segs + 4);
}

void plotPixel(int x, int y, short int lineColor){
    *(short int *)(pixelBufferStart + (y << 10) + (x << 1)) = lineColor;
}

void colourScreen(short int colour)
{
	//short int black = 0;
	for(int i = 0; i < SCREEN_WIDTH; i++){
        for(int j = 0; j  < SCREEN_HEIGHT; j++){
            plotPixel(i, j, colour);
        }
    }
}
void clearScreen()
{
	short int black = 0;
	for(int i = 0; i < SCREEN_WIDTH; i++){
        for(int j = 0; j  < SCREEN_HEIGHT; j++){
            plotPixel(i, j, black);
        }
    }
}


