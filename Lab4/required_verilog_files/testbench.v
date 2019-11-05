module tb;
  reg clk, reset;
  wire[31:0] out;
  
  lfsr_32 lfsr_32(
    .clk 	(clk),
    .reset 	(reset),
    .out	(out)
  );
  
  initial
    begin
      clk = 0;
      reset = 0;
    end
  
  always
    #50 clk = !clk;
  
  always
    #1000 reset = !reset;
  
  initial
    #5000 $finish;
  
endmodule