## INSTALLATION INSTRUCTIONS - DEV/TEST ##

Note that Windows 10 was used for this installation however as the imager is available for macOS and Ubuntu there should be no significant changes. Raspberry Pi Imager unfortunately unavailable for AmigaOS!

1. Install the Raspberry Pi Imager (v1.9.4 used - https://www.raspberrypi.com/software/
2. Insert a MicroSD card into the computer. Recommended at least 64GB and at absolute minimum a Class 10 (recommended UHS).
3. Run the Raspberry Pi Imager selecting the Device as "RASPBERRY PI ZERO 2 W".
4. For the OS select "Raspberry Pi OS (other)" and then "Raspberry Pi OS Lite (32-bit)".
5. Select your storage. Ensure you select the correct device!
6. Select Next
7. When the "Would you like to apply OS customisation settings" window pops up select "EDIT SETTINGS"
8. Set the hostname as appropriate.
9. Set a suitable username and password
10. Setup your wireless LAN. Note that both the SSID and password are case sensitive!
11. On the "Services" tab ensure "Enable SSH" is ticked.
12. If you know what you are doing setup public-key authentication. If you don't then leave it as "Use password authentication"
13. Select SAVE and then YES
14. Answer YES to the "All existing data will be erased" warning (as long as you are very sure you selected the correct device!)
15. When the "Write Successful" message pops up remove the MicroSD card and ensuring the CDTV is powered off insert it into the MicroSD slot on the CDTV PiDrive (accessible with the Video card removed).
16. Power on the CDTV
17. The Raspberry Pi will update some settings and do a couple of reboots which may take a minute or two. It should automatically connect to your Wifi. 
18. Check your router to see which IP address the Raspberry Pi is using. If you do not have access to the router or you're unsure then you can download Advanced IP Scanner from https://www.advanced-ip-scanner.com/
![image](https://github.com/user-attachments/assets/91c0b9bf-9d51-4385-bbab-5650dd3f52aa)
19. Using an ssh client connect to the IP address shown. If you do not have an ssh client then I recomment Putty - https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html for windows download the MSI ('Windows Installer') appropriate for your hardware.
20. Using the login you setup earlier (You *DID* setup a login at step 9 didn't you?)
21. Add the CDTV PiDrive repository. Copy the following and paste it into your ssh window.

  `echo "deb [trusted=yes arch=armhf] https://fastdruid.co.uk/PiDrive/ testing main" | sudo tee /etc/apt/sources.list.d/fastdruid.list`  
  `sudo apt update`  
  `sudo apt upgrade`  
  `sudo apt install cdtvpidrive`  
  `sudo apt install cdtvfirmware`  
  `sudo apt install cdtvisobuilder`  
  `sudo shutdown -r now`  


.... and thats it for now


