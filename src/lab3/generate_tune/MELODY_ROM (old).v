//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template ROM module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

module MELODY_ROM(	
	input [4:0] addr,
	input clk,
	output reg [1:0] q
);

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================

	 reg [4:0] melody[28:0];
	 
	 initial begin
		melody[0]  <= 3;
		melody[1]  <= 2;
		melody[2]  <= 1;
		melody[3]  <= 2;
		melody[4]  <= 3;
		melody[5]  <= 3;
		melody[6]  <= 3;
		melody[7]  <= 3;
		melody[8]  <= 2;
		melody[9]  <= 2;
		melody[10] <= 2;
		melody[11] <= 2;
		melody[12] <= 3;
		melody[13] <= 3;
		melody[14] <= 3;
		melody[15] <= 3;
		melody[16] <= 3;
		melody[17] <= 2;
		melody[18] <= 1;
		melody[19] <= 2;
		melody[20] <= 3;
		melody[21] <= 3;
		melody[22] <= 3;
		melody[23] <= 3;
		melody[24] <= 2;
		melody[25] <= 2;
		melody[26] <= 3;
		melody[27] <= 2;
		melody[28] <= 1;
	 end
	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
	always @ (posedge clk)
	begin
		q <= melody[addr];
	end
	 
endmodule
