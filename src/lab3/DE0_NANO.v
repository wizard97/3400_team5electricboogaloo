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
    reg [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 
	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off
	 
    // Module outputs coordinates of next pixel to be written onto screen
    VGA_DRIVER driver(
		  .RESET(reset),
        .CLOCK(CLOCK_25),
        .PIXEL_COLOR_IN(PIXEL_COLOR),
        .PIXEL_X(PIXEL_COORD_X),
        .PIXEL_Y(PIXEL_COORD_Y),
        .PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
        .H_SYNC_NEG(GPIO_0_D[7]),
        .V_SYNC_NEG(GPIO_0_D[5])
    );
	 
	 // Spi driver for communication
	 wire [23:0] arduino_data;

	 
	 wire dummy;
	 wire spi_done;
	 
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
	
	wire [7:0] spi_cord_x;
	wire [7:0] spi_cord_y;
	wire [7:0] spi_color;
	
	assign spi_cord_x = arduino_data[23:16];
	assign spi_cord_y = arduino_data[15:8];
	assign spi_color = arduino_data[7:0];
	
		
	reg [7:0] crap[0:63][0:31];
	
	wire [5:0] new_x_cord;
	assign new_x_cord = PIXEL_COORD_X/11;
	wire [4:0] new_y_cord; 
	assign new_y_cord = PIXEL_COORD_Y/15;

	integer i;
	integer j;
   
	
   always @ (posedge CLOCK_25) begin
	
     for (i = 0; i < 64; i = i + 1) begin
	  
	    for (j = 0; j < 32; j = j + 1) begin
				if (reset) begin
					crap [i][j] <= 0;
				end 
				/*
				else begin
					crap [i][j] <= crap [i][j];
				end
				*/
				//crap [i][j] <= (reset) ? 0 : (spi_done) ? spi_color : crap [i][j];
				
				// Handle vga output
				if (new_x_cord == i && new_y_cord == j) begin
					PIXEL_COLOR <= crap [i][j];
				end 
		 end
	  end
	  
		//update from spi
	  	if (!reset && spi_done && spi_cord_x < 64 && spi_cord_y < 32) begin
			crap[spi_cord_x][spi_cord_y] <= spi_color;
		end
	  
	 end

	  
	  /*
	 always @ (posedge CLOCK_25) begin
		
		if (reset) begin
			PIXEL_COLOR <= 0;
		end else if (spi_done) begin
			PIXEL_COLOR <= arduino_data;
		end else begin
			PIXEL_COLOR <= PIXEL_COLOR;
		end
	 end
	 */
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	 //assign PIXEL_COLOR = 8'b000_111_00; // Green
	 assign LED[0] = led_state;
	 /*
	 always @ (posedge CLOCK_25) begin //PIXEL_COORD_X PIXEL_COORD_Y
		if (reset) begin
			PIXEL_COLOR <= 0;
		end else if (spi_done) begin
			PIXEL_COLOR <= arduino_data;
		end else begin
			PIXEL_COLOR <= PIXEL_COLOR;
		end
	 end
	 */
	 
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
