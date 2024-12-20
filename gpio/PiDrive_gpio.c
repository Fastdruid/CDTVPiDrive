// SPDX-License-Identifier: MIT

/*
  Original Copyright 2020 Claude Schwarz
  Code reorganized and rewritten by
  Niklas Ekström 2021 (https://github.com/niklasekstrom)
*/

/*
Co-Author   : David Sharp
Date        : 18/10/2024
Version     : 0.3
Name        : PiDrive_gpio.c
Description : CDROM specific functions for the CDTVPiDrive
Notes       : GPIO stuff heavily based on the original PiStorm code (ps_protocol.c). Essentially setup_gpio() is the same as PiStorm (with the header to match).
              All the rest however is my code. Sorry about that. :)
Changes     : 27/10/2024 - Fixed the read_byte and write_byte functions. Hopefully :)
            : 28/10/2024 - Added nano_delay()
            : 12/12/2024 - Added in EFFK Clock on pin 4 along with function to set it.
            : 20/12/2024 - Bug fixes...
*/

#include "PiDrive_gpio.h"
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
/*
* Start by initialising GPIO.
*/


volatile unsigned int *gpio;
volatile unsigned int *gpclk;

static void setup_gpio() {
  int fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (fd < 0) {
    printf("Unable to open /dev/mem. Run as root using sudo?\n");
    exit(-1);
  }

  void *gpio_map = mmap(
      NULL,                    // Any adddress in our space will do
      BCM2708_PERI_SIZE,       // Map length
      PROT_READ | PROT_WRITE,  // Enable reading & writing to mapped memory
      MAP_SHARED,              // Shared with other processes
      fd,                      // File to map
      BCM2708_PERI_BASE        // Offset to GPIO peripheral
  );

  close(fd);

  if (gpio_map == MAP_FAILED) {
    printf("mmap failed, errno = %d\n", errno);
    exit(-1);
  }

  gpio  = ((volatile unsigned *)gpio_map) +  GPIO_ADDR / 4;
  gpclk = ((volatile unsigned *)gpio_map) + GPCLK_ADDR / 4;
}

/*
* Setup EFFK Clock
* We'll start off with no disk - use the XOSC (19.2MHz) with a 2117 divisor to give 9.07kHz
* Later we'll change this on the fly between 9.07kHZ and 7.25kHz.
* I'm not 100% sure this is _really_ needed but its what the real drive does!
* In theory we could consider faster too if we're going multi speed however that then assumes we're doing subcode...
* I'm not sure what our limits here are going to be.
*/

static void setup_effk_clk() {
  // Enable 200MHz CLK output on GPIO4, adjust divider and pll source depending
  // on pi model
  *(gpclk + (CLK_GP0_CTL / 4)) = CLK_PASSWD | (1 << 5);
  usleep(10);
  while ((*(gpclk + (CLK_GP0_CTL / 4))) & (1 << 7))
    ;
  usleep(100);
  *(gpclk + (CLK_GP0_DIV / 4)) =
      CLK_PASSWD | (2117 << 12);  // divider , 6=200MHz on pi3
  usleep(10);
  *(gpclk + (CLK_GP0_CTL / 4)) =
      CLK_PASSWD | 1 | (1 << 4);  // 1=XOSC,  6=plld, 5=pllc
  usleep(10);
  while (((*(gpclk + (CLK_GP0_CTL / 4))) & (1 << 7)) == 0)
    ;
  usleep(100);

  SET_GPIO_ALT(EFFK, 0);  // gpclk0
}




