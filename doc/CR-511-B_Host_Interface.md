# MKE CR-511-B host interface: reference for the Commodore CDTV ODE

## 0. Status and provenance

- **Source 1:** Sanyo *LC8950 & LC8951 Real-Time Error Correction and Host Interface Processors, Design Manual* (scanned copy, 139-page PDF, not committed to the repo). Written in our own words. Nothing here is copied from the manual.
- **Source 2:** Sanyo *M50423FP CD PLAYER DIGITAL SIGNAL PROCESSOR* (scanned copy, 21-page PDF, publicly available but not committed to the repo). Written in our own words. Nothing here is copied from this datasheet. 
- **Source 3:** Commodore CDTV Service Manual
- **Source 4:** Logic Analyser recordings of interfaces. 
- **Confidence tags:** `[text]` stated in the manual's prose, `[fig]` read from a timing diagram, `[table]` read from an AC table, `[measured]` actual behaviour from a logic analyser.
- **Quality warning:** Source 1 is a degraded scan. Numbers marked `[table]` were read from page images, not the OCR text, but should be re-checked against the PDF before being used as hard limits.
- **Analyser limitations:** The analyser is sampling at varying rates 16Mhz-32Hz. This gives a minimum detected period of 62.5ns-31.25ns. 
- **Scope:** the **host-side (front-end) interface only**: what the CDTV sees at the pins.
- **Chips:** for this use LC8950, LC8951, LC89510 and LC89515 behave identically.
- **This document describes the actual interface.**
- How the CPLD/Pi/RP2350 implementation behaves is documented separately in `docs/CDTV_PiDrive-implementation.md`. 

## 1. Role in this project

The MKE CR-511-B is a single speed CD-ROM used exclusively in the Commodore CDTV.  utilises a Sanyo LC8951 and M50423FP

The ODE **emulates the entire drive**, the LC8951 chip is the **host interface**, the M50423FP is the **DSP**. The CDTV is the **host**. So:

- The ODE always **drives** the following outputs: `STEN`, `DRQ`, `EFFK`, `SCBP`, `SCOR`.
- The ODE always **samples** the following inputs: `ENABLE`, `CMD`, `HRD`, `HWR`, `RESET`, `SCCK`, `SCK`.
- The ODE **samples** `HD[0-7]` during command write from CDTV *to* the drive/ODE). 
- The ODE **drives** `HD[0-7]`  during data read from drive/ODE *to* the CDTV.
- The ODE **samples** or **drives** `SDATA` depending on position in the cycle (see CN26 Interface).

## 2. Signals

Directions are from the **CR-511-B** point of view (so the ODE implements the "out" signals).

CN9 - Main interface connector.

| PIN | Signal | Dir | Active | Meaning | Source |
|---|---|---|---|---|---|
| 1 | `/RESET` | in | low | drive reset | [text] M p.18 / PDF p.29 |
| 2 | GND | | | | |
| 3 | `EFFK` | out | clock | EFM Frame clock. Output duty = 50% | |
| 4 | `SCCK` | in | | Shift clock for serial subcode data output | |
| 5 | `SBCP` | out | | Subcode Pch output Pch~Wch serial data output | |
| 6 | `SCOR` | out | | Signal Composite Optical Read | |
| 7 | GND | | | | |
| 8 | GND | | | | |
| 9 | `C16M` | out | clock | 16MHz DAC and LC8951 Clock | |
| 10 | GND | | | | |
| 11 | `/XAEN` | out | low | Unknown. Undocumented. Unused! | |
| 12 | GND | | | | |
| 13 | `AEMP` | out | high | | |
| 14 | `DATA` | out | high | Digital audio. | |
| 15 | `LRCLK` | out | clock | Audio Left / Right clock | |
| 16 | `BCLK` | out | clock | Audio bit clock | | 
| 17 | `/MUTE` | out | high | Audio mute signal - Not connected | | 
| 18 | GND | | | | |
| 19 | `/INAC` | out | low | INdicator ACtivity Light | |
| 20 | `/STCH` | out | low | STatus CHange | |
| 21 | `/ENABLE` | in | low | Host interface enable | |
| 22 | `DRQ` | out | high | Data Request Queue - Data ReQuest
| 23 | `/HWR ` | in | low | Host data write input | |
| 24 | GND | | | | |
| 25 | `/DTEN` | out | low | Data enable output - Not used | |
| 26 | `/HRD` | in | low | Host data read input. This is directly connected to the 74LS245 bus transceiver direction | | 
| 27 | `/STEN` | in | low | Status enable output | |
| 28 | `/CMD` | in | low | Host command / data select | |
| 29 | `/EOP` | out | n/c | End of Process - Not used or connected | |
| 30 | GND | | | | |
| 31 | `DB7` | bi | | Host interface bit 7 | |
| 32 | GND | | | | |
| 39 | `DB6` | bi | | Host interface bit 6 | |
| 39 | `DB5` | bi | | Host interface bit 5 | |
| 39 | `DB4` | bi | | Host interface bit 4 | |
| 36 | GND | | | | |
| 39 | `DB3` | bi | | Host interface bit 3 | |
| 39 | `DB2` | bi | | Host interface bit 2 | |
| 39 | `DB1` | bi | | Host interface bit 1 | |
| 40 | `DB0` | bi | | Host interface bit 0 | |

