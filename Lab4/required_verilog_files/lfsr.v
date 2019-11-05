// Example LFSR Module

module lfsr_32 (
    clk,              // clock input
    reset,            // reset input
	 out               // random output
);

input wire clk, reset;						// set as inputs
output reg [31:0] out = 32'h12345678;  // initial seed value

wire linear_feedback;

// TODO: assign polynomial to linear feedback wire
// assign linear_feedback = TODO;

always @(posedge clk)		// update on rising clock edge

	if (reset) begin 			// if reset is high
		out <= 32'h12345678;	// reset seed value
	  
	end else begin				// if reset is low
	
		// TODO: replace hard coded number with
		// some combination of output and linear feedback
		out <= 32'h00000000;
		
	end 
 
endmodule
