//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template ROM module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

module MELODY_ROM(	
	input [5:0] addr,
	input clk,
	output reg [2:0] q
);

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================

	 reg [5:0] melody[41:0];
	 
	 initial begin
		melody[0]  <= 0;
		melody[1]  <= 0;
		melody[2]  <= 0;
		melody[3]  <= 1;
		melody[4]  <= 1;
		melody[5]  <= 1;
		melody[6]  <= 1;
		melody[7]  <= 1;
		melody[8]  <= 1;
		melody[9]  <= 5;
		melody[10] <= 5;
		melody[11] <= 5;
		melody[12] <= 4;
		melody[13] <= 3;
		melody[14] <= 2;
		melody[15] <= 6;
		melody[16] <= 6;
		melody[17] <= 6;
		melody[18] <= 6;
		melody[19] <= 6;
		melody[20] <= 6;
		melody[21] <= 5;
		melody[22] <= 5;
		melody[23] <= 5;
		melody[24] <= 4;
		melody[25] <= 3;
		melody[26] <= 2;
		melody[27] <= 6;
		melody[28] <= 6;
		melody[29] <= 6;
		melody[30] <= 6;
		melody[31] <= 6;
		melody[32] <= 6;
		melody[33] <= 5;
		melody[34] <= 5;
		melody[35] <= 5;
		melody[36] <= 4;
		melody[37] <= 3;
		melody[38] <= 4;
		melody[39] <= 2;
		melody[40] <= 2;
		melody[41] <= 2;
	 end
	 
    //=======================================================
    //  Structural coding
    //=======================================================
	 always @(posedge clk) begin
		q <= melody[addr];
	 end
	 
endmodule