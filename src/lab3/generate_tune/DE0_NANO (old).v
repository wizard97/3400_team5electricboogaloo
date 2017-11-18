//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template top-level module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

`define HALF_SEC 25000000/2

module DE0_NANO(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// KEY //////////
	KEY,

	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	//GPIO_0_D,
	//GPIO_0_IN,

	//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	GPIO_1_D,
	//GPIO_1_IN,
);
	 
	 //=======================================================
	 //  PARAMETER declarations
	 //=======================================================
	 
	 localparam HALF_SEC = 25000000/2;
	 
	 //=======================================================
	 //  PORT declarations
	 //=======================================================

	 //////////// CLOCK //////////
	 input 		          		CLOCK_50;

	 /////////// KEY //////////
	 input 		     [1:0]		KEY;

	 //////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	 //inout 		    [33:0]		GPIO_0_D;
	 //input 		     [1:0]		GPIO_0_IN;

	 //////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_1_D;
	 //input 		     [1:0]		GPIO_1_IN;

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
	 
    reg                       CLOCK_25;
	 
	 reg            [23:0]     counter;
	 reg             [4:0]     addr;
	 reg             [9:0]     tone_counter;
	 reg             [6:0]     counter_90;
	 reg             [7:0]     tone;
	 
	 wire            [1:0]     q;
	 wire            [7:0]     a_q;
	 wire            [7:0]     b_q;
	 wire            [7:0]     csharp_q;
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	 assign GPIO_1_D[0] = tone[0];
	 assign GPIO_1_D[1] = tone[1];
	 assign GPIO_1_D[2] = tone[2];
	 assign GPIO_1_D[3] = tone[3];
	 assign GPIO_1_D[4] = tone[4];
	 assign GPIO_1_D[5] = tone[5];
	 assign GPIO_1_D[6] = tone[6];
	 assign GPIO_1_D[7] = tone[7];
	 
	 A_440_ROM a_rom(
		.addr(tone_counter),
		.clk(CLOCK_25),
		.q(a_q)
	 );

	 B_493_ROM b_rom(
		.addr(tone_counter),
		.clk(CLOCK_25),
		.q(b_q)
	 );

	 CSHARP_554_ROM c_rom(
		.addr(tone_counter),
		.clk(CLOCK_25),
		.q(csharp_q)
	 );

	 MELODY_ROM mel_rom(
		.addr(addr),
		.clk(CLOCK_25),
		.q(q)
	 );
	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
	 // Generate 25MHz clock for VGA, FPGA has 50 MHz clock
    always @ (posedge CLOCK_50) begin
        CLOCK_25 <= ~CLOCK_25; 
    end
	
	 // Simple state machine
	 always @ (posedge CLOCK_25) begin
		  if (reset) begin
				counter      <= HALF_SEC;
				counter_90   <= 90;
				addr         <= 0;
				tone_counter <= 0;
				tone         <= csharp_q;
		  end
		  else if (counter == 0) begin
				counter      <= HALF_SEC;
				counter_90   <= 90;
				if (addr == 25) begin
					addr <= 0;
				end
				else begin
					addr <= addr + 1;
				end
				tone_counter <= 0;
		  end
		  else begin
			   if (q == 3) begin
				   if (tone_counter == 501) begin
						tone_counter <= 0;
					end
					tone <= csharp_q;
				end
				else if (q == 2) begin
					if (tone_counter == 562) begin
						tone_counter <= 0;
					end
					tone <= b_q;
				end
				else begin
					if (tone_counter == 632) begin
						tone_counter <= 0;
					end
					tone <= a_q;
				end
				
				if (counter_90 == 0) begin
					counter_90 <= 90;
					tone_counter <= tone_counter + 1;
				end
				counter	    <= counter - 1;
				counter_90   <= counter_90 - 1;
			end
	 end
	 
endmodule
