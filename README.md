# CDTVPiDrive

![image](https://github.com/user-attachments/assets/2d1f0f2e-6e39-4105-9603-3c31e4def2e0)


## About

This is an attempt to create a replacement drive for the CDTV that will allow either a physical CDROM (via USB) or images to be used.

## Status

WIP - Functionally complete on dev hardware. Requires integration and automation as functionality is currently very "manual".

## Hardware

On what is hopefully the final release of the hardware!

### Rev 0.9c

#### Known Issues
None yet!

#### Status
Ordered

## Software

Software was heavily inspired by the PiStorm efforts of bit-banging the GPIO of a Raspberry Pi but things moved on. Currently uses SPI to pass the "commands" from the CDTV via the CPLD to the Pi, with the CPLD handling the complexities of the drive functionality. The Pi then passes Data to the CPLD across an 8-bit bus and the CPLD then sends it at the appropriate time with all the required signals to the CDTV. 

All the code in this repo is massively out of date. 
There is another private repo which contains the current WIP code, this public repo will only be occasionally updated in comparison!














