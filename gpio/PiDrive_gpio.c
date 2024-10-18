// SPDX-License-Identifier: MIT

/*
  Original Copyright 2020 Claude Schwarz
  Code reorganized and rewritten by
  Niklas Ekström 2021 (https://github.com/niklasekstrom)
*/

#include "PiDrivei_gpio.h"
/*

... Just in case we want to try with a really real CDROM Drive. 
#include <linux/cdrom.h>
*/




static void setup_gpio() {
  int fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (fd < 0) {
    printf("Unable to open /dev/mem. Run as root using sudo?\n");
    exit(-1);
  }

  void *gpio_map = mmap(
      NULL,                    // Any adddress in our space will do
      BCM2708_PERI_SIZE,       // Map length
      PROT_READ | PROT_WRITE,  // Enable reading & writting to mapped memory
      MAP_SHARED,              // Shared with other processes
      fd,                      // File to map
      BCM2708_PERI_BASE        // Offset to GPIO peripheral
  );

  close(fd);

  if (gpio_map == MAP_FAILED) {
    printf("mmap failed, errno = %d\n", errno);
    exit(-1);
  }

  gpio = ((volatile unsigned *)gpio_map) + GPIO_ADDR / 4;
}

/*
GPIO0  = CDRST (Always Input)
GPIO1  = SCCK (Always Input)
GPIO2  = SDATA (Unsure -  Probably bidirectional - I2C?)
GPIO3  = SCK ( Unsure - Probably bidirectional - I2C?)
GPIO4  = EFFK (Always Output)
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

GPIO32 - > 0
00000000 00010000 11010101 00000000
00000000 10101011 00001000 00000000 - set all output high 0x00AB0800
00000000 00000000 00000000 00000000

GPIO see page 89 onwards of https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf

GPIO 9 - 0
GPFSEL0_INPUT

    9   8   7   6   5   4   3   2   1   0
00 000 001 000 001 001 001 100 100 000 000
   In  Out In  Out Out Out I2C I2C In  In

GPFSEL0_OUTPUT = GPFSEL0_INPUT


GPIO 19 - 10
GPFSEL1_INPUT

   19  18  17  16  15  14  13  12  11  10
00 100 100 000 000 001 001 000 001 000 001
   PCM PCM In  In  Out Out In  Out In  Out

GPFSEL1_OUTPUT 

   19  18  17  16  15  14  13  12  11  10
00 100 100 001 001 001 001 000 001 000 001
   PCM PCM Out Out Out Out In  Out In  Out


GPIO 29 - 20
GPFSEL2_INPUT

   29  28  27  26  25  24  23  22  21  20
00 000 000 000 000 000 000 000 000 100 001
   NC  NC  In  In  In  In  In  In  PCM Out

GPFSEL2_OUTPUT 

   29  28  27  26  25  24  23  22  21  20
00 000 000 001 001 001 001 001 001 100 001
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
  setup_io();

//  *(gpio + 10) = TBC;
  *(gpio + 0) = GPFSEL0_INPUT;
  *(gpio + 1) = GPFSEL1_INPUT;
  *(gpio + 2) = GPFSEL2_INPUT;
  *(gpio + 7) = 0x00AB0800; // Set all "active low" outputs high. 

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
   *(gpio + 2) = GPFSEL2_OUTOUT;

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
   *(gpio + 2) = GPFSEL2_OUTOUT;
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
   int val ;
   val = ((val ^ (1 << ENABLE)) & (1 << ENABLE)) >> ENABLE ;
   return(val);
}

// Check if *HWR is LOW

unsigned int check_hwr() {
   int val ;
   val = ((val ^ (1 << HWR)) & (1 << HWR)) >> HWR ;
   return(val);
}


// Check if *HRD is LOW

unsigned int check_hrd() {
   int val ;
   val = ((val ^ (1 << HRD)) & (1 << HRD)) >> HRD ;
   return(val);
}
  

// Check if *CMD is LOW

unsigned int check_cmd() {
   int val ;
   val = ((val ^ (1 << CMD)) & (1 << CMD)) >> CMD ;
   return(val);
}

unsigned int read_byte() {
   int val;
   val = *(gpio + 13); // this returns a 32 bit number for GPIO31-GPIO0. Most of which we throw away ;) 
   // so now we need GPIO16,17 & 22-27. Shuffle numbers about to drop all the bits we don't care about. 
   //   00001111 11000011 00000000 00000000 = 0FC30000
   //   31                                0
   val = (val & 0x0000C3F0)>>16 ; // Do a bitwise AND to remove any bits we don't want then shift right to lose the first 16 bits.
   val = (val >>4) | (val & 0x00000003) ; // right shift and then OR it back with the remainder which has a bitwise AND.
   return(val);  
}

unsigned int write_byte(data) {
   int data = data & 0x0000C3F0 ; // Do a bitwise AND to remove any bits we don't want. 
   *(gpio + 10) = (data ^ 0x0000C3F0)  // clear any pins set to zero using a bitwise XOR to flip only the relevant zero's to ones, and ones to zeros.. 
   *(gpio + 7) = data; // Set the remaining pins
}











 