/*
GPIO0  = /CDRST (Always Input) - LOW to reset!
GPIO1  = SCCK (Always Input)
GPIO2  = SDATA (Unsure -  Probably bidirectional - I2C?)
GPIO3  = SCK ( Unsure - Probably bidirectional - I2C?)
GPIO4  = EFFK (Always Output) - 50% duty cycle Square wave. 7.35kHz or 9.07kHz depending on if CD is spinning or not.
GPIO5  = SCOR (Always Output)
GPIO6  = *STCH (Always Output)
GPIO7  = *ENABLE (Always Input)
GPIO8  = DRQ (Always Output)
GPIO9  = *HWR (Always Input)
GPIO10 = *DTEN (Always Output)
GPIO11 = *HRD (Always Input)
GPIO12 = *STEN (Always Output)
GPIO13 = *CMD (Always Input)
GPIO14 = SBCP (Always Output)
GPIO15 = *XAEN (Always Output)
GPIO16 = DB7 (Bidirectional)
GPIO17 = DB6 (Bidirectional)
GPIO18 = BCLK (Always Output - PCM CLK)
GPIO19 = LRCLK (Always Output - PCM FS)
GPIO20 = AEMP (Always Output)
GPIO21 = DATA (Always Output - PCM OUT)
GPIO22 = DB5 (Bidirectional)
GPIO23 = DB4 (Bidirectional)
GPIO24 = DB3 (Bidirectional)
GPIO25 = DB2 (Bidirectional)
GPIO26 = DB1 (Bidirectional)
GPIO27 = DB0 (Bidirectional)


6,10,12,15


GPIO32 - > 0
00000000 00010000 11010101 00000000
00000000 10101011 00001000 00000000 - set all output high 0x00AB0800
00000000 00000000 00000000 00000000

GPIO see page 89 onwards of https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf

GPIO 9 - 0
GPFSEL0_INPUT

    9   8   7   6   5   4   3   2   1   0
00 000 001 000 001 001 100 100 100 000 000 == 0x0104C900
   In  Out In  Out Out CLK I2C I2C In  In

GPFSEL0_OUTPUT = GPFSEL0_INPUT


GPIO 19 - 10
GPFSEL1_INPUT

   19  18  17  16  15  14  13  12  11  10
00 100 100 000 000 001 001 000 001 000 001 -- 0x24009041
   PCM PCM In  In  Out Out In  Out In  Out

GPFSEL1_OUTPUT

   19  18  17  16  15  14  13  12  11  10
00 100 100 001 001 001 001 000 001 000 001 == 0x24249041
   PCM PCM Out Out Out Out In  Out In  Out


GPIO 29 - 20
GPFSEL2_INPUT

   29  28  27  26  25  24  23  22  21  20
00 000 000 000 000 000 000 000 000 100 001 == 0x00000021
   NC  NC  In  In  In  In  In  In  PCM Out

GPFSEL2_OUTPUT

   29  28  27  26  25  24  23  22  21  20
00 000 000 001 001 001 001 001 001 100 001 == 0x00249261
   NC  NC  Out Out Out Out Out Out PCM Out

gpio + 0  = GPFSEL0 GPIO Function Select 0 (GPIO0-9)
gpio + 1  = GPFSEL1 GPIO Function Select 1 (GPIO10-19)
gpio + 2  = GPFSEL2 GPIO Function Select 2 (GPIO20-29)
gpio + 7  = GPSET   GPIO Pin Output Set 0:1
gpio + 10 = GPCLR   GPIO Pin Output Clear 0 - ONLY CLEARS GPIO if set to 1.
gpio + 13 = GPLEV   GPIO Pin Levl 0 - Returns the actual value of the pin. Datasheet doesn't make sense though as 0 is both HIGH and LOW. ;)
gpio + 16 = GPEDS   GPIO Pin Event Detect Status 0
gpio + 19 = GPREN   GPIO Pin Rising Edge Detect Enable 0
gpio + 22 = GPFEN   GPIO Pin Falling Edge Detect Enable 0
gpio + 25 = GPHEN   GPIO Pin High Detect Enable 0
gpio + 28 = GPLEN   GPIO Pin Low Detect Enable 0
gpio + 31 = GPAREN  GPIO Pin Asysnchronous Rising Edge Detect Enable 0
gpio + 34 = GPAFEN  GPIO Pin Asysnchronous Falling Edge Detect Enable 0

*/

void setup_pidrive() {
  setup_gpio();
  setup_effk_clk();
//  *(gpio + 10) = TBC;
  *(gpio + 0) = GPFSEL0_INPUT;
  *(gpio + 1) = GPFSEL1_INPUT;
  *(gpio + 2) = GPFSEL2_INPUT;
  *(gpio + 7) = GPIOACT_LOW; // Set all "active low" outputs high (GPIO 6,10,12,15)


}

/*
Ok, the options we have are

1) Command write mode - ENABLE LOW, HWR LOW, HRD HIGH, CMD LOW. This instructs the LC8951 to enter command mode and read DB0-7 into the command register.
2) Read Status - ENABLE LOW, HWR HIGH, HRD LOW, CMD LOW.
3) Read data - ENABLE LOW, CMD HIGH, DTEN LOW, DATA PLACED, DRQ is set HIGH until HRD is set high.



*/

/*

void write_status(unsigned int status) {
// *(gpio + 0) = GPFSEL0_OUTPUT; // Just in case we change the pinout later!
   *(gpio + 1) = GPFSEL1_OUTPUT;
   *(gpio + 2) = GPFSEL2_OUTPUT;

TBC write status to DB0-7
SET STEN low.
*/




