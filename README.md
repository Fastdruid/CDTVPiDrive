# CDTVPiDrive

![image](https://github.com/user-attachments/assets/372723c2-746b-4247-a57c-dc5766e81cb8)

## About

This is an attempt to create a replacement drive for the CDTV that will allow either a physical CDROM (via USB) or images to be used.

## Status

WIP - Re-designing the board.

## Hardware

The initial hardware has been created based largely off the original drive and then interfacing with a Raspberry Pi. The first revision has been abandoned apart from for testing and a new version will be produced soon. 

### Rev 0.1

#### Known Issues
1) 3-pin port for connecting to U75 is too close to the 40-pin to fit a right-angle plug.
2) Wrong value for R1 - C16M was too high a voltage.
3) Messed up the GPIO for direction control on the bi-directional data bus. Needs a track cutting and a link wire. 
4) Intended to use this for both connecting to a real drive as well as the CDTV however the bi-directional data bus direction is the "wrong way" (ignoring the previous mistake). Needs either an inverter or use another GPIO.
5) Missed one of the GPIO connections for the data bus.
6) The three wire connector from U62 was presumed to be I2C. Its not. 
7) I2S doesn't work from the Pi to the CDTV as the Pi outputs either 16 or 32 bit frames while the CDTV requires 16 bit I2S in a 24 bit frame.
8) The Subcode requires syncing to an external clock. This is frankly going to be next to impossible while bit-banging. Figured could do this blind but easier from a CPLD. 

#### Status
Built
   
### Rev 0.2

#### Fixes
1) Moved 3-pin connector.
2) Increased value of R1
3) Changed GPIO for direction link.

#### Changes
This was mostly just fixing issues. 

1) Changed jumpers to solder links.

#### Status
Abandoned - Realised this could not be both drive and CDTV emulator and it was actually pointless wasting GPIO on certain functionality. 


### Rev 0.3

#### Planned fixes
1) ~Remove option to be CDTV emulator.~
2) ~Change the GPIOs used to abandon de-emphasis and instead use for activity LED.~

#### Status
Designed but abandoned. See Rev 0.4

### Rev 0.4

All change please!

After significant discussion and investigation came to the conclusion that the Subcode was ultimately _far_ too important and there was no realistic way to syncronise it from the Pi without firing it blindly into the night...
So instead this borrows even more from PiStorm and brings in a CPLD along with a flip/flop to latch the data. This then frees up a number of GPIO's that allows the Pi to communicate with the CPLD as well as external controls to change CD's etc. I'm thinking a rotary control similar to Gotek.

#### Planned fixes/Changes
1) Add CPLD
2) Add Latch
3) Add second GPIO header purely for development programming the CPLD (because I'm intending on using all the pins!)

#### Status
Designed but abandoned. 

### Rev 0.5

Even more change. Dropped the flip-flop, instead the Pi will communicate with the CPLD over SPI and the CPLD will shove the data onto the bus. No more bit banging. This frees up a lot of pins so remove the second header and program the CPLD straight from the Pi.
Discovered that the CDTV does 16 bit I2S encapsulated in 24 bit frames... I get silence from I2S via CDTV (using an external DAC works great). 
This isn't something the Pi Supports. So the I2S now goes via the CPLD. This will sync BCLK with MCLK (16.9334MHz clock) as well as zero pad the 16 bit I2S from the Pi out to 24 bit frames. 

#### Planned fixes/Changes
1) Remove latch again...
2) Remove second GPIO header
3) Re-route I2S via CPLD.
4) Add another level-shifter.

#### Status
Partly designed but abandoned. 

### Rev 0.6A

Not massively functionally different to rev 0.5 however changed to fit the video card slot. It's not _required_ to use the slot, a power header is also included but the video slot is a convenient place for it.. 

#### Known Issues
1) Missed two grounds off.
2) Without a backplate it is almost impossible to remove from the CDTV!

#### Changes
1) Change to Video Card Slot format.
2) Route IN/CD via level-shifter into CPLD.
3) Add individual OE control to each of the level-shifters. 
4) Used EPM570 rather than EPM240 due to being unsure how many LUT's I'd use!

#### Status
Built

### Rev 0.6B

Minor revision merely to fix missing grounds.

#### Planned fixes/Changes
1) Fix grounds.
   
#### Status
Designed. 



## Software

~Software is heavily inspired by the PiStorm efforts of bit-banging the GPIO of a Raspberry Pi.~

Things have moved on. No longer bit-banging but rather using SPI from the Pi and handling the complexities of the drive functionality via the CPLD. The only PiStorm-like functionality retained is generating a clock from the Pi for the CPLD and programming the CPLD in an identical way. 

Currently this means all the code in this repo is massively out of date. 
This repository contains the code I consider "largely complete" however it as yet has not been tested (although it does compile). There is another private repo which contains the WIP code, this public repo will only be occasionally updated in comparison!