CN26 - serial control connector

Note: SDATA is active HIGH but also held HIGH by a pullup. This means when not being actively driven it will default to HIGH. 

| PIN | Signal | Dir | Active | Meaning | Source |
|---|---|---|---|---|---|
| 1 | `SCK` | in | clock | serial interface clock | |
| 2 | `SDATA` | bi | high | Serial Data | |
| 3 | GND | | | | |

## 3. Bus cycle decode

All decoding requires `/ENABLE` low.

| `/CMD` | `/HWR` | `/HRD` | Cycle | Data bus |
|---|---|---|---|---|
| low | low | high | **Command write** | Host drives `HD[0-7]`; drive captures the byte |
| low | high | low | **Status read** | Drive drives `HD[0-7]` with the status byte |
| high | high | low | **Data read** | Drive drives `HD[0-7]` with the next data byte |
| any other combination | | | **Undefined**; the manual says not to use them, including `HWR` and `HRD` both low | |

Source: [text] M p.18 / PDF p.29. The ODE should ignore or at least not misbehave on the undefined combinations.

## 4. Command write - measured timings

1. The CDTV simultaneously sets /ENABLE and /CMD LOW. [measured]
2. 4.25μs-4.8125μs later CDTV places a byte on `HD[0-7]` and simultaneously pulses `/HWR` LOW for 437.5n-500ns (with `/ENABLE` and `/CMD` still held LOW). [measured]
3. Bytes are written one per `/HWR` pulse at 3.9375μs intervals (437.5ns-500ns LOW pulse followed by 3.4375μs-3.5μs HIGH period). Multi-byte commands are just consecutive writes. [measured]
4. CDTV simultaneously releases /ENABLE and /CMD 6μs after the last written byte. [measured]
5. Reset aborts 

| Parameter | Min | Max |
|---|---|---|
| `ENABLE` low to `HWR` low (setup) | 30 | |
| `CMD` to `HWR` low (setup) | 15 | |
| `HWR` low period | 50 | |
| `HD[0-7]` valid to `HWR` inactive (setup) | 50 | |
| `HWR` inactive to end of data (hold) | 20 | |
| `HWR` high period (between writes) | 50 | |
| `HWR` inactive to `ENABLE` inactive | 0 | |
| `HWR` inactive to `CMD` inactive | 5 | |
| `HWR` to `DTEN`/`STEN` change (command break) | | 105 |

## 5. Status read (STEN handshake) - measured timings

Normal sequence. [text] M p.54 / PDF p.65; [fig] Fig 13 (M p.73 / PDF p.84)

1. When the status data is available `/STEN` is set LOW and the byte is placed on `HD[0-7]`
2. 99.0625μs later CDTV simultaneously sets `/ENABLE` and `/CMD` LOW. [measured]
3. 875ns later the CDTV reads the byte by pulsing `/HRD` LOW 437.5n-500ns (with `/ENABLE` and `/CMD` held LOW) [measured]
4. `/STEN` returns HIGH on detection of `/HRD` as the host starts the read (at the falling edge of `/HRD`). [fig] Fig 13; [text] M p.27 (register description: pin edges follow the `/HRD` falling edge, internal bit changes follow the rising edge) [measured]
5. CDTV simultaneously releases /ENABLE and /CMD 4μs-4.06256μs after the last written byte. [measured]
5. Further status bytes repeat the sequence, one `STEN` cycle per byte. Each new `/STEN` HIGH is 140ns. [measured].  Note: As the ODE can respond quicker than the real drive it is possible to respond too quickly and this will be missed by the CDTV. As speed improvements are desired (but missed bytes are not) the minimum `/STEN` HIGH should be a configurable default.

Rules and corner cases:

- The host should only read status while `/STEN` is low. Reading while it is high returns invalid data (marked "invalid data" in Fig 13). [text] M p.54; [fig] Fig 13
- If the controller writes a new status byte while the host is mid-read, the new byte is flagged ready (`/STEN` goes low again) just after the read finishes, but the manual says the integrity of that byte "cannot be guaranteed". [text] M p.54
- Status output can be aborted by chip reset, by the controller disabling status output, or by a command break (a command write with break enabled). [text] M p.54-55 / PDF p.65-66

## 6. Status byte read - measured timings

Only one byte. See "Status Byte" for breakdown of contents and which conditions set `STCH`.

1. `STCH` set HIGH to indicate a change to the "Status Byte"
2. CDTV sends command 0x81 (see command write)
3. 510.9375μs after `/ENABLE` and `/CMD` return HIGH `STCH` resets to LOW [measured]
4. 922.6875μs after `/ENABLE` and `/CMD` return HIGH `/STEN` is set LOW [measured]
5. Status Byte is read as per "Status read (STEN handshake)"


