/*
 * Copyright 2025 David Sharp
*/

// Revision History
/* v0.0.1 : 2025-02-01 : BRAND NEW! Its so *SHINY*! Its got all that "new code" smell and everything.
*                      : Initial version creates EFFK & SCOR clocks as well as repads the I2S to 24 bit frames. 
*  v0.0.2 : 2025-02-01 : Initial version didn't correctly create the SCOR clock. Changed the logic and added a frame counter as this can be used for other things too.
*                        Added (parameter) delay for SCOR, need to measure this accurately to configure it.
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
    input wire CE0, // Chip Select for SPI - Active LOW!
    input wire SCLK, // SPI clock
     // TBC - pin 64
    output wire MISO, // Master In Slave Out
    input wire MOSI, // Master Out Slave In
    /*
  
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

// Tristate control for SDATA
    assign SDATA = sdata_oe ? sdata_out : 1'bz;
/*
* ####################################################################################################################
* #### PARAMETERS
* ####################################################################################################################
*/

// Define HI and LO
    localparam LO = 1'b0;
    localparam HI = 1'b1;

/*
* Set some parameters for the clocks...
* Note that this assumes a 200MHz clock from the Pi. This will *VARY* depending on Pi Model. Everything assumes a Pi3.  
*/
   
    parameter CLOCK_FREQ = 200_000_000;  // 200 MHz. 
    parameter EFFK_FREQ = 7_350;         // 7.35 kHz
    parameter SCOR_FREQ = 98;            // Every 98 frames....
    parameter CLOCK_CYCLE_TIME = 5 ;      // 5ns at 200MHz...
    parameter SCOR_DELAY = 2000;          // Assuming a 10 μs delay and a clock cycle time of 5ns. 
/*
* Parameters for the front panel buttons.
*
*/

 // Commands thanks to chriskuta!
    localparam [7:0] FP_CMD_PLAY   = 8'b10000000;
    localparam [7:0] FP_CMD_STOP   = 8'b01100000;
    localparam [7:0] FP_CMD_REWIND = 8'b00100000;
    localparam [7:0] FP_CMD_FF     = 8'h01000000;
    localparam [7:0] FP_CMD_IDLE   = 8'b00001000;

 // State machine states for SPI comms
    localparam IDLE = 2'b00;
    localparam RECEIVE = 2'b01;
    localparam TRANSMIT = 2'b10;


/*
* Paramaters for the MKE command set
*
*/ 
    localparam CMD_SEEK    = 8'h01;
    localparam CMD_READ    = 8'h02;
    localparam CMD_MOTORON = 8'h04;
    localparam CMD_MOTOROF = 8'h05;
    localparam CMD_PLAYLSN = 8'h09;
    localparam CMD_PLAYMSF = 8'h0a;
    localparam CMD_PLAYTRK = 8'h0b;
    localparam CMD_STATUS  = 8'h81;
    localparam CMD_CLRERR  = 8'h82;
    localparam CMD_MODEL   = 8'h83;
    localparam CMD_SETMODE = 8'h84;
    localparam CMD_SUBQ    = 8'h87;
    localparam CMD_INFO    = 8'h89;
    localparam CMD_READTOC = 8'h8a;
    localparam CMD_PAUSE   = 8'h8b;
    localparam CMD_PANEL   = 8'ha3;
     
/*
* Bonus commands... 
* We'll put some cool stuff here so we can control things from the Amiga. :)
*   localparam CMD_TBC   = 8'hxx;
*
*/


/*
* ####################################################################################################################
* #### Registers / Variables
* ####################################################################################################################
*/

  reg [7:0] cd_status;  // Error status. 
  /* Consists of these bits
  
  cd_isready = 0
  cd_locked = 0
  cd_playing = 0
  cd_finished = 0
  cd_error = 0
  cd_motor = 0
  cd_media = 0
  cd_closed = 0

  */



/*
* Registers for SPI
*/

    reg [7:0] spi_data_out;         // Data to send to the master (internal register)
    reg [7:0] spi_data_in;          // Data received from the master 
    reg [1:0] state, next_state;
    reg [2:0] spi_bit_count;        // 3 bits to count 8 bits
    reg [7:0] spi_shift_reg;        // Shift register for receiving data


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
    reg [7:0] scor_counter = 0;

    // Frame Counter - Goes up to 98
    reg [7:0] frame_counter = 0;
     
    // SCOR pulse counter
    reg [14:0] scor_pulse_counter = 0;

    // SCOR delay counter
    reg [10:0] scor_delay_counter = 0;

    // State for SCOR
    reg scor_state = 0;


