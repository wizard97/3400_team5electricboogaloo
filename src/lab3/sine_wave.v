//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template top-level module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

`define COUNT 25000000/440/2

module DE0_NANO(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// LED //////////
	//LED,

	//////////// KEY //////////
	KEY,

	//////////// SW //////////
	//SW,

	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	//GPIO_0_D,
	//GPIO_0_IN,

	//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	GPIO_1_D,
	//GPIO_1_IN,
	
	GPIO_2_IN,
	GPIO_3_IN,
	GPIO_4_IN,
	GPIO_5_IN,
	GPIO_6_IN,
	GPIO_7_IN,
	GPIO_8_IN,
	GPIO_9_IN,
);

	 //=======================================================
	 //  PARAMETER declarations
	 //=======================================================

	 localparam COUNT = 25000000/440/2; // one second in 25MHz clock cycles
	 
	 //=======================================================
	 //  PORT declarations
	 //=======================================================

	 //////////// CLOCK //////////
	 input 		          		CLOCK_50;

	 //////////// LED //////////
	 //output		     [7:0]		LED;

	 /////////// KEY //////////
	 input 		     [1:0]		KEY;

	 //////////// SW //////////
	 //input 		     [3:0]		SW;

	 //////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	 //inout 		    [33:0]		GPIO_0_D;
	 //input 		     [1:0]		GPIO_0_IN;

	 //////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_1_D;
	 //input 		     [1:0]		GPIO_1_IN;
	 
	 input			  [1:0]		GPIO_2_IN;
	 input			  [1:0]		GPIO_3_IN;
	 input			  [1:0]		GPIO_4_IN;
	 input			  [1:0]		GPIO_5_IN;
	 input			  [1:0]		GPIO_6_IN;
	 input			  [1:0]		GPIO_7_IN;
	 input			  [1:0]		GPIO_8_IN;
	 input			  [1:0]		GPIO_9_IN;

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
    reg         CLOCK_25;
    //wire        reset; // active high reset signal 

    //wire [9:0]  PIXEL_COORD_X; // current x-coord from VGA driver
    //wire [9:0]  PIXEL_COORD_Y; // current y-coord from VGA driver
    //wire [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 
	 reg [15:0]  counter; // timer to keep track of when to toggle square wave
	 reg  [7:0]  sine_wave[628:0];
	 
	 assign GPIO_1_D[0] = sine_wave;
	 //reg 			led_state;   // 1 is on, 0 is off
	 
    // Module outputs coordinates of next pixel to be written onto screen
    /*VGA_DRIVER driver(
		  .RESET(reset),
        .CLOCK(CLOCK_25),
        .PIXEL_COLOR_IN(PIXEL_COLOR),
        .PIXEL_X(PIXEL_COORD_X),
        .PIXEL_Y(PIXEL_COORD_Y),
        .PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
        .H_SYNC_NEG(GPIO_0_D[7]),
        .V_SYNC_NEG(GPIO_0_D[5])
    );*/
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	 //assign PIXEL_COLOR = 8'b000_111_00; // Green
	 //assign LED[0] = led_state;
	 
	 initial begin
	 end
	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
	 // Generate 25MHz clock for VGA, FPGA has 50 MHz clock
    always @ (posedge CLOCK_50) begin
        CLOCK_25 <= ~CLOCK_25; 
    end // always @ (posedge CLOCK_50) 
	
	 // Simple state machine to toggle square wave every one second
	 always @ (posedge CLOCK_25) begin
		  if (reset) begin
				counter <= COUNT - 1;
				square_wave <= 1;
		  end
		  
		  else if (counter == 0) begin
				counter		<= COUNT - 1;
				square_wave <= ~square_wave;
		  end
		  /*if (led_counter == ONE_SEC) begin
				led_state   <= ~led_state;
				led_counter <= 25'b0;
		  end*/
		  else begin	
				//led_state   <= led_state;
				counter		<= counter - 1;
				square_wave <= square_wave;
		  end // always @ (posedge CLOCK_25)
	 end
	 

endmodule
