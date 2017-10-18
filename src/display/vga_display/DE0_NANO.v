//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template top-level module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

`define ONE_SEC 25000000

module DE0_NANO(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// LED //////////
	LED,

	//////////// KEY //////////
	KEY,

	//////////// SW //////////
	SW,

	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	GPIO_0_D,
	GPIO_0_IN,

	//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	GPIO_1_D,
	GPIO_1_IN,
);

	 //=======================================================
	 //  PARAMETER declarations
	 //=======================================================

	 localparam ONE_SEC = 25000000; // one second in 25MHz clock cycles
	 
	 //=======================================================
	 //  PORT declarations
	 //=======================================================

	 //////////// CLOCK //////////
	 input 		          		CLOCK_50;

	 //////////// LED //////////
	 output		     [7:0]		LED;

	 /////////// KEY //////////
	 input 		     [1:0]		KEY;

	 //////////// SW //////////
	 input 		     [3:0]		SW;

	 //////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_0_D;
	 input 		     [1:0]		GPIO_0_IN;

	 //////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_1_D;
	 input 		     [1:0]		GPIO_1_IN;

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
    reg         CLOCK_25;
    wire        reset; // active high reset signal 

    wire [9:0]  PIXEL_COORD_X; // current x-coord from VGA driver
    wire [9:0]  PIXEL_COORD_Y; // current y-coord from VGA driver
    wire [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 
	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off
	 
	 wire [9:0]  PIXEL_COORD_NEXT_X; // current x-coord from VGA driver
    wire [9:0]  PIXEL_COORD_NEXT_Y; // current y-coord from VGA driver
	 
	 
	 wire [9:0]  PIXEL_COORD_SHIFT_X;
	 wire [9:0]  PIXEL_COORD_NEXT_SHIFT_X;
	 
	 // Shift to middle of screen
	 assign PIXEL_COORD_SHIFT_X =  PIXEL_COORD_X - 10'd80;
	 assign PIXEL_COORD_NEXT_SHIFT_X =  PIXEL_COORD_NEXT_X - 10'd80;
	 
    // Module outputs coordinates of next pixel to be written onto screen
    VGA_DRIVER driver(
		  .RESET(reset),
        .CLOCK(CLOCK_25),
        .PIXEL_COLOR_IN(PIXEL_COLOR),
        .PIXEL_X(PIXEL_COORD_X),
        .PIXEL_Y(PIXEL_COORD_Y),
        .PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
        .H_SYNC_NEG(GPIO_0_D[7]),
        .V_SYNC_NEG(GPIO_0_D[5]),
		  
		  .PIXEL_X_NEXT(PIXEL_COORD_NEXT_X),
		  .PIXEL_Y_NEXT(PIXEL_COORD_NEXT_Y)
    );
	 
	 // Spi driver for communication
	 wire [23:0] arduino_data;
	 
	 wire dummy;
	 wire spi_done;
	 
	 // The SPI module that will assert done after receiving 3 bytes
	 spi_slave arduino(
    .clk(CLOCK_25),
    .rst(reset),
    .ss(GPIO_1_D[29]),
    .mosi(GPIO_1_D[31]),
    .miso(dummy),
    .sck(GPIO_1_D[33]),
    .done(spi_done),
    .din(0),
    .dout(arduino_data)
	);
	
	wire [15:0] spi_cord_addr;
	wire [7:0] spi_cord_x;
	wire [7:0] spi_cord_y;
	wire [7:0] spi_color;
	wire spi_valid;
	
	
	
	assign spi_cord_x = arduino_data[23:16];
	assign spi_cord_y = arduino_data[15:8];
	assign spi_cord_addr = spi_cord_x*240 + spi_cord_y; //generate the mem address to store
	assign spi_color = arduino_data[7:0];
	assign spi_valid = spi_done && spi_cord_x < 240 && spi_cord_y < 240;
	
	
	wire [15:0] memreq_addr;
	wire [7:0] memresp;

	
	assign memreq_addr = (PIXEL_COORD_NEXT_SHIFT_X < 480 && PIXEL_COORD_NEXT_Y < 480) ? 
										(PIXEL_COORD_NEXT_SHIFT_X/2)*240 + (PIXEL_COORD_NEXT_Y/2) : 0;
										
										
	assign PIXEL_COLOR = (PIXEL_COORD_SHIFT_X < 480 && PIXEL_COORD_Y < 480) ? memresp : 0;
	
	/* Test

	always @(*) begin

		if (PIXEL_COORD_X == 0 && PIXEL_COORD_Y == 0)
			PIXEL_COLOR = 8'b11100000;
		else if (PIXEL_COORD_X == 475 && PIXEL_COORD_Y == 475)
			PIXEL_COLOR = 8'b11100000;
		else if (PIXEL_COORD_X == 0 && PIXEL_COORD_Y == 475)
			PIXEL_COLOR = 8'b11100000;
		else if (PIXEL_COORD_X == 475 && PIXEL_COORD_Y == 0)
			PIXEL_COLOR = 8'b11100000;
		else
			PIXEL_COLOR = 0;
			
		
	end
	
	*/


	
		// framebuffer
	VGA_FRAMEBUFFER framebuf(
	.clock(CLOCK_25),
	.data(spi_color),
	.rdaddress(memreq_addr),
	.wraddress(spi_cord_addr),
	.wren(spi_valid),
	.q(memresp));



	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	 //assign PIXEL_COLOR = 8'b000_111_00; // Green
	 assign LED[0] = led_state;

	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
	 // Generate 25MHz clock for VGA, FPGA has 50 MHz clock
    always @ (posedge CLOCK_50) begin
        CLOCK_25 <= ~CLOCK_25; 
    end // always @ (posedge CLOCK_50)
	
	 // Simple state machine to toggle LED0 every one second
	 always @ (posedge CLOCK_25) begin
		  if (reset) begin
				led_state   <= 1'b0;
				led_counter <= 25'b0;
		  end
		  
		  if (led_counter == ONE_SEC) begin
				led_state   <= ~led_state;
				led_counter <= 25'b0;
		  end
		  else begin	
				led_state   <= led_state;
				led_counter <= led_counter + 25'b1;
		  end // always @ (posedge CLOCK_25)
	 end
	 

endmodule
