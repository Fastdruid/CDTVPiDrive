// SPDX-License-Identifier: MIT

/*
Author      : David Sharp
Date        : 24/09/2024
Version     : 0.1
Name        : PiDrive_gpio.h
Description : Header file for the GPIO functionality of the CDTVPiDrive
Notes       : Inspiration for as well as a chunk of this and the functionality taken from PiStorm. Thanks and attribution to the writers. 
*/

/* Physical addresses range from 0x20000000 to 0x20FFFFFF for peripherals.
* The bus addresses for peripherals are set up to map onto the peripheral bus address range starting at 0x7E000000.
* Thus a peripheral advertised in the manual at bus address 0x7Ennnnnn is available at physical address 0x20nnnnnn (for Pi0-1).
*      a peripheral advertised in the manual at bus address 0x7Ennnnnn is available at physical address 0x3Fnnnnnn (for Pi2-3).
*      a peripheral advertised in the manual at bus address 0x7Ennnnnn is available at physical address 0xFEnnnnnn (for Pi4).
* We don't talk about the Pi5 :(
*/

// We really don't need more than a Pi3. The Pi3 has faster gpio than the Pi4 as well. If however you desire to run a Pi4 then change this here.


//#define BCM2708_PERI_BASE 0x20000000  // Pi0-1
//#define BCM2708_PERI_BASE 0xFE000000  // Pi4
//#define BCM2708_PERI_SIZE 0x01800000  //Pi4
#define BCM2708_PERI_BASE 0x3F000000  // Pi2-3
#define BCM2708_PERI_SIZE 0x01000000  // Pi0-3

#define GPIO_ADDR 0x200000 /* GPIO controller */
#define GPCLK_ADDR 0x101000

#define GPFSEL0_INPUT 0x08200249
#define GPFSEL1_INPUT 0x00000208
#define GPFSEL2_INPUT 0x00000000

#define GPFSEL0_OUTPUT 0x08200249
#define GPFSEL1_OUTPUT 0x00249208
#define GPFSEL2_OUTPUT 0x00249240

#define GPIOACT_LOW 

#define GPFSEL_OUTPUT \
  *(gpio + 0) = GPFSEL0_OUTPUT; \
  *(gpio + 1) = GPFSEL1_OUTPUT; \
  *(gpio + 2) = GPFSEL2_OUTPUT;

#define GPFSEL_INPUT \
  *(gpio + 0) = GPFSEL0_INPUT; \
  *(gpio + 1) = GPFSEL1_INPUT; \
  *(gpio + 2) = GPFSEL2_INPUT;


// All the CD Pins (numbers are the GPIO)

// Reset
#define CDRST 0
// Not used here... 
#define SCCK 1
// DATA Direction. Sets output or input. High is input. Low is output.
#define DATAD 2
// Subcode shift clock
#define SCK 3
// Subcode EFM Frame Clock
#define EFFK 4
// Subcode sync 
#define SCOR 5
// Unsure here what this does... maybe CD-Status?
#define STCH 6
// Enable drive
#define ENABLE 7
// WAIT! :) - Data ReQuest 
#define DRQ 8
// Host Data Write input
#define HWR 9
// DaTa ENable 
#define DTEN 10
// Host Data Read input
#define HRD 11
// STatus ENable
#define STEN 12
//Host Command/Data select
#define CMD 13
// Subcode P-W serial data output
#define SBCP 14
// Unsure. Either eXternal Audio ENable or related to XTA AEN. 
#define XAEN 15
//Bidirectional Data bit 7
#define DB7 16
//Bidirectional Data bit 6
#define DB6 17
// Audio Bit CLocK
#define BCLK 18
// Audio Left / Right CLocK
#define LRCLK 19
// Audio de-EMPhasis enable
#define AEMP 20
// Audio I2S Data
#define DATA 21
//Bidirectional Data bit 5
#define DB5 22
//Bidirectional Data bit 4
#define DB4 23
//Bidirectional Data bit 3
#define DB3 24
//Bidirectional Data bit 2
#define DB2 25
//Bidirectional Data bit 1
#define DB1 26
//Bidirectional Data bit 0
#define DB0 27

