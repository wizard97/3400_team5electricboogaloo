//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template ROM module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

module B4_493_ROM(	
	input [8:0] addr,
	input clk,
	output reg [7:0] q
);

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
	 reg [7:0] sin[253:0];
	 
	 initial begin
		sin[0]  <= 8'b10000000;
		sin[1]  <= 8'b10000011;
		sin[2]  <= 8'b10000110;
		sin[3]  <= 8'b10001001;
		sin[4]  <= 8'b10001100;
		sin[5]  <= 8'b10001111;
		sin[6]  <= 8'b10010010;
		sin[7]  <= 8'b10010110;
		sin[8]  <= 8'b10011001;
		sin[9]  <= 8'b10011100;
		sin[10] <= 8'b10011111;
		sin[11] <= 8'b10100010;
		sin[12] <= 8'b10100101;
		sin[13] <= 8'b10101000;
		sin[14] <= 8'b10101011;
		sin[15] <= 8'b10101110;
		sin[16] <= 8'b10110001;
		sin[17] <= 8'b10110100;
		sin[18] <= 8'b10110111;
		sin[19] <= 8'b10111001;
		sin[20] <= 8'b10111100;
		sin[21] <= 8'b10111111;
		sin[22] <= 8'b11000010;
		sin[23] <= 8'b11000100;
		sin[24] <= 8'b11000111;
		sin[25] <= 8'b11001010;
		sin[26] <= 8'b11001100;
		sin[27] <= 8'b11001111;
		sin[28] <= 8'b11010001;
		sin[29] <= 8'b11010100;
		sin[30] <= 8'b11010110;
		sin[31] <= 8'b11011000;
		sin[32] <= 8'b11011010;
		sin[33] <= 8'b11011101;
		sin[34] <= 8'b11011111;
		sin[35] <= 8'b11100001;
		sin[36] <= 8'b11100011;
		sin[37] <= 8'b11100101;
		sin[38] <= 8'b11100111;
		sin[39] <= 8'b11101001;
		sin[40] <= 8'b11101010;
		sin[41] <= 8'b11101100;
		sin[42] <= 8'b11101110;
		sin[43] <= 8'b11101111;
		sin[44] <= 8'b11110001;
		sin[45] <= 8'b11110010;
		sin[46] <= 8'b11110011;
		sin[47] <= 8'b11110101;
		sin[48] <= 8'b11110110;
		sin[49] <= 8'b11110111;
		sin[50] <= 8'b11111000;
		sin[51] <= 8'b11111001;
		sin[52] <= 8'b11111010;
		sin[53] <= 8'b11111011;
		sin[54] <= 8'b11111100;
		sin[55] <= 8'b11111100;
		sin[56] <= 8'b11111101;
		sin[57] <= 8'b11111101;
		sin[58] <= 8'b11111110;
		sin[59] <= 8'b11111110;
		sin[60] <= 8'b11111111;
		sin[61] <= 8'b11111111;
		sin[62] <= 8'b11111111;
		sin[63] <= 8'b11111111;
		sin[64] <= 8'b11111111;
		sin[65] <= 8'b11111111;
		sin[66] <= 8'b11111111;
		sin[67] <= 8'b11111110;
		sin[68] <= 8'b11111110;
		sin[69] <= 8'b11111110;
		sin[70] <= 8'b11111101;
		sin[71] <= 8'b11111101;
		sin[72] <= 8'b11111100;
		sin[73] <= 8'b11111011;
		sin[74] <= 8'b11111011;
		sin[75] <= 8'b11111010;
		sin[76] <= 8'b11111001;
		sin[77] <= 8'b11111000;
		sin[78] <= 8'b11110111;
		sin[79] <= 8'b11110101;
		sin[80] <= 8'b11110100;
		sin[81] <= 8'b11110011;
		sin[82] <= 8'b11110001;
		sin[83] <= 8'b11110000;
		sin[84] <= 8'b11101110;
		sin[85] <= 8'b11101101;
		sin[86] <= 8'b11101011;
		sin[87] <= 8'b11101010;
		sin[88] <= 8'b11101000;
		sin[89] <= 8'b11100110;
		sin[90] <= 8'b11100100;
		sin[91] <= 8'b11100010;
		sin[92] <= 8'b11100000;
		sin[93] <= 8'b11011110;
		sin[94] <= 8'b11011100;
		sin[95] <= 8'b11011001;
		sin[96] <= 8'b11010111;
		sin[97] <= 8'b11010101;
		sin[98] <= 8'b11010010;
		sin[99] <= 8'b11010000;
		sin[100] <= 8'b11001110;
		sin[101] <= 8'b11001011;
		sin[102] <= 8'b11001000;
		sin[103] <= 8'b11000110;
		sin[104] <= 8'b11000011;
		sin[105] <= 8'b11000000;
		sin[106] <= 8'b10111110;
		sin[107] <= 8'b10111011;
		sin[108] <= 8'b10111000;
		sin[109] <= 8'b10110101;
		sin[110] <= 8'b10110010;
		sin[111] <= 8'b10110000;
		sin[112] <= 8'b10101101;
		sin[113] <= 8'b10101010;
		sin[114] <= 8'b10100111;
		sin[115] <= 8'b10100100;
		sin[116] <= 8'b10100001;
		sin[117] <= 8'b10011101;
		sin[118] <= 8'b10011010;
		sin[119] <= 8'b10010111;
		sin[120] <= 8'b10010100;
		sin[121] <= 8'b10010001;
		sin[122] <= 8'b10001110;
		sin[123] <= 8'b10001011;
		sin[124] <= 8'b10001000;
		sin[125] <= 8'b10000100;
		sin[126] <= 8'b10000001;
		sin[127] <= 8'b01111110;
		sin[128] <= 8'b01111011;
		sin[129] <= 8'b01111000;
		sin[130] <= 8'b01110101;
		sin[131] <= 8'b01110001;
		sin[132] <= 8'b01101110;
		sin[133] <= 8'b01101011;
		sin[134] <= 8'b01101000;
		sin[135] <= 8'b01100101;
		sin[136] <= 8'b01100010;
		sin[137] <= 8'b01011111;
		sin[138] <= 8'b01011100;
		sin[139] <= 8'b01011001;
		sin[140] <= 8'b01010110;
		sin[141] <= 8'b01010011;
		sin[142] <= 8'b01010000;
		sin[143] <= 8'b01001101;
		sin[144] <= 8'b01001010;
		sin[145] <= 8'b01000111;
		sin[146] <= 8'b01000100;
		sin[147] <= 8'b01000010;
		sin[148] <= 8'b00111111;
		sin[149] <= 8'b00111100;
		sin[150] <= 8'b00111001;
		sin[151] <= 8'b00110111;
		sin[152] <= 8'b00110100;
		sin[153] <= 8'b00110010;
		sin[154] <= 8'b00101111;
		sin[155] <= 8'b00101101;
		sin[156] <= 8'b00101010;
		sin[157] <= 8'b00101000;
		sin[158] <= 8'b00100110;
		sin[159] <= 8'b00100100;
		sin[160] <= 8'b00100001;
		sin[161] <= 8'b00011111;
		sin[162] <= 8'b00011101;
		sin[163] <= 8'b00011011;
		sin[164] <= 8'b00011001;
		sin[165] <= 8'b00010111;
		sin[166] <= 8'b00010110;
		sin[167] <= 8'b00010100;
		sin[168] <= 8'b00010010;
		sin[169] <= 8'b00010001;
		sin[170] <= 8'b00001111;
		sin[171] <= 8'b00001110;
		sin[172] <= 8'b00001100;
		sin[173] <= 8'b00001011;
		sin[174] <= 8'b00001010;
		sin[175] <= 8'b00001001;
		sin[176] <= 8'b00000111;
		sin[177] <= 8'b00000110;
		sin[178] <= 8'b00000101;
		sin[179] <= 8'b00000101;
		sin[180] <= 8'b00000100;
		sin[181] <= 8'b00000011;
		sin[182] <= 8'b00000010;
		sin[183] <= 8'b00000010;
		sin[184] <= 8'b00000001;
		sin[185] <= 8'b00000001;
		sin[186] <= 8'b00000001;
		sin[187] <= 8'b00000000;
		sin[188] <= 8'b00000000;
		sin[189] <= 8'b00000000;
		sin[190] <= 8'b00000000;
		sin[191] <= 8'b00000000;
		sin[192] <= 8'b00000000;
		sin[193] <= 8'b00000000;
		sin[194] <= 8'b00000001;
		sin[195] <= 8'b00000001;
		sin[196] <= 8'b00000001;
		sin[197] <= 8'b00000010;
		sin[198] <= 8'b00000011;
		sin[199] <= 8'b00000011;
		sin[200] <= 8'b00000100;
		sin[201] <= 8'b00000101;
		sin[202] <= 8'b00000110;
		sin[203] <= 8'b00000111;
		sin[204] <= 8'b00001000;
		sin[205] <= 8'b00001001;
		sin[206] <= 8'b00001010;
		sin[207] <= 8'b00001011;
		sin[208] <= 8'b00001101;
		sin[209] <= 8'b00001110;
		sin[210] <= 8'b00010000;
		sin[211] <= 8'b00010001;
		sin[212] <= 8'b00010011;
		sin[213] <= 8'b00010101;
		sin[214] <= 8'b00010110;
		sin[215] <= 8'b00011000;
		sin[216] <= 8'b00011010;
		sin[217] <= 8'b00011100;
		sin[218] <= 8'b00011110;
		sin[219] <= 8'b00100000;
		sin[220] <= 8'b00100010;
		sin[221] <= 8'b00100100;
		sin[222] <= 8'b00100111;
		sin[223] <= 8'b00101001;
		sin[224] <= 8'b00101011;
		sin[225] <= 8'b00101110;
		sin[226] <= 8'b00110000;
		sin[227] <= 8'b00110011;
		sin[228] <= 8'b00110101;
		sin[229] <= 8'b00111000;
		sin[230] <= 8'b00111010;
		sin[231] <= 8'b00111101;
		sin[232] <= 8'b01000000;
		sin[233] <= 8'b01000010;
		sin[234] <= 8'b01000101;
		sin[235] <= 8'b01001000;
		sin[236] <= 8'b01001011;
		sin[237] <= 8'b01001110;
		sin[238] <= 8'b01010001;
		sin[239] <= 8'b01010100;
		sin[240] <= 8'b01010111;
		sin[241] <= 8'b01011010;
		sin[242] <= 8'b01011101;
		sin[243] <= 8'b01100000;
		sin[244] <= 8'b01100011;
		sin[245] <= 8'b01100110;
		sin[246] <= 8'b01101001;
		sin[247] <= 8'b01101100;
		sin[248] <= 8'b01101111;
		sin[249] <= 8'b01110011;
		sin[250] <= 8'b01110110;
		sin[251] <= 8'b01111001;
		sin[252] <= 8'b01111100;
		sin[253] <= 8'b01111111;
	 end
	 
    //=======================================================
    //  Structural coding
    //=======================================================
	 always @(posedge clk) begin
		q <= sin[addr];
	 end
	 
endmodule