## MicroSD INSTRUCTIONS - DEV/TEST ##

**NOTE:** By installing cdtvpidrive it will make a number of system changes. Do not install this (except for testing) on a Pi not intended for this. It is recommended to wipe the MicroSD card to if you desire to uninstall. 


Note that Windows 10 was used for this installation however as the imager is available for macOS and Ubuntu there should be no significant changes. Raspberry Pi Imager unfortunately unavailable for AmigaOS!

1. Install the Raspberry Pi Imager (v2.0.6 is current at time of writing - https://www.raspberrypi.com/software/)
2. Insert a MicroSD card into the computer. Recommended at least 64GB and at absolute minimum a Class 10 (recommended UHS).
3. Run the Raspberry Pi Imager selecting the Device as "RASPBERRY PI ZERO 2 W".
<img width="682" height="482" alt="image" src="https://github.com/user-attachments/assets/a49fbd17-52ec-43c3-9897-20f828fb9741" />

4. For the OS select "Raspberry Pi OS (other)" and then "Raspberry Pi OS (Legacy, 32-bit) Lite".
Ensure that you select Bookworm and not Trixie (Trixie is untested). 
<img width="682" height="482" alt="image" src="https://github.com/user-attachments/assets/a97b62ed-f9eb-4933-bacb-79de63b4f360" />

5. Select your storage. Ensure you select the correct device!
6. Select Next
7. Choose the hostname. I'd recommend "PiDrive" but you can call it what you want.
8. When the "Would you like to apply OS customisation settings" window pops up select "EDIT SETTINGS"
9. Set the hostname as appropriate.
10. Set the time zone and keyboard layout
11. Set a suitable username and password
12. Setup your wireless LAN. Note that both the SSID and password are case sensitive!
13. On the "SSH Authentication" ensure "Enable SSH" is ticked.
14. If you know what you are doing setup public-key authentication. If you don't then leave it as "Use password authentication"
15. Select WRITE
16. Answer "I UNDERSTAND< ERASE AND WRITE" "You are about to ERASE all data on: " warning (as long as you are very sure you selected the correct device!)
17. When the "Write Successful" message pops up remove the MicroSD card and ensuring the CDTV is powered off insert it into the MicroSD slot on the CDTV PiDrive (accessible with the Video card removed).
18. Power on the CDTV
19. The Raspberry Pi will update some settings and do a couple of reboots which may take a minute or two. It should automatically connect to your Wifi. 
20. Check your router to see which IP address the Raspberry Pi is using. If you do not have access to the router or you're unsure then you can download Advanced IP Scanner from https://www.advanced-ip-scanner.com/
![image](https://github.com/user-attachments/assets/91c0b9bf-9d51-4385-bbab-5650dd3f52aa)
21. Using an ssh client connect to the IP address shown. If you do not have an ssh client then I recomment Putty - https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html for windows download the MSI ('Windows Installer') appropriate for your hardware.
22. Using the login you setup earlier (You *DID* setup a login at step 9 didn't you?)
23. Add the CDTV PiDrive repository. Copy the following and paste it into your ssh window.

  `echo "deb [trusted=yes arch=armhf] https://fastdruid.co.uk/PiDrive/ testing main" | sudo tee /etc/apt/sources.list.d/fastdruid.list`  
  `sudo apt update`  
  `sudo apt upgrade -y`  
  `sudo apt install cdtvpidrive`  
  `sudo apt install cdtvfirmware`  
  `sudo apt install cdtvisobuilder`  
  `sudo shutdown -h now`  

At this point you _must_ powercycle the CDTV and its recommended to leave it off for at least 30 seconds.  This is because the firmware loaded to the CPLD requires a cold start to work correctly. 


.... and thats it for now


