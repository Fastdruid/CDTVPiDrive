# This file documents both the original and new commands #

As any arbitary command can be run from the Amiga and the program on the Pi can do anything you can do with a Pi running Linux the only limits are our imaginations!  
The intention is to allow anything "different" to run a script or program on the Pi. To avoid multiple people doing different stuff I want to keep this as being the single source of new commands. If anyone wants a new command for any reason raise an issue and I'll add a "command" in here for it (along with a link if you desire to a repo etc). 
# Notes #

All commands except one are 7 bytes in length. The one exception being 0x81 which is a single byte command. The first byte being the command itself followed by 6 bytes that are the options to that command.  

Some of this has been gathered from a Logic Analyser on the drive, some from WinUAE, some from C4ptFuture and his work on reverse engineering CDTV OS. Thanks go to everyone who has contributed, named or otherwise. 

## Original ##
### 0x01 - Seek ###
#### Description ####
As a CD-ROM is relatively slow and can taking a second or so to get from one location to another on a drive this command exists to move the head before reading. 
#### Options ####
#### Returns ####
Unsure. Not seen this yet.

### 0x02 - Read ###
#### Description ####
This command reads data from the drive. 
#### Options ####
#### Returns ####
Data. 

### 0x04 - Motor On ###
#### Description ####
This command starts the motor
#### Options ####
#### Returns ####
Nothing

### 0x05 - Motor Off ###
#### Description ####**
This command stops the motor
#### Options ####
#### Returns ####
Nothing

### 0x09 - Play Audio (LSN) ###
#### Description ####
Play Audio using LSN addressing.
#### Options ####
#### Returns ####
Nothing

### 0x0A - Play Audio (MSF) ###
#### Description ####
Play Audio using MSF addressing
#### Options ####
#### Returns ####
Nothing

### 0x0B - Play Audio (track) ###
#### Description ####
Play Audio using track/index
#### Options ####
#### Returns ####

### 0x81 - Status ###
#### Description ####
Return the status byte. Note that this is the only (known) single byte command.
#### Options ####
None
#### Returns ####
Single byte of status

### 0x82 - Read/Clear Errors ###
#### Description ####
Reads any errors and resets the error bit.
#### Options ####
#### Returns ####
6 bytes of Error code

### 0x83 - Model Name ###
#### Description ####
Returns the model name.
#### Options ####
#### Returns ####
Model name. Unsure if this has a fixed size

### 0x84 - Set Sector size ###
#### Description ####
Sets the sector size. 
#### Options ####
#### Returns ####
Nothing

### 0x87 - Subcode Q enquiry ###
#### Description ####
Basically returns where on the CD the head is, what the drive is doing and what type of track its reading.
#### Options ####
#### Returns ####
13 bytes

### 0x89 - Volume Summary ###
#### Description ####
Returns a short summary of the CD, first track, last track and how long the CD is.
#### Options ####
#### Returns ####
5 bytes

### 0x8A - Track Summary ###
#### Description ####
Returns type of track, track number and sector position (either LSN or MSF depending on options).
#### Options ####
#### Returns ####
8 Bytes

### 0x8B - Pause ###
#### Description ####
Pause or unpause audio
#### Options ####
#### Returns ####
Nothing

### 0xA3 - Disable FP ###
#### Description ####
Disables or enables the front panel controls. 
#### Options ####
#### Returns ####
Nothing

## New ##
### 0xA2 - FP Command ###
#### Description ####
Due to the way the PiDrive works the CPLD sends the front panel commands to the Pi the same way as the existing commands, these commands therefore have been slotted into the same sequence. 
#### Options ####
##### Byte 1 - Button code #####
0x08 (00001000) which represents TRACK  
0x60 (01100000) which represents STOP   
0x80 (10000000) which represents PLAY   
0x20 (00100000) which represents REW    
0x40 (01000000) which represents FF     
##### Byte 2-6 #####
N/A
#### Returns ####
N/A

