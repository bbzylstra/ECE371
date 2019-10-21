#include <stdio.h>
#include <stdlib.h>

#include "aes.c"
#include "address_map_arm.h"
#include "capture_image.h"
#define KEY_BASE              0xFF200050
#define VIDEO_IN_BASE         0xFF203060
#define FPGA_ONCHIP_BASE      0xC8000000

#define SWITCH_9_MSK		  0x00000200
#define SWITCH_8_MSK		  0x00000100
#define SWITCH_7_MSK		  0x00000080
#define KEY_3_MSK			  0x00000008
#define KEY_2_MSK			  0x00000004
#define KEY_1_MSK			  0x00000002

/* This program demonstrates the use of the D5M camera with the DE1-SoC Board
 * It performs the following: 
 * 	1. Capture one frame of video when any key is pressed.
 * 	2. Display the captured frame when any key is pressed.		  
*/
/* Note: Set the switches SW1 and SW2 to high and rest of the switches to low for correct exposure timing while compiling and the loading the program in the Altera Monitor program.
*/
#define STANDARD_X 320
#define STANDARD_Y 240 
int main(void)
{
	volatile int * KEY_ptr				= (int *) KEY_BASE;
	volatile int * Video_In_DMA_ptr	= (int *) VIDEO_IN_BASE;
	volatile short * Video_Mem_ptr	= (short *) FPGA_ONCHIP_BASE;
	volatile int* SWITCH_ptr		= (int *) SW_BASE;
	uint8_t plaintext[9600][16];
	int encryption_standard = 0;
	int encrypt_yes = 1;
	struct AES_ctx ctx;
	uint8_t key[16] =        { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16,
							   (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, 
							   (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, 
							   (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };


	//get_image(plaintext, Video_Mem_ptr, Video_In_DMA_ptr,KEY_ptr);
	
	while(1){
		encryption_standard = (*SWITCH_ptr & SWITCH_9_MSK) >> 9;
		encrypt_yes = (*SWITCH_ptr & SWITCH_8_MSK) >> 8;
		update_and_draw_menu(encrypt_yes, encryption_standard);
		if(*SWITCH_ptr & SWITCH_7_MSK){
			get_image(plaintext, Video_Mem_ptr, Video_In_DMA_ptr,KEY_ptr);
		}
		
		if((*KEY_ptr & KEY_3_MSK) != 0){
			//wait for key release
			while ((*KEY_ptr & KEY_3_MSK) != 0); 
			
			//perform the operation
			if(encrypt_yes){
				//encrypt
				encrypt(encryption_standard,&ctx, key, plaintext);
			}else{
				//decrypt
				decrypt(encryption_standard,&ctx, key, plaintext);
			}
			displayText(plaintext, Video_Mem_ptr);
		}
		
	}
}

void put_jtag( char c )
{
	volatile int* JTAG_UART_ptr = (int *) 0xFF201000; 
	int control;
	control = *(JTAG_UART_ptr + 1);			// read the JTAG_UART control register
	if (control & 0xFFFF0000)					// if space, then echo character, else ignore 
		*(JTAG_UART_ptr) = c;
}

void displayText(uint8_t display_text[9600][16],volatile short *Video_Mem_ptr){
	int x,y,block,count;
	block=0;
	count=0;
	for (y = 0; y < 240; y++) {
		for (x = 0; x < 320; x++) {
			uint8_t temp1 = display_text[block][count];
			uint8_t temp2 = display_text[block][count+1];
			short temp = (((short)temp2) << 8) | (short)temp1;
			*(Video_Mem_ptr + (y << 9) + x) = temp;
			
			count+=2;
			if(count==16){
				block++;
				count=0;
			}
		}
	}
}

void encrypt(int encryption_standard, struct AES_ctx* ctx, uint8_t key[16], uint8_t plaintext[9600][16]){
	AES_init_ctx(ctx, key);
	int i;
	for(i=0;i<9600;i++){
		if(encryption_standard == 0){
			
			AES_ECB_encrypt(ctx, plaintext[i]);
			//memcpy(cipher_text[i],ctx.Iv,16);
		}
		if(encryption_standard == 1){
			AES_CBC_encrypt_buffer(ctx, plaintext[i],16);
			//memcpy(cipher_text[i],ctx.Iv,16);
									
			

		}
	}
}

void decrypt(int encryption_standard, struct AES_ctx* ctx, uint8_t key[16], uint8_t plaintext[9600][16]){
	AES_init_ctx(ctx, key);
	int i;
	for(i=0;i<9600;i++){
		if(encryption_standard == 0){
			
			AES_ECB_decrypt(ctx, plaintext[i]);
			//memcpy(cipher_text[i],ctx.Iv,16);
		}
		if(encryption_standard == 1){
			AES_CBC_decrypt_buffer(ctx, plaintext[i],16);
			//memcpy(cipher_text[i],ctx.Iv,16);
									
			

		}
	}
}

void get_image(uint8_t plaintext[9600][16], volatile short *Video_Mem_ptr, volatile int *Video_In_DMA_ptr, volatile int *KEY_ptr){
	*(Video_In_DMA_ptr + 3)	= 0x4;	
	while (1)
	{	
		
		if ((*KEY_ptr & KEY_3_MSK) != 0)		// check if KEY_3 was pressed
		{
		
			*(Video_In_DMA_ptr + 3) = 0x0;		// Disable the video to capture one frame
			while ((*KEY_ptr & KEY_3_MSK) != 0);				// wait for pushbutton KEY release			
			break;
		}
	}
	int count = 0;
	int block = 0;
	//get image from pixel buffer
	int x, y;
	for (y = 0; y < 240; y++) {
		for (x = 0; x < 320; x++) {
			short temp = *(Video_Mem_ptr + (y << 9) + x);
			uint8_t temp1 = temp & 0xFF;
			uint8_t temp2 = temp >> 8;
			plaintext[block][count] = temp1;
			plaintext[block][count+1] = temp2;
			count+=2;
			if(count==16){
				block++;
				count=0;
			}
		}
	}
}

void video_text(int x, int y, char * text_ptr) { 
	int offset; 
	volatile char * character_buffer = (char *)FPGA_CHAR_START; // video character buffer

/* assume that the text string fits on one line */ 
	offset = (y << 7) + x; 
	while (*(text_ptr)) {
		*(character_buffer + offset) = *(text_ptr); // write to the character buffer
		++text_ptr;
		++offset;
	}
}

void video_box(int x1, int y1, int x2, int y2, short pixel_color) {
	int pixel_buf_ptr = *(int *)PIXEL_BUF_CTRL_BASE;
	int pixel_ptr, row, col;
	int x_factor = 0x1; 
	int y_factor = 0x1; 
	x1 = x1 / x_factor; 
	x2 = x2 / x_factor; 
	y1 = y1 / y_factor; 
	y2 = y2 / y_factor;
/* assume that the box coordinates are valid */ 
	for (row = y1; row <= y2; row++) 
		for (col = x1; col <= x2; ++col) { 
			pixel_ptr = pixel_buf_ptr + (row << (10)) + (col << 1); 
			*(short *)pixel_ptr = pixel_color; // set pixel color 
		}
}

void draw_text_with_background(int x, int y, char * text_ptr, short pixel_color){
	size_t txt_len = strlen(text_ptr);
	
	int x_size = txt_len*4;
	int x_start = x*4;
	
	int y_size = 4;
	int y_start = y *4;
	
	video_text(x,y,text_ptr);
	video_box(x_start, y_start, x_start + x_size, y_start + y_size, pixel_color);
	
	
}

void update_and_draw_menu(int encrypt_yes, int encryption_standard){
	char standard_top_row[40]    = " Encryption Mode(SW9): ";
	char standard_bottom_row[40] = "         ECB/CBC           \0";
	
	char encrypt_top_row[40]     = " Direction(SW8): ";
	char encrypt_bottom_row[40]  = "      Encrypt/Decrypt    \0";
	
	char camera_top_row[40]      = " Picture Mode \0";
	char camera_bottom_row[40]   = "      SW7     \0";
	
	char action_top_row[40]      = " Key3 Runs \0";
	char action_bottom_row[40]   = " Operation \0";
	
	if(encryption_standard){
		strcpy(standard_top_row, concat(standard_top_row, "CBC \0"));
	}
	else{
		strcpy(standard_top_row, concat(standard_top_row, "ECB \0"));
	}
	
	if(encrypt_yes){
		strcpy(encrypt_top_row, concat(encrypt_top_row, "Encrypt \0"));
	}
	else{
		strcpy(encrypt_top_row, concat(encrypt_top_row, "Decrypt \0"));
	}
	size_t enc_start = strlen(standard_top_row) + 1;
	size_t cam_start = strlen(encrypt_top_row) + enc_start + 1;
	size_t act_start = strlen(camera_top_row) + cam_start + 1;

	draw_text_with_background(0, 57, standard_top_row, 0);
	draw_text_with_background(0, 58, standard_bottom_row, 0);
	
	draw_text_with_background(enc_start, 57, encrypt_top_row, 0);
	draw_text_with_background(enc_start, 58, encrypt_bottom_row, 0);
	
	draw_text_with_background(cam_start, 57, camera_top_row, 0);
	draw_text_with_background(cam_start, 58, camera_bottom_row, 0);
	
	draw_text_with_background(act_start, 57, action_top_row, 0);
	draw_text_with_background(act_start, 58, action_bottom_row, 0);
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}