void setup_read() {
// *(gpio + 0) = GPFSEL0_INPUT; // Just in case we change the pinout later!
   *(gpio + 1) = GPFSEL1_INPUT;
   *(gpio + 2) = GPFSEL2_INPUT;
}
void setup_write() {
// *(gpio + 0) = GPFSEL0_OUTPUT; // Just in case we change the pinout later!
   *(gpio + 1) = GPFSEL1_OUTPUT;
   *(gpio + 2) = GPFSEL2_OUTPUT;
}

// A bunch of active low signals so to clear it we set it high
// clear and set *STCH

void clear_stch() {
   *(gpio + 7) = 1 << STCH;
}

void set_stch() {
   *(gpio + 10) = 1 << STCH;
}
// clear and set *STEN

void clear_sten() {
   *(gpio + 7) = 1 << STEN;
}

void set_sten() {
   *(gpio + 10) = 1 << STEN;
}

// clear and set *XAEN

void clear_xaen() {
   *(gpio + 7) = 1 << XAEN;
}

void set_xaen() {
   *(gpio + 10) = 1 << XAEN;
}


// clear and set *DRQ

void clear_drq() {
   *(gpio + 7) = 1 << DRQ;
}

void set_drq() {
   *(gpio + 10) = 1 << DRQ;
}


// Check if *ENABLE is LOW

unsigned int check_enable() {
   unsigned int val ;
   val = ((val ^ (1 << ENABLE)) & (1 << ENABLE)) >> ENABLE ;
   return(val);
}

// Check if *HWR is LOW

unsigned int check_hwr() {
   unsigned int val ;
   val = ((val ^ (1 << HWR)) & (1 << HWR)) >> HWR ;
   return(val);
}


// Check if *HRD is LOW

unsigned int check_hrd() {
   unsigned int val ;
   val = ((val ^ (1 << HRD)) & (1 << HRD)) >> HRD ;
   return(val);
}


// Check if *CMD is LOW

unsigned int check_cmd() {
   unsigned int val ;
   val = ((val ^ (1 << CMD)) & (1 << CMD)) >> CMD ;
   return(val);
}

unsigned int read_byte() {
   unsigned int val;
   val = *(gpio + 13); // this returns a 32 bit number for GPIO31-GPIO0. Most of which we throw away ;)
   // so now we need GPIO16,17 & 22-27. Shuffle numbers about to drop all the bits we don't care about.
   //   00001111 11000011 00000000 00000000 = 0FC30000
   //   31                                0
   val = (val & 0x0FC30000)>>16 ; // Do a bitwise AND to remove any bits we don't want then shift right to lose the first 16 bits.
   //   00001111 11000011 00000000 00000000 -> 00000000 00000000 00001111 11000011
   val = (val >>4) | (val & 0x00000003) ; // right shift and then OR it back with the remainder which has a bitwise AND.
   // 00000000 00000000 00001111 11000011 -> 00000000 00000000 00000000 11111100
   // 00000000 00000000 00000000 11111100 | 00000000 00000000 00000000 00000011 = 00000000 00000000 00000000 11111111

   return(val);
}

unsigned int write_byte(unsigned int data) {
   unsigned int databyte;
   // So more fuckaboutary. What kind of muppet stuck a gap in the GPIO?
   // 00000000 00000000 00000000 11111111 -> 00001111 11000011 00000000 00000000
   //00001111 11000000 00000000 00000000 = 0x0FC00000
   //00000000 00000011 00000000 00000000 = 0x00030000
   databyte = ((data << 20) & 0x0FC00000) | ((data << 16) & 0x00030000);
   databyte = databyte & 0x0FC30000 ; // Do a bitwise AND to remove any bits we don't want.
   *(gpio + 10) = (databyte ^ 0x0FC30000);  // clear any pins set to zero using a bitwise XOR to flip only the relevant zero's to ones, and ones to zeros..
   *(gpio + 7) = databyte; // Set the remaining pins
}


// Because we have some minimum timings we need to have some really tight sleeps...

void nano_delay(long ns)
{
    struct timespec res;
    long end;

    /* avoid out of range values */
    if( ns<1 || ns>MAX_N )
        return;

    /* obtain the current value */
    clock_gettime(CLOCK_REALTIME,&res);
    /* calculate end time */
    end = res.tv_nsec + ns;
    /* wait for the end time */
    while(1)
    {
        clock_gettime(CLOCK_REALTIME,&res);
        if( res.tv_nsec > end )
            break;
    }
}
