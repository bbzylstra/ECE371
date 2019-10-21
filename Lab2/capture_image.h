#ifndef _CAPTURE_IMAGE_H_
#define _CAPTURE_IMAGE_H_

void put_jtag( char c );
void displayText(uint8_t display_text[9600][16], volatile short *Video_Mem_ptr);
void encrypt(int encryption_standard, struct AES_ctx* ctx, uint8_t key[16], uint8_t plaintext[9600][16]);
void decrypt(int encryption_standard, struct AES_ctx* ctx, uint8_t key[16], uint8_t plaintext[9600][16]);
void get_image(uint8_t plaintext[9600][16], volatile short *Video_Mem_ptr, volatile int *Video_In_DMA_ptr, volatile int *KEY_ptr);
void video_text(int x, int y, char * text_ptr);
void video_box(int x1, int y1, int x2, int y2, short pixel_color);
void draw_text_with_background(int x, int y, char * text_ptr, short pixel_color);
void update_and_draw_menu(int encrypt_yes, int encryption_standard);
char* concat(const char *s1, const char *s2);
#endif