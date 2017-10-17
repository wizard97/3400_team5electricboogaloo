//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template ROM module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

module MELODY_ROM(	
	input [2:0] addr,
	input clk,
	output reg [1:0] q
);

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================

	 reg [1:0] sin[6:0];
	 
	 initial begin
		melody[0]  <= 3
		melody[1]  <= 2
		melody[2]  <= 1
		melody[3]  <= 2
		melody[4]  <= 3
		melody[5]  <= 3
		melody[6]  <= 3
	 end
	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
	always @ (posedge clk)
	begin
		q <= melody[addr];
	end
	 
endmodule
