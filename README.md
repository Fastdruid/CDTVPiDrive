# CDTVPiDrive

![image](https://github.com/user-attachments/assets/433b4078-b0bd-4e89-8090-4b137aa7cca7)

## About

This is an attempt to create a replacement drive for the CDTV that will allow either a physical CDROM or images to be used.

## Status

WIP - Re-designing the board.

## Hardware

The initial hardware has been created based largely off the original drive and then interfacing with a Raspberry Pi. The first revision has been abandoned and a new version will be produced soon. 

### Rev 0.1

#### Status
Built

#### Known Issues
1) 3-pin port for connecting to U75 is too close to the 40-pin to fit a right-angle plug.
2) Wrong value for R1 - C16M was too high a voltage.
3) Messed up the GPIO for direction control on the bi-directional data bus. Needs a track cutting and a link wire. 
4) Intended to use this for both connecting to a real drive as well as the CDTV however the bi-directional data bus direction is the "wrong way" (ignoring the previous mistake). Needs either an inverter or use another GPIO.
5) Missed one of the GPIO connections for the data bus. 

### Rev 0.2
#### Status
Abandoned - Realised this could not be both drive and CDTV emulator and it was actually pointless wasting GPIO on certain functionality. 

#### Fixes
1) Moved 3-pin connector.
2) Increased value of R1
3) Changed GPIO for direction link.

#### Changes
1) Changed jumpers to solder links.

### Rev 0.3
#### Status
Designed but abandoned. See Rev 0.4

#### Planned fixes
1) ~Remove option to be CDTV emulator.~
2) ~Change the GPIOs used to abandon de-emphasis and instead use for activity LED.~

### Rev 0.4
All change please!

After significant discussion and investigation came to the conclusion that the Subcode was ultimately _far_ too important and there was no realistic way to syncronise it from the Pi without firing it blindly into the night...
So instead this borrows even more from PiStorm and brings in a CPLD along with a flip/flop to latch the data. This then frees up a number of GPIO's that allows the Pi to communicate with the CPLD as well as external controls to change CD's etc. I'm thinking a rotary control similar to Gotek.

#### Status
In design phase. 




## Software

Software is heavily inspired by the PiStorm efforts of bit-banging the GPIO of a Raspberry Pi.
This repository contains the code I consider "largely complete" however it as yet has not been tested (although it does compile). There is another private repo which contains the WIP code, this public repo will only be occasionally updated in comparison!














