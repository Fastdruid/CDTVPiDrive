# CDTVPiDrive

![image](https://github.com/user-attachments/assets/433b4078-b0bd-4e89-8090-4b137aa7cca7)

## About

This is an attempt to create a replacement drive for the CDTV that will allow either a physical CDROM or images to be used.

## Status

WIP - Writing the code to make it work. 

## Hardware

The initial hardware has been created based largely off the original drive and then interfacing with a Raspberry Pi.

### Rev 0.1

#### Status
Built

#### Known Issues
1) 3-pin port for connecting to U75 is too close to the 40-pin to fit a right-angle plug.
2) Wrong value for R1 - C16M was too high a voltage.
3) Messed up the GPIO for direction control on the bi-directional data bus. Needs a track cutting and a link wire. 
4) Intended to use this for both connecting to a real drive as well as the CDTV however the bi-directional data bus direction is the "wrong way" (ignoring the previous mistake). Needs either an inverter or use another GPIO. 

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
WIP

#### Planned fixes

1) Remove option to be CDTV emulator.
2) Change the GPIOs used to abandon de-emphasis and instead use for activity LED.

## Software

Software is heavily inspired by the PiStorm efforts of bit-banging the GPIO of a Raspberry Pi.
This repository contains the code I consider "largely complete" however it as yet has not been tested. There is another private repo which contains the WIP code, this public repo will only be occasionally updated in comparison!














