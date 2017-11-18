//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template top-level module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

`define FIFTH_SEC 25000000/5

module DE0_NANO(
	//////////// CLOCK //////////
	CLOCK_50,

	//////////// KEY //////////
	KEY,

	//////////// GPIO_1 //////////
	GPIO_1_D
);
	 
	 //=======================================================
	 //  PARAMETER declarations
	 //=======================================================
	 localparam FIFTH_SEC = 25000000/5;
	 
	 //=======================================================
	 //  PORT declarations
	 //=======================================================
	 //////////// CLOCK //////////
	 input 		  CLOCK_50;

	 /////////// KEY //////////
	 input [1:0]  KEY;

	 //////////// GPIO_1 //////////
	 inout [33:0] GPIO_1_D;

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
    reg          CLOCK_25;
	 
	 reg   [22:0] counter;
	 reg   [7:0]  tone_counter;
	 reg   [4:0]  addr;
	 reg   [9:0]  tone_addr;
	 reg   [7:0]  tone;
	 
	 wire  [2:0]  q;
	 wire  [7:0]  d3_q;
	 wire  [7:0]  g3_q;
	 wire  [7:0]  a3_q;
	 wire  [7:0]  b3_q;
	 wire  [7:0]  c4_q;
	 wire  [7:0]  d4_q;
	 wire  [7:0]  g4_q;
	 
	 assign reset       = ~KEY[0]; // reset when KEY0 is pressed
	 
	 assign GPIO_1_D[0] = tone[0];
	 assign GPIO_1_D[1] = tone[1];
	 assign GPIO_1_D[2] = tone[2];
	 assign GPIO_1_D[3] = tone[3];
	 assign GPIO_1_D[4] = tone[4];
	 assign GPIO_1_D[5] = tone[5];
	 assign GPIO_1_D[6] = tone[6];
	 assign GPIO_1_D[7] = tone[7];
	 
	 D3_146_ROM d3_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(d3_q)
	 );

	 G3_196_ROM g3_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(g3_q)
	 );

	 A3_220_ROM a3_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(a3_q)
	 );
	 
	 B3_246_ROM b3_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(b3_q)
	 );
	 
	 C4_261_ROM c4_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(c4_q)
	 );
	 
	 D4_293_ROM d4_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(d4_q)
	 );
	 
	 G4_392_ROM g4_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(g4_q)
	 );

	 MELODY_ROM mel_rom(
		.addr(addr),
		.clk(CLOCK_25),
		.q(q)
	 );
	 
    //=======================================================
    //  Structural coding
    //=======================================================
	 // Generate 25MHz clock for VGA
    always @(posedge CLOCK_50) begin
		CLOCK_25        <= ~CLOCK_25; 
    end
	
	 // State machine
	 always @(posedge CLOCK_25) begin
		if(reset) begin
			counter      <= FIFTH_SEC;
			tone_counter <= 200;
			addr         <= 0;
			tone_addr    <= 0;
			tone         <= d3_q;
		end
		else if(counter == 0) begin
			counter      <= FIFTH_SEC;
			tone_counter <= 200;
			if(addr == something) begin
				addr      <= 0;
			end
			else begin
				addr      <= addr + 1;
			end
			tone_addr    <= 0;
		end
		else begin
			counter            <= counter - 1;
			if(tone_counter == 0) begin
				tone_counter    <= 200;
				tone_addr       <= tone_addr + 1;
			end
			else begin
				tone_counter    <= tone_counter - 1;
			end
			case(q)
				0: begin
					if(tone_addr == 852) begin
						tone_addr <= 0;
					end
					tone         <= d3_q;
					end
				1: begin
					if(tone_addr == 638) begin
						tone_addr <= 0;
					end
					tone         <= g3_q;
					end
				2: begin
					if(tone_addr == 569) begin
						tone_addr <= 0;
					end
					tone         <= a3_q;
					end
				3: begin
					if(tone_addr == 507) begin
						tone_addr <= 0;
					end
					tone         <= b3_q;
					end
				4: begin
					if(tone_addr == 478) begin
						tone_addr <= 0;
					end
					tone         <= c4_q;
					end
				5: begin
					if(tone_addr == 425) begin
						tone_addr <= 0;
					end
					tone         <= d4_q;
					end
				6:	begin
					if(tone_addr == 319) begin
						tone_addr <= 0;
					end
					tone         <= g4_q;
					end
				default: tone   <= d3_q;
			endcase
		end
	 end
	 
endmodule