# Workbench

## Legal situation

I/We cannot distribute Workbench or any Copyrighted software/files.  We're all fully aware that Commodore have been gone for 30 years and Workbench 1.3 is close to 40 years old but its still under copyright and to avoid any legal issues no copyrighted software has been included.   
This includes RMTM which is required to remove the trademark warning, C4ptFuture has written a freely distributable version to get round this issue. 

This is a problem because we really want to have a nice GUI program to select which images etc you want to use. 

## The Trademark file

The original CDTV as shipped required the inclusion of a trademark file to be either encoded into the disc or as a file on the disc. This file contains copyrighted code (presumably intentionally) so that any disc with it distributed without the appropriate license could be subject to legal challanges. 

With Commodore defunct it's clear as mud who you'd even get to license the use of this. 
It is strongly recommended to run CDTV OS 2.35. This removes the requirement for the trademark file and so removes the problem with its inclusion (or lack of). Without the trademark file there is an issue and the CD will not boot. 

### Workround

There are two ways round this. Firstly you provide the trademark file. This can be found on any number of CDTV discs (look for cdtv.tm), secondly a script can try and download it for you. 

The preference is for you to save this to /opt/PiDrive/ISOBuilder

## Workbench

The same goes here for all the Workbench files. While you _have_ a genuine Commodore product which _came_ with Workbench 1.3 I cannot supply it for you to use. 

So, by default the ISO does not have any Copyright files on it. This does make it somewhat useless for anything other than disc image selection. To allow you to have a more usable disc we have created a script that will download a CD image from the internet, take the contents that ISO and populate the image with the contents of Workbench. 

The running of this script should only be performed under the understanding that you are legally entitled to use the software,  allowed to download it and you accept any legal consequences from running the script are entirely on you. 

To run it go to /opt/PiDrive/ISOBuilder and run AddWorkbench
