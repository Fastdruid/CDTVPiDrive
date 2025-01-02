/*
 * Copyright 2025 David Sharp
*/

// Revision History
/* v0.0.1 : 2025-02-01 : BRAND NEW! Its so *SHINY*! Its got all that "new code" smell and everything.
*                      : Initial version creates EFFK & SCOR clocks as well as repads the I2S to 24 bit frames. 
*
*
*/ 

module pidrive(


    output reg AEMP, // Audio Emphasis. Not really used but not a big deal to apply it correctly! 
    output reg INAC, // Drive activity indicator. 
    output reg IC5_OE, // Output enable IC5
    output reg IC4_OE, // Output enable IC4
    output reg IC3_OE, // Output enable IC3
    output reg IC2_OE, // Output enable IC2
    output reg IC1_OE, // Output enable IC1
    output reg XAEN,   // ain't no one has a clue what this does...
    input wire PI_CLK, // Clock from the Pi. Only used as a nice highspeed clock!
    input wire MCLK,   // 16.9334MHz DAC clock. Used for syncing I2S
    input wire BCLK_IN, // Bit Clock from Pi
    input wire DATA_IN, // I2S DATA from Pi - 16 bit
    input wire LRCLK_IN, // Word Clock from Pi
    output reg DATA_OUT, // I2S DATA to CDTV, 16 bit padded to 24 bit.
    output reg BCLK_OUT, // Bit Clock to CDTV. Synced to MCLK
    output reg LRCLK_OUT, // Word Clock to CDTV
    output reg MUTE, // Set when not playing audio... 
    output reg DIR_BI, // Direction for the bidirectional data bus.
    inout [7:0]  DATA_BUS, // All aboard the DATA_BUS! 
    input wire INCD, // IN/CD controlled by button/controller. Dunno what we'll use this for but we have it so...
    output reg DIR_IN, //Direction for "input" data chips
    output reg DIR_OUT, //Direction for "output" data chips
    /*
    # TBC - pin 42
    # TBC - pin 43
    # TBC - pin 44
    # TBC - pin 47
    # TBC - pin 48
    # TBC - pin 49
    # TBC - pin 50
    # TBC - pin 51
    # TBC - pin 52
    # TBC - pin 53
    # TBC - pin 54
    # TBC - pin 55
    # TBC - pin 56
    # TBC - pin 57
    */
    input wire CE0, // Chip Select for SPI
    input wire SCLK, // SPI clock
    output wire MISO, // SPI output
    input wire MOSI, // SPI input
    /*
    # TBC - pin 67
    # TBC - pin 68
    # TBC - pin 69
    # TBC - pin 70
    # TBC - pin 71
    # TBC - pin 72
    # TBC - pin 73
    # TBC - pin 74
    # TBC - pin 75
    # TBC - pin 76
    # TBC - pin 77
    */
    inout SDATA, // Serial connection from/to U62
    input wire SCK, // Serial clock from U62
    input wire RESET, // CD reset from CDTV
    input wire SCCK, // Subcode serial clock
    input wire ENABLE, // Enable drive!
    input wire HWR,    // 
    // TBC - pin 86
    output reg STCH, // STatus CHanged. 
    input wire HRD,  //
    input wire CMD, //
    output reg EFFK, 
    output reg SBCP,
    output reg SCOR,
    output reg DRQ,
    output reg DTEN, 
    output reg STEN, 
    output reg EOP,

    input wire RESET_n    // Need to wire this up to a pin... ;)
) ;

/*
* ####################################################################################################################
* #### Assigns
* ####################################################################################################################
*/

// Detect falling edge of SCK
wire sck_falling_edge = sck_prev & ~SCK;
// SDATA direction control
assign SDATA = sdata_dir ? u62_shift_reg[7] : 1'bz;

/*
* ####################################################################################################################
* #### PARAMETERS
* ####################################################################################################################
*/

/*
* Set some parameters for the clocks...
* Note that this assumes a 200MHz clock from the Pi. This will *VARY* depending on Pi Model. Everything assumes a Pi3.  
*/
   
    parameter CLOCK_FREQ = 200_000_000;  // 200 MHz. 
    parameter EFFK_FREQ = 7_350;         // 7.35 kHz
    parameter SCOR_FREQ = 75;            // 75 Hz
    parameter SCOR_PULSE_WIDTH = 27200;  // 136 us * 200 MHz
    parameter SCOR_DELAY = 980;          // 4900 ns * 200 MHz

/*
* Parameters for the front panel buttons.
*
*/

 // Commands thanks to chriskuta!
    localparam [7:0] CMD_PLAY   = 8'b10000000;
    localparam [7:0] CMD_STOP   = 8'b01100000;
    localparam [7:0] CMD_REWIND = 8'b00100000;
    localparam [7:0] CMD_FF     = 8'h01000000;
    localparam [7:0] CMD_IDLE   = 8'b00001000;



/*
* ####################################################################################################################
* #### Registers / Variables
* ####################################################################################################################
*/

