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
	 reg   [8:0]  tone_addr;
	 reg   [7:0]  tone;
	 
	 wire  [2:0]  q;
	 wire  [7:0]  d4_q;
	 wire  [7:0]  g4_q;
	 wire  [7:0]  a4_q;
	 wire  [7:0]  b4_q;
	 wire  [7:0]  c5_q;
	 wire  [7:0]  d5_q;
	 wire  [7:0]  g5_q;
	 
	 assign reset       = ~KEY[0]; // reset when KEY0 is pressed
	 
	 assign GPIO_1_D[0] = tone[0];
	 assign GPIO_1_D[1] = tone[1];
	 assign GPIO_1_D[2] = tone[2];
	 assign GPIO_1_D[3] = tone[3];
	 assign GPIO_1_D[4] = tone[4];
	 assign GPIO_1_D[5] = tone[5];
	 assign GPIO_1_D[6] = tone[6];
	 assign GPIO_1_D[7] = tone[7];
	 
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

	 A4_440_ROM a4_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(a4_q)
	 );
	 
	 B4_493_ROM b4_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(b4_q)
	 );
	 
	 C5_523_ROM c5_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(c5_q)
	 );
	 
	 D5_587_ROM d5_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(d5_q)
	 );

	 G5_783_ROM g5_rom(
		.addr(tone_addr),
		.clk(CLOCK_25),
		.q(g5_q)
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
			tone         <= d4_q;
		end
		else if(counter == 0) begin
			counter      <= FIFTH_SEC;
			tone_counter <= 200;
			if(addr == 41) begin
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
					if(tone_addr == 426) begin
						tone_addr <= 0;
					end
					tone         <= d4_q;
					end
				1: begin
					if(tone_addr == 319) begin
						tone_addr <= 0;
					end
					tone         <= g4_q;
					end
				2: begin
					if(tone_addr == 285) begin
						tone_addr <= 0;
					end
					tone         <= a4_q;
					end
				3: begin
					if(tone_addr == 254) begin
						tone_addr <= 0;
					end
					tone         <= b4_q;
					end
				4: begin
					if(tone_addr == 239) begin
						tone_addr <= 0;
					end
					tone         <= c5_q;
					end
				5: begin
					if(tone_addr == 213) begin
						tone_addr <= 0;
					end
					tone         <= d5_q;
					end
				6:	begin
					if(tone_addr == 160) begin
						tone_addr <= 0;
					end
					tone         <= g5_q;
					end
				default: tone   <= d4_q;
			endcase
		end
	 end
	 
endmodule