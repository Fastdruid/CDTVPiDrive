# Logging

By default CDTV PiDrive will use the systemd journal to log. This will catch both stdout and stderr. 

In addition there is a command log that will log every command received. 

## Modifying output settings.

By default CDTV will not show any messages, if you wish to see under the hood or are looking to diagnose an issue then undoubtedly will need more details.

edit /opt/PiDrive/config/pidrive.cfg with your editor of choice (which is obviously vi).

```
sudo vi /etc/PiDrive/config/pidrive.cfg
```

Locate the option ```log-level``` and set which options you require. With the exception of NONE and ALL they are separate and distinct.

```NONE``` No output. Note that this overrides all other options. Including ALL!
```FATAL``` Show fatal messages.
```WARN``` Show warning messages.
```INFO``` Show info messages.
```DEBUG``` Show debug messages.
```TRACE``` Show trace messages (A *LOT* of messages)
```ALL```  Show all messages.

Once you have made the changes then Esc :wq! 

### Examples

1) This will show fatal, debug and info messages.

```log-level = FATAL DEBUG INFO```

2) This will show **NO** messages.

```log-level = NONE FATAL DEBUG INFO```

3) This will show ALL messages.

```log-level = ALL FATAL DEBUG INFO```

### Viewing

Use the standard Linux commands

View log
```journalctl -u pidrive```

Follow log
```journalctl -u pidrive -f```

Note that default this merely uses the journald timestamps, as the journald timestamps aren't accurate enough to follow very short timeframes (or if you're running this interactively) there is another option.

```log-timestamp```


## Command log. 

Again, by default CDTV PiDrive will not log commands. 

edit /opt/PiDrive/config/pidrive.cfg with your editor of choice (which assuming you've managed to exit the previous vi session is obviously still vi).

```
sudo vi /etc/PiDrive/config/pidrive.cfg
```

By default on Debian Bookworm (including Raspberry Pi OS), logs are volatile and cleared on reboot.


If you are having issues that require keeping logs longer then follow these steps to enable persistence. 

## Enable Persistent Journal Logs
Persistent logging stores systemd journal logs across reboots in /var/log/journal. By default on Debian Bookworm this is volatile and cleared on reboot.

Create the persistent journal directory with proper structure and ownership:

```
sudo mkdir -p /var/log/journal
sudo systemd-tmpfiles --create --prefix /var/log/journal
```
This uses the machine ID to set up subdirectories like /var/log/journal/$(machine-id)/.

Verify the configuration in /etc/systemd/journald.conf (optional but recommended):

```
sudo nano /etc/systemd/journald.conf
```

Ensure or add these lines:

```
[Journal]
Storage=persistent
```

Restart the journal service to apply changes:

```
sudo systemctl restart systemd-journald
```

Test persistence: Reboot, then check logs with ```journalctl -b -1``` (previous boot) or ```ls /var/log/journal```.

## To revert to volatile (in-memory) logging, which clears on reboot:

Edit the journald configuration:

```
sudo nano /etc/systemd/journald.conf
```

Set:

```
[Journal]
Storage=volatile
```
Restart the journal service:

```
sudo systemctl restart systemd-journald
```
(Optional) Remove persistent log files to free space:

```
sudo rm -rf /var/log/journal
```



