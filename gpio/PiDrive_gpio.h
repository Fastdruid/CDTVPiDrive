// SPDX-License-Identifier: MIT

/*
Author      : David Sharp
Date        : 24/09/2024
Version     : 0.4
Name        : PiDrive_gpio.h
Description : Header file for the GPIO functionality of the CDTVPiDrive
Notes       : Inspiration for as well as a chunk of this and the functionality taken from PiStorm. Thanks and attribution to the writers.
Changes     : 25/10/2024 - Wrong register values for GPFSEL[0-2]_INPUT/OUTPUT - fixed
            : 28/10/2024 - MAX_N for nano_delay()
            : 12/12/2024 - Changed GPFSEL0_* to change pin 4 from Output to Clock. Added CLK Passwd and Divider constants.
            : 20/12/2024 - Copied in the GPIO Setup macros SET_GPIO_ALT.
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
#define GPCLK_ADDR 0x101000 /* Clock */


// #define GPCLK_BASE (BCM2708_PERI_BASE + 0x101000) /* Clock */
// #define GPIO_BASE (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#define GPFSEL0_INPUT 0x0104C900
#define GPFSEL1_INPUT 0x24009041
#define GPFSEL2_INPUT 0x00000021

#define GPFSEL0_OUTPUT 0x0104C900
#define GPFSEL1_OUTPUT 0x24249041
#define GPFSEL2_OUTPUT 0x00249261



#define CLK_PASSWD 0x5a000000
#define CLK_GP0_CTL 0x070
#define CLK_GP0_DIV 0x074


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or
// SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio + ((g) / 10)) &= ~(7 << (((g) % 10) * 3))
#define OUT_GPIO(g) *(gpio + ((g) / 10)) |= (1 << (((g) % 10) * 3))
#define SET_GPIO_ALT(g, a)  \
  *(gpio + (((g) / 10))) |= \
      (((a) <= 3 ? (a) + 4 : (a) == 4 ? 3 : 2) << (((g) % 10) * 3))



#define GPIOACT_LOW     0x00AB0800 // GPIO 6,10,12,15

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
// Subcode shift clock
#define SCCK 1
// SPI data from U62 / LC6554. I think this is bidirectional
#define SDATA 2
// SPI clock. Generated at U62 / LC6554
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

#define MAX_N 999999999L
