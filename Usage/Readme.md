# Usage Instructions


There are four ways to use the CDTV PiDrive.

## 1) Front Panel / Remote buttons. 

The front panel buttons (and by extension the remote buttons) are interpreted by the PiDrive. This allows us to add extra functionality. 

In normal use with an image loaded they (should!) behave identically to a standard CDTV with one difference, you double press stop it "unloads" the current image (if loaded) and resets the pre-selection id to zero

Without an image loaded the buttons function slightly differently.   

Pressing the FF or Rev buttons will increment the "track" shown on the display, this number reflects an image that is pre-selected from the current list. 
Pressing play will "load" that pre-selected image. 

The buttons will then revert to their normal function. 

## 2) Script / CLI

There is a special program (pidrive-cli) that uses a named pipe to send the same commands to the pidrive program as the CDTV would send. This allows _any_ command to be used. 

Great care should be used with this as sending data to the CDTV that was not requested is likely to result in lots of confusion (aka crashes, lockups etc). 

There are however a number of "extra" commands that have been added to deal with handling images as this allows control from within AmigaOS and these can be used to select images etc.

### pidrive-cli

This sends raw 7 byte MKE commands. 

Located in /opt/PiDrive/bin  
Example: ```pidrive-cli --raw 0x01 0x02 0x03 0x04 0x05 0x06 0x07```

### Scripts

A number of scripts have been written that call pidrive-cli with relevant commands. These are all located in /opt/PiDrive/scripts

#### fp-stop

This sends front panel stop command. 

#### fp-2xstop

This sends two front panel stop commands. 

#### fp-ff

This sends front panel ff command.

#### fp-rev

This sends front panel rev command.

#### fp-play

This sends front panel play command.

#### load-image

This loads the image given, currently limited to only work up to image 255. The image id can be given as hex or decimal. 

Decimal load image 15: ```load-image --dec 15```  
Hex load image 15 15: ```load-image 0x0e```  


## 3) ODEmu
This is a special image that is loaded on selecting image 0. This can be accessed by double pressing stop and then pressing play. 

This is a full gui menu that allows both to select any image as well as organising the pre-selection lists. 

## 4) Web

TBC