/*
* Registers for the serial bus to/from U62
*/ 
reg [7:0] FP_STATUS = 8'h00;   // Status register
reg [7:0] FP_COMMAND = 8'h00;  // Command register
reg fp_disabled = 1'b0;       // Flag to disable command processing
reg [1:0] u62_state = IDLE;
reg [2:0] u62_bit_counter = 3'b000;
reg [7:0] u62_shift_register = 8'b0;
reg sck_prev = 1'b1;
reg sdata_out = 1'b0;
reg sdata_oe = 1'b0;  // Output enable for SDATA

/*
* ####################################################################################################################
* #### Tasks
* ####################################################################################################################
*/

   
/*
* ####################################################################################################################
* #### Functions
* ####################################################################################################################
*/


/*
* ####################################################################################################################
* #### Initialisation
* ####################################################################################################################
*/


    // Initialise outputs
    initial begin
        EFFK = 1'b0;
        SCOR = 1'b0;
        STEN = 1'b1;
        DTEN = 1'b1;
        DRQ = 1'b0;
        EOP = 1'b1;
        cd_status = 8'b00000000; // All status bits LOW on init
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
        DIR_OUT = 0;



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
    // EFFK generation
    if (effk_counter == EFFK_MAX) begin
        effk_counter <= 0;
        EFFK <= ~EFFK;
        // We're going to sync SCOR to EFFK every 98 frames. We start on a LOW signal because that's what has been seen on the 'scope.
        
        if (EFFK == HI) begin
            if (frame_counter == 0) begin
                scor_state <= HI;
                frame_counter <= frame_counter + 1;
            end
            else if (frame_counter > 0 && frame_counter < 97) begin
                scor_state <= LO;
                frame_counter <= frame_counter + 1;
            end
            else if (frame_counter == 97) begin
                frame_counter <= 0;
            end
        end
    end
    else begin
        if (effk_counter > SCOR_DELAY) begin
           case (scor_state)
              LO: begin
                SCOR <= LO;
              end
              HI: begin
                SCOR <= HI;
              end
            endcase
        end  

        effk_counter <= effk_counter + 1;
    end
end



/*
* Deal with the front panel commands and status. 
*/


    always @(posedge PI_CLK) begin
        sck_prev <= SCK;

        case (u62_state)
            IDLE: begin
                if (sck_falling_edge) begin
                    if (SDATA) begin
                        u62_state <= TRANSMIT;
                        sdata_oe <= 1'b1;
                        u62_shift_register <= FP_STATUS;
                    end else begin
                        u62_state <= RECEIVE;
                    end
                    u62_bit_counter <= 0;
                end
            end

            RECEIVE: begin
                if (sck_falling_edge) begin
                    u62_shift_register <= {u62_shift_register[6:0], SDATA};
                    u62_bit_counter <= u62_bit_counter + 1;
                    if (u62_bit_counter == 3'b111) begin
                        u62_state <= IDLE;
                        if (!fp_disabled) begin
                            case ({u62_shift_register[6:0], SDATA})
                                FP_CMD_PLAY:   FP_COMMAND <= FP_CMD_PLAY;
                                FP_CMD_STOP:   FP_COMMAND <= FP_CMD_STOP;
                                FP_CMD_REWIND: FP_COMMAND <= FP_CMD_REWIND;
                                FP_CMD_FF:     FP_COMMAND <= FP_CMD_FF;
                                default:       FP_COMMAND <= 8'h00; // Invalid command
                            endcase
                        end
                    end
                end
            end

            TRANSMIT: begin
                if (sck_falling_edge) begin
                    sdata_out <= u62_shift_register[7];
                    u62_shift_register <= {u62_shift_register[6:0], 1'b0};
                    u62_bit_counter <= u62_bit_counter + 1;
                    if (u62_bit_counter == 3'b111) begin
                        u62_state <= IDLE;
                        sdata_oe <= 1'b0;
                    end
                end
            end
        endcase
    end



always @(posedge PI_CLK) begin
        case (FP_COMMAND)
            FP_CMD_PLAY: begin
                // Add play functionality here
            end
            FP_CMD_STOP: begin
                // Add stop functionality here
            end
            FP_CMD_REWIND: begin
                // Add rewind functionality here
            end
            FP_CMD_FF: begin
                // Add fast forward functionality here
            end
            default: begin
                // Handle invalid command or do nothing
            end
        endcase
    end


endmodule

