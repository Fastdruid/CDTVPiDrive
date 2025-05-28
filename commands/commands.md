# This file documents both the original and new commands #

As any arbitary command can be run from the Amiga and the program on the Pi can do anything you can do with a Pi running Linux the only limits are our imaginations!  
The intention is to allow anything "different" to run a script or program on the Pi. To avoid multiple people doing different stuff I want to keep this as being the single source of new commands. If anyone wants a new command for any reason raise an issue and I'll add a "command" in here for it (along with a link if you desire to a repo etc). 
# Notes #

All commands except one are 7 bytes in length. The one exception being 0x81 which is a single byte command. The first byte being the command itself followed by 6 bytes that are the options to that command.  

Some of this has been gathered from a Logic Analyser on the drive, some from WinUAE, some from C4ptFuture and his work on reverse engineering CDTV OS. Thanks go to everyone who has contributed, named or otherwise. 

## Original ##

Where MSF/LSN is given MSF = 0x02, LSN = 0x00

| Command Name | Command Byte | Byte 1 | Byte 2 | Byte 3 | Byte 4 | Byte 5 | Byte 6 | Returns | Description |
| --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- |
| Checkpath | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 2 Bytes | There are very few details on this but I believe this is a diagnostic command that checks the data path. |
| Seek | 0x01 | Sector | Sector | Sector | 0x00 | 0x00 | 0x00 | Nothing | As a CD-ROM is relatively slow and can taking a second or so to get from one location to another on a drive this command exists to move the head before reading. |
| Read | 0x02 | Sector | Sector | Sector | Length | Length | 0x00 | Data :) | This command reads data from the drive, it sends a 24 bit number reflecting the start sector and a 16 bit number reflecting the number of sectors to return |
| Header | 0x03 |  UNK | UNK | UNK | UNK | UNK | UNK | Unknown | Unknown |
| Motor On | 0x04 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Turns the motor on |
| Motor Off | 0x05| 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Turns the motor off |
| Diagnostic | 0x06 | 0x80 - Seek test, 0x81 - Long Seek test | UNK | UNK | UNK | UNK | UNK | Nothing - Check for errors after | Performs Diagnostics. Only test known is the seek test. |
| UPC | 0x07 | UNK | UNK | UNK | UNK | UNK | UNK | Unknown | Unknown |
| ISRC | 0x08 | UNK | UNK | UNK | UNK | UNK | UNK | Unknown | Unknown |
| Play (LSN) | 0x09 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing - Plays audio | Plays Audio by using LSN references |
| Play (MSF) | 0x0a | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing - Plays audio | Plays Audio by using MSF references |
| Play (track) | 0x0b | start track | start index | end track | end index | 0x00 | 0x00 | Nothing - Plays audio | Plays Audio by track |
| Play XA | 0x0c | UNK | UNK | UNK | UNK | UNK | UNK | Unknown | Unknown |
| Checkpath | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 2 Bytes | There are very few details on this but I believe this is a diagnostic command that checks the data path. |
| Status | 0x81 | | | | | | | 1 byte | The only single byte command! Requests a status byte from the drive. |
| Read / Reset Error | 0x82 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 6 bytes | Reads and then resets the error status. |
| Model name | 0x83 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 12 bytes | Returns Model and version, MATSHITA0.96 or MATSHITA0.97 on an original drive. |
| Select Mode | 0x84 | ?? | sector size | sector size | 0x00 | ?? | 0x00 | Nothing | Sets the sector size, valid sizes are 256, 512, 1024, 2048, 2052, 2336, 2340.|
| Modesense | 0x85 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 2 bytes | Returns the current mode size set. |
| Capacity | 0x86 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 5 bytes | Returns the disc size, first three bytes are the number of sectors, next two are the sector size. |
| SubQ | 0x87 | MSF/LSN | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 13 bytes | Returns audio playback status, control data and current LSN or MSF position |
| Diskcode | 0x88 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 14 bytes | Unknown, all the discs I have tested return 14 zeros |
| Diskinfo | 0x89 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 5 bytes | Returns a disc summary, returns first track, last track, m,s,f of the lead out |
| Read TOC | 0x8a | MSF/LSN | Track | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 8 bytes | Returns Q channel info, Track number and position of the track in the requested format (MSF/LSN) |
| Pause | 0x8b | Pause (0x80) / Unpause (0x00) |  0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Pauses / Unpauses audio. Bug in the original hardware which means it does not pause audio started from the front panel! |
| Packet | 0x8C | Number of bytes returned, 0x00 = 256 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | up to 256 bytes | Unknown what this is | 
| XD Params | 0xa2 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 4 bytes | Unknown, all the discs I have tested return 4 zeros |
| Front Panel | 0xa3 | Enable (0x20) / Disable (0x00) | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Enables or Disables the front panel controls. Note that despite the documentation the front panel buttons send keycodes at all times!| 

### 0x00 - Checkpath ###
Two bytes. Unsure what they represent but 0xAA 0x55 indicates everything is good. 
### 0x03 - Header ###
With options set to 0x00 it does not return any data.
### 0x0C - Play XA ###
Nothing seems to be returned. 

### 0x80 - Checkpath ###
Two bytes. Unsure what they represent but 0xAA 0x55 indicates everything is good. 

### 0x81 - Status ###
Single byte of status

### 0x82 - Read/Clear Errors ###
6 bytes of Error code

### 0x83 - Model Name ###
Model name. Unsure if this has a fixed size

### 0x85 - Mode sense ###
Two bytes which match the sector size. Unsure if this is what is set or what is detected. 

### 0x86 - Capacity ###
Returns 5 bytes. The first three bytes are the number of data sectors and the last two bytes are the sector size. So as an example a disc with 25,546,752 bytes on and 2048 byte sectors would report as "0x00 0x30 0xBA 0x08 0x00"

### 0x87 - Subcode Q enquiry ###
13 bytes

### 0x88 - Diskcode ###
14 bytes. Unsure what exactly this is, all discs tried so far just report 0. 

### 0x89 - Volume Summary ###
5 bytes

### 0x8A - Track Summary ###
8 Bytes

### 0xA2 - XAPARMS ###
4 Bytes. Unsure what exactly this is, all discs tried so far just report 0. 

### 0xA3 - Disable FP ###

## New ##

TBC - change this one as 0xA2 is in use!
### 0xA4 - FP Command ###
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