## 7. Data read in DRQ (DMA) mode - measured timings.

CDTV will send a command (see "Command write") and then 177.625μs later set `/ENABLE` LOW while `/CMD` remains HIGH
As this is an early single speed CD-ROM the first read is slow and the CDTV *expects* it to be slow. Experimentation has shown that the ODE can respond too quickly (before the CDTV is expecting data) but shortening this time will result in faster data transfers. For this reason minimum periods should be set as configurable defaults.

178.9226875ms after `/ENABLE` is set HIGH from the command [measured] the following cycle starts.

Data is sent in sector size chunks (typically 2048 bytes) and the gap between these chunks is 12.1590625ms - 12.193375ms. Testing however has shown that this gap can be eliminated resulting in significantly faster data transfers but should be set as a configurable default to retain "cycle exact" timing.   

1. The byte is placed on `HD[0-7]` and `DRQ` is set HIGH to indicate there is data.
3. 187.5ns after `DRQ` goes HIGH, the CDTV performs a read by pulsing `/HRD` LOW for 187.5ns
4. 62.5ns after `/HRD` goes LOW `DRQ` is reset to LOW
5. If there is more data then 312.5ns-375ns after `DRQ` goes LOW the cycle repeats. Noting that if a full sector has been sent then there will be the sector gap. 

The CDTV does not utilise `/EOP` and will only end the transmission when all the data it is expecting has been sent. 

The CDTV will reset `/ENABLE` to HIGH 125.0625μs after DRQ goes LOW for the last time. 

## 8. Serial Control

The CR-511-B has a three wire interface which allows audio control of the drive and for the drive to report back. There are only 5 known commands. 

This interface is controlled by the HOST and follows one of the following cycles.

Write cycle (HOST to DRIVE)
1) HOST checks for `SDATA` HIGH. If `SDATA` is HIGH then perform write cycle
2) HOST drives `SDATA` LOW and then 33.94μs later starts 8 pulses on `SCLK` (5.76-5.78μs LOW, 8.2μs HIGH)
3) HOST sends a bit by setting `SDATA` HIGH or LOW as appropriate on the falling edge of `SCLK`. 
4) HOST disconnects drive to `SDATA` allowing it to return HIGH 9.7μs after the rising edge of the last clock pulse.

Read cycle (DRIVE to HOST)
1) DRIVE checks for `SDATA` HIGH (while not reading/writing data). 
2) DRIVE drives `SDATA` LOW and waits for the `SCLK` pulse from the HOST.
3) HOST checks for `SDATA` HIGH. If `SDATA` is LOW then this is now a read cycle.
4) HOST starts 8 pulses on `SCLK` (5.76-5.78μs LOW, 8.2μs HIGH)
5) DRIVE sends a bit by setting `SDATA` HIGH or LOW as appropriate on the falling edge of `SCLK`.
6) DRIVE disconnects drive to `SDATA` allowing it to return HIGH 20.8μs after the rising edge of the last clock pulse.

Command set (known)
| Name | Byte | Description |
| --- | --- | --- |
| TRACK | 00001000 | Request current track - This is the only command with a response and sends the current playing track (or 0 for no playing track)|
| STOP | 01100000 | Stop playback |
| PLAY | 10000000 | Play |
| REW | 00100000 | Previous Track |
| FF | 01000000 | Next Track |

### 8.1 Example. 

1) HOST sends 00001000 (Request current Track).
2) 9.7μs after `STATA` has returned HIGH the DRIVE then drives `SDATA` LOW and waits for `SCLK` to pulse.
3) 4.3779ms later the HOST starts the 8 clock pulses on `SCLK`
4) DRIVE detects the falling edge of `SCLK` and sets `SDATA` as appropriate (typically 4μs after the falling edge).
5) HOST reads the data on the rising edge of `SCLK`
6) DRIVE resets `SDATA` LOW on the falling edge of `SCLK`
7) Once all bits have transferred (8 pulses of `SCLK`) the DRIVE disconnects drive to `SDATA` allowing it to return HIGH. 

## 9. Reset

- `/RESET` low,  resets the chip. [text] M p.59 / PDF p.70
- On reset: all tristate outputs go high-impedance (including `HD[0-7]`), the command FIFO is cleared, the interface control and decoder control registers are cleared (which disables data and status output until the controller re-enables them), and the MCK output is suppressed. [text] M p.59
- Reset timing (Fig 41, M p.115 / PDF p.126): minimum `/RESET` pulse width is about 100 ns, with output hold times of the order of 50 to 100 ns. These values were read from a poor OCR layer and **need checking against the page image**.
- An in-progress status or data transfer is terminated by reset. For data transfers, no transfer-end interrupt is generated when a transfer is aborted this way. [text] M p.54, M p.58
