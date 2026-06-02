# This attempts to document a way to do a SANA-II type network interface via the PiDrive

## Commands

| Command Byte | Command Name  |
| --------------- | --------------- | 
| 0xC0 | IP setup / control | 
| 0xC1 | IP data / transmit | 
| 0xC2 | IP poll / receive status | 
| 0xC3 | IP fetch / receive data | 
| 0xC4 | IP ack / bookkeeping | 
| 0xC5 | IP reset / clear state | 

## IP setup / control

| Byte | function  |
| --------------- | --------------- | 
| Byte 0 | 0xC0 |
| Byte 1 | Subcommand |
| Byte 2 | Protocol version |
| Byte 3 | flags |
| Byte 4 | MTU low byte |
| Byte 5 | MTU high byte |
| Byte 6 | Reserved |

### Subcommands

| Byte value | Subcommand  |
| --------------- | --------------- | 
| 0x00 | Query capabilities |
| 0x01 | set local IP |
| 0x02 | set netmask |
| 0x03 | set gateway |
| 0x04 | set dns |
| 0x05 | enable/disable DHCP |
| 0x06 | set link mode |

## IP Data / transmit

| Byte | function  |
| --------------- | --------------- | 
| Byte 0 | 0xC1 |
| Byte 1 | Sequence number |
| Byte 2 | Flags |
| Byte 3 | Data byte 0 |
| Byte 4 | Data byte 1 |
| Byte 5 | Data byte 2 |
| Byte 6 | Data byte 3 |

## IP Poll
| Byte | function  |
| --------------- | --------------- | 
| Byte 0 | 0xC2 |
| Byte 1 | poll type |
| Byte 2 | reserved |
| Byte 3 | reserved |
| Byte 4 | reserved |
| Byte 5 | reserved |
| Byte 6 | reserved |

Replies back with 6 bytes that notifies there is data (or not). 

| Byte | function  |
| --------------- | --------------- | 
| Byte 0 | status |
| Byte 1 | sequence number |
| Byte 2 | available data low byte |
| Byte 3 | available data high byte |
| Byte 4 | packet type |
| Byte 5 | flags |

## IP Fetch

| Byte | function  |
| --------------- | --------------- | 
| Byte 0 | 0xC3 |
| Byte 1 | sequence number or fetch token |
| Byte 2 | requested chunk index / offset |
| Byte 3 | requested length low byte |
| Byte 4 | requested length high byte |
| Byte 5 | reserved |
| Byte 6 | reserved |

Replies back with the requested data




