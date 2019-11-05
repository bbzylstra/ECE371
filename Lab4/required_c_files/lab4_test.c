
///////////////////////////////////////////////////////////////////////////////
//
// TEST CODE - 371 Lab 4
//
// Run this to test if your LFSR is working
// Should print out random value when reset is low, seed when reset is high
//
// Example Output:
//
// RESET=0, OUT=0x                        00000000
// RESET=0, OUT=0x                        00000000
// DEFAULT, OUT=0x                        00000000
// DEFAULT, OUT=0x                        00000000
// RESET=1, OUT=0x                        12345678
// RESET=1, OUT=0x                        12345678
// DEFAULT, OUT=0x                        12345678
// DEFAULT, OUT=0x                        12345678
// RESET=0, OUT=0x                        00000000
// RESET=0, OUT=0x                        00000000
//
// Notes: 
// 1. RESET value stays at previously set value during DEFAULT
// 2. This output is expected based on the example LFSR
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "socal.h"
#include "hps.h"
#include "Computer_System.h"

#define SW_BASE				  0xFF200040

int main(void){										// main method
	
	volatile int * SW_ptr = (int *) SW_BASE;
	
	int read_output = 0;

	while (1){										// while loop
		
		switch(*SW_ptr){
			
			// RESET = 0
			case 14:								// Switch 3,2,1 high

				alt_write_byte(ALT_FPGA_BRIDGE_LWH2F_OFST + ARM_A9_HPS_LFSR_RESET_BASE, 0);
				read_output = alt_read_word(ALT_FPGA_BRIDGE_LWH2F_OFST + ARM_A9_HPS_LFSR_OUT_BASE);
				printf("RESET=0, OUT=0x%32X\n",read_output);
				break;
				
			// RESET = 1
			case 22:								// Switch 4,2,1 high

				alt_write_byte(ALT_FPGA_BRIDGE_LWH2F_OFST + ARM_A9_HPS_LFSR_RESET_BASE, 1);
				read_output = alt_read_word(ALT_FPGA_BRIDGE_LWH2F_OFST + ARM_A9_HPS_LFSR_OUT_BASE);
				printf("RESET=1, OUT=0x%32X\n",read_output);
				break;
		
			default:								// for any other switch combo

				read_output = alt_read_word(ALT_FPGA_BRIDGE_LWH2F_OFST + ARM_A9_HPS_LFSR_OUT_BASE);
				printf("DEFAULT, OUT=0x%32X\n",read_output);
				break;
			
		}	// end case switch

	}	// end while loop

}	// end main method