/*
* Registers for I2S stuff...
*/

    reg [31:0] i2s_shift_reg; // shift register for I2S input data
    reg [4:0] i2s_bit_count; // Counter for I2S bits.
    reg lrclk_prev;          // Previous LRCLK status


/*
* Registers for the clocks
*/
    // Counter for EFFK
    reg [14:0] effk_counter = 0;
    localparam EFFK_MAX = (CLOCK_FREQ / (2 * EFFK_FREQ)) - 1;  // TBC - Reconsider this if changing drive speed. 
    
    // Counter for SCOR
    reg [20:0] scor_counter = 0;
    localparam SCOR_MAX = (CLOCK_FREQ / SCOR_FREQ) - 1; // TBC - Reconsider this if changing drive speed. 

    // SCOR pulse counter
    reg [14:0] scor_pulse_counter = 0;

    // SCOR delay counter
    reg [9:0] scor_delay_counter = 0;

    // State for SCOR
    reg scor_state = 0;


/*
* Registers for the serial bus to/from U62
*/ 
reg [7:0] FP_STATUS = 8'h00;   // Status register
reg [7:0] FP_COMMAND = 8'h00;  // Command register
// Internal signals
reg [2:0] u62_bit_count = 3'b000;
reg sdata_dir = 1'b0;       // 0 for input, 1 for output
reg [7:0] u62_shift_reg = 8'h00;
reg sck_prev = 1'b1;
reg sdata_prev = 1'b1;



/*
* ####################################################################################################################
* #### Initialisation
* ####################################################################################################################
*/


    // Initialise outputs
    initial begin
        EFFK = 0;
        SCOR = 0;
        /*
        * Enable outputs on all chips. 
        */
        IC1_OE = 0;
        IC2_OE = 0;
        IC3_OE = 0;
        IC4_OE = 0;
        IC5_OE = 0;
        /*
        * Set direction on input chips
        */
        DIR_IN = 1 ;
        /*
        * Set direction on output chips
        */
        DIR_OUT = 1;

    end


/*
* ####################################################################################################################
* #### Main body
* ####################################################################################################################
*/



/* 
*
* Do I2S conversion from 16 bit frames to 16 bit in a zero padded 24 bit frame. 
*
*/
    always @(posedge MCLK or negedge RESET_n) begin
    if (!RESET_n) begin
            i2s_shift_reg <= 32'b0;
            i2s_bit_count <= 5'b0;
            DATA_OUT <= 1'b0;
            BCLK_OUT <= 1'b0;
            LRCLK_OUT <= 1'b0;
            lrclk_prev <= 1'b0;
        end else begin
            BCLK_OUT <= BCLK_IN;
            LRCLK_OUT <= LRCLK_IN;

            if (BCLK_OUT && !BCLK_IN) begin  // Falling edge of BCLK
                if (LRCLK_IN != lrclk_prev) begin  // New word
                    i2s_bit_count <= 5'd23;  // Start from MSB of 24-bit word
                    i2s_shift_reg <= {DATA_IN, i2s_shift_reg[31:1]};  // Shift in new bit
                end else if (i2s_bit_count > 5'd7) begin  // Still receiving 16-bit data
                    i2s_shift_reg <= {DATA_IN, i2s_shift_reg[31:1]};  // Shift in new bit
                    i2s_bit_count <= i2s_bit_count - 1'b1;
                end else begin  // Outputting zero padding
                    i2s_bit_count <= i2s_bit_count - 1'b1;
                end

                // Output current bit
                DATA_OUT <= (i2s_bit_count > 5'd7) ? i2s_shift_reg[0] : 1'b0;
            end

            lrclk_prev <= LRCLK_IN;
        end
    end

/*
* Create EFFK and SCOR based off the Pi Clock. 
*/ 


 always @(posedge PI_CLK) begin
    /*
    * Deal with commands from the front panel... 
    */
    
        // EFFK generation
        if (effk_counter == EFFK_MAX) begin
            effk_counter <= 0;
            EFFK <= ~EFFK;
        end else begin
            effk_counter <= effk_counter + 1;
        end

        // SCOR generation
        if (scor_counter == SCOR_MAX) begin
            scor_counter <= 0;
            scor_state <= 1;
            scor_delay_counter <= 0;
        end else begin
            scor_counter <= scor_counter + 1;
        end

        if (scor_state == 1) begin
            if (scor_delay_counter == SCOR_DELAY - 1) begin
                SCOR <= 1;
                scor_pulse_counter <= 0;
                scor_state <= 2;
            end else begin
                scor_delay_counter <= scor_delay_counter + 1;
            end
        end else if (scor_state == 2) begin
            if (scor_pulse_counter == SCOR_PULSE_WIDTH - 1) begin
                SCOR <= 0;
                scor_state <= 0;
            end else begin
                scor_pulse_counter <= scor_pulse_counter + 1;
            end
        end
    end


endmodule
