

/* function prototypes */
void HEX_PS2(char, char, char);
void plotPixel(int x, int y, short int lineColor);
void clearScreen(short int colour);
volatile int pixelBufferStart; // global variable
int SCREEN_WIDTH = 320; //X
int SCREEN_HEIGHT = 240; //Y
/*******************************************************************************
* This program demonstrates use of the PS/2 port by displaying the last three
* bytes of data received from the PS/2 port on the HEX displays.
******************************************************************************/
int main(void) {
	/* Declare volatile pointers to I/O registers (volatile means that IO load
	and store instructions will be used to access these pointer locations,
	instead of regular memory loads and stores) */
	volatile int * pixelCtrlPtr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixelBufferStart = *pixelCtrlPtr;
	
	volatile int * PS2_ptr = (int *)0xff200100;
	
	int PS2_data, RVALID;
	char byte1 = 0, byte2 = 0, byte3 = 0;
	
	// PS/2 mouse needs to be reset (must be already plugged in)
	*(PS2_ptr) = 0xFF; // reset
	
	while (1) {
		PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
		RVALID = PS2_data & 0x8000; // extract the RVALID field
		if (RVALID) {
			/* shift the next data byte into the display */
			byte1 = byte2;
			byte2 = byte3;
			byte3 = PS2_data & 0xFF;
			HEX_PS2(byte1, byte2, byte3);
			if ((byte2 == (char)0xAA) && (byte3 == (char)0x00))
				// mouse inserted; initialize sending of data
				*(PS2_ptr) = 0xF4;
		}
	}
}

/****************************************************************************************
* Subroutine to show a string of HEX data on the HEX displays
****************************************************************************************/
void HEX_PS2(char b1, char b2, char b3) {
	volatile int * HEX3_HEX0_ptr = (int *)0xff200020;
	volatile int * HEX5_HEX4_ptr = (int *)0xff200030;
	volatile int * LED_ptr = (int *) 0xff200000;
	/* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
	* a single 7-seg display in the DE1-SoC Computer, for the hex digits 0 - F
	*/
	unsigned char seven_seg_decode_table[] = {
		0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
		0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	unsigned char letters_table[] = {
		0x00, 0x5e, 0x79, 0x71, 0x3e, 0x5b, 0x06, 0x1c,
		0x3b, 0x3f, 0x79, 0x77, 0x58, 0x1f, 0x1e, 0x0f};
	unsigned numbers_table[] = {
		0x00, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
		0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	
	unsigned char decode_table[] = {0, 0, 0, 0, 0, 0, 0, 0};
	
	unsigned char hex_segs[] = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned int shift_buffer, nibble;
	unsigned char code;
	int i, LED_bits,letters, numbers;
	LED_bits = 0x000fffff;
	
	numbers = 0;
	letters = 0;
	
	shift_buffer = (b1 << 16) | (b2 << 8) | b3;
	
	if(shift_buffer == 0x16f016){ //Pressed 1
		LED_bits = 0x00000001;
		shift_buffer = 0x000001; 
		numbers = 1;
	}else if(shift_buffer == 0x1ef01e){ //Pressed 2
		LED_bits = 0x00000002;
		shift_buffer = 0x000002;
		numbers = 1;
	}else if(shift_buffer == 0x26f026){ //Pressed 3
		LED_bits = 0x00000004;
		shift_buffer = 0x000003;
		numbers = 1;
	}else if(shift_buffer == 0x25f025){ //Pressed 4
		LED_bits = 0x00000008;
		shift_buffer = 0x000004;
		numbers = 1;
	}else if(shift_buffer == 0x2ef02e){ //Pressed 5
		LED_bits = 0x00000010;
		shift_buffer = 0x000005;
		numbers = 1;
	}else if(shift_buffer == 0x36f036){ //Pressed 6
		LED_bits = 0x00000020;
		shift_buffer = 0x000006;
		numbers = 1;
	}else if(shift_buffer == 0x3df03d){ //Pressed 7
		LED_bits = 0x00000040;
		shift_buffer = 0x000007;
		numbers = 1;
	}else if(shift_buffer == 0x3ef03e){ //Pressed 8
		LED_bits = 0x00000080;
		shift_buffer = 0x000008;
		numbers = 1;
	}else if(shift_buffer == 0x46f046){ //Pressed 9
		LED_bits = 0x00000100;
		shift_buffer = 0x000009;
		numbers = 1;
	}
	
	else if(shift_buffer == 0xe0f074){  //Go Right (light up LED4-0)
		LED_bits = 0x0000001f;
		shift_buffer = 0x0b645c;
		letters = 1;
		clearScreen(0x227B);
	}else if(shift_buffer == 0xe0f06b){ //Go Left (light up LED9-5)
		LED_bits = 0x000003e0;
		shift_buffer = 0x00723c;
		letters = 1;
		clearScreen(0xD13B);
	}else if(shift_buffer == 0xe0f075){ //Go Up (light up LED5-4)
		LED_bits = 0x00000030;
		shift_buffer = 0x0000da;
		letters = 1;
		clearScreen(0x3466);
	}else if(shift_buffer == 0xe0f072){ //Go Down (light up LED9 & LED0)
		LED_bits = 0x00000201;
		shift_buffer = 0x019ef8;
		letters =1;
		clearScreen(0x0000);
	}
	
	if(numbers){
		for (i = 0; i < 6; ++i) {
			nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
			code = numbers_table[nibble];
			hex_segs[i] = code;
			shift_buffer = shift_buffer >> 4;
		}
	}else if(letters){
		for (i = 0; i < 6; ++i) {
			nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
			code = letters_table[nibble];
			hex_segs[i] = code;
			shift_buffer = shift_buffer >> 4;
		}
	}else{
		for (i = 0; i < 6; ++i) {
			nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
			code = seven_seg_decode_table[nibble];
			hex_segs[i] = code;
			shift_buffer = shift_buffer >> 4;
		}
	}
	
	/* drive the hex displays */
	*(HEX3_HEX0_ptr) = *(int *)(hex_segs);
	*(HEX5_HEX4_ptr) = *(int *)(hex_segs + 4);
	*(LED_ptr) = LED_bits;
}

void clearScreen(short int colour)
{
	for(int i = 0; i < SCREEN_WIDTH; i++){
        for(int j = 0; j  < SCREEN_HEIGHT; j++){
            plotPixel(i, j, colour);
        }
    }
}

void plotPixel(int x, int y, short int lineColor){
    *(short int *)(pixelBufferStart + (y << 10) + (x << 1)) = lineColor;
}
