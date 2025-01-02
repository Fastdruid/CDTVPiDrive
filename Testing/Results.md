# This page is for testing results to keep a log. 

## Notes

EFFK is synced to the frame (7350 frames / second for a single speed CDROM) 7.35kHz while a CD is spinning, ~9.07kHz when its stopped/no CD, frequency dips as the CD spins up or seeks.   
SCOR is synced to the Sector, as there are 98 frames per sector this then runs at 75Hz. In theory this is triggered by the synchronizing signal from the Subcode, either S0 or S1... in practice it turns up regardless of there being a CD in the drive!   

![image](https://github.com/user-attachments/assets/282616fb-a77e-4455-ba26-271e813d1896)

_*Note* t - Oscilationg frequency (VCO, typically 1/8.6436MHz = 115ns)_


SCCK is 8 pulses at CCK frequency (3.546895 MHz for PAL, 3.579545 MHz for NTSC). This clocks data via SBCP. Without SCCK then SBCP merely outputs the "P" Subcode (this is part of a parallel subcode interface, the other 7 bits are also output in parallel from the M50423FP). Normally SBCP outputs P and then the remaining bits of the subcode byte are clocked by SCCK.   
This is all in "hardware" by the M50423FP and explained here in the datasheet.
![image](https://github.com/user-attachments/assets/05e1c2eb-4849-4fdd-853b-4a03d5a85a07)

This shows how the data is clocked.

![image](https://github.com/user-attachments/assets/4953c02e-5d2b-439d-89a5-50b7e1cd2652)

_*Note* t - Oscilationg frequency (VCO, typically 1/8.6436MHz = 115ns)_


Finally... WTF were Commodore thinking here? The M50423 has parallel subcode output, there is this horrible serial clock generator and serial to parallel converter where they could have saved ~6 chips by binning EFFK, SCCK and adding the parallel SBCW/V/U/T/S/R/Q. Ok, would have needed 5 extra pins on the connector but one is N/C (29), Mute isn't used and it wouldn't have been beyond the pale to make it a 44pin connector!


## Oscilloscope pictures. 

### These ones thanks to chriskuta

#### EFFK (top) / SCOR (bottom)
![image](https://github.com/user-attachments/assets/61c7fd66-0f9a-45d9-9f12-04d315a0344f)
#### SBCP (top) / SCCK (bottom)
![image](https://github.com/user-attachments/assets/c5d7332d-72bb-4f06-a828-14f7ade68b37)
#### EFFK (top) / SCCK (bottom)
![image](https://github.com/user-attachments/assets/d6224742-067b-4971-b8d0-3a659ab37855)




