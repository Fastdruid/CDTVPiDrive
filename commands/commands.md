# This file documents both the original and new commands #

As any arbitary command can be run from the Amiga and the program on the Pi can do anything you can do with a Pi running Linux the only limits are our imaginations!  
The intention is to allow anything "different" to run a script or program on the Pi. To avoid multiple people doing different stuff I want to keep this as being the single source of new commands. If anyone wants a new command for any reason raise an issue and I'll add a "command" in here for it (along with a link if you desire to a repo etc). 
# Notes #

All original commands except one are 7 bytes in length. The one exception being 0x81 which is a single byte command. The first byte being the command itself followed by 6 bytes that are the options to that command.  While it turns out the CDTV can send an arbitary length command at the moment the code on the CPLD only allows 7 bytes so until this changes this will remain the limit.

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
This section describes commands that are not valid on an original drive. They should not be used without a PiDrve as unexpected results may occur! 

| Command Name | Command Byte | Byte 1 | Byte 2 | Byte 3 | Byte 4 | Byte 5 | Byte 6 | Returns | Description |
| --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- |
| FP Command | 0xa4 | 0x80 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Front Panel PLAY Pressed |
| FP Command | 0xa4 | 0x60 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Front Panel STOP Pressed |
| FP Command | 0xa4 | 0x40 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Front Panel FF Pressed |
| FP Command | 0xa4 | 0x20 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Front Panel REW Pressed |
| FP Command | 0xa4 | 0x08 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Front Panel Requested current Track |
| --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- | --------------- |
| ODE Command | 0xcd | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 2 bytes | Return how many CD images are currently indexed |
| ODE Command | 0xcd | 0x01 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | Nothing | Refresh / rescan directory for new CD Images |
| ODE Command | 0xcd | 0x02 | Disc ID Byte 1 | Disc ID Byte 2 | 0x00 | 0x00 | 0x00 | Nothing | Load CD with ID the ID given (use ID 0xffff to eject *without* loading a new CD).|
| ODE Command | 0xcd | 0x03 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 2 bytes | Return the current loaded CD ID |
| ODE Command | 0xcd | 0x04 | Disc ID Byte 1 | Disc ID Byte 2 | 0x00 | 0x00 | 0x00 | 1 byte | Return the position of the CD with the ID given in the "quick list" |
| ODE Command | 0xcd | 0x05 | Disc ID Byte 1 | Disc ID Byte 2 | Position | 0x00 | 0x00 | Nothing | Set the position of the CD with the ID given in the "quick list" to "position". |
| ODE Command | 0xcd | 0x06 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 || Nothing | Reset the sort order |
| ODE Command | 0xcd | 0x07 | Disc ID Byte 1 | Disc ID Byte 2 | 0x00 | length | 0x00 | variable bytes | Returns the first "length" bytes of the filename of the CD image. |
| ODE Command | 0xcd | 0x07 | Disc ID Byte 1 | Disc ID Byte 2 | 0x01 | length | 0x00 | variable bytes | Returns the first "length" bytes of the path of the CD image. |
| ODE Command | 0xcd | 0x07 | Disc ID Byte 1 | Disc ID Byte 2 | 0x02 | length | 0x00 | variable bytes | Returns the first "length" bytes of the text description of the CD image. |
| ODE Command | 0xcd | 0x07 | Disc ID Byte 1 | Disc ID Byte 2 | 0x03 | 0x00 | 0x00 | 1 bytes | Returns the catagory of the CD image. |



### 0xA4 - FP Command ###
#### Description ####
Due to the way the PiDrive works the CPLD sends the front panel commands to the Pi the same way as the existing commands, these commands therefore have been slotted into the same sequence. 
#### Options ####
##### Byte 1 - Button code #####
* 0x80 (10000000) which represents PLAY   
* 0x60 (01100000) which represents STOP   
* 0x40 (01000000) which represents FF
* 0x20 (00100000) which represents REW    
* 0x08 (00001000) which represents TRACK  

### 0xA4 - ODE Command ###
#### Description ####
This commands allows control of the underlying CD handling from the OS. 
CD images are given a unique ID from 0x0001 to 0xfffe. 0x0000 and 0xffff are "special" reflecting the ODE & Settings CD and "no CD" respectively. 

##### Byte 1 - Sub-Command #####
* 0x00 - Give a count of current CD images. Two bytes up to 0xfffe (65534)
* 0x01 - Refresh the directory searched for images. For use if you've just uploaded something new.
* 0x02 - Load a CD (if to reboot or not is handled by the program calling this).
* 0x03 - Return the ID of the currently loaded CD.
* 0x04 - Return the position in the "quick list" of the CD with the given ID.
* 0x05 - set the position in the custom list for the disc with id given by byte 2 & 3 and position by byte 4. Note that to prevent duplicates this will *remove* the already existing CD from that "slot".
* 0x06 - reset the custom list to default sort order (in ID order) 
##### Byte 2 - first byte of disc UID #####
##### Byte 3 - Second byte of disc UID #####
##### Byte 4 - Position in custom list #####



