# slslstatus - suckless suckless status

slslstatus is a suckless status monitor for window managers that use WM_NAME (e.g. dwm) or stdin to fill the status bar.  
This was originally (and is mostly, still) based on suckless's slstatus. I fixed the volume checking function and added a few features so it fit my specific needs/wants:  
- a way to make it only run 1 iteration of updating the top bar.  
- only update the bar once a minute (right at the beggining of the minute).  
- since this project was only meant to be used by me and maybe a few friends, I got rid of the code that refered to non-linux systems.  

So the project of editing this code was born both as a need/want and as a way to learn a bit more of C and good code practices. *It was/is being fun.*  
I'll most likely be making chances (mostly small adjustments) to this project over time.  

Go checkout suckless's projects at [suckless.org](https://suckless.org), they have amazing stuff over there.

# Features

- Battery percentage/state/time left
- CPU usage
- CPU frequency
- Custom shell commands
- Date and time
- Disk status (free storage, percentage, total storage and used storage)
- Available entropy
- Username/GID/UID
- Hostname
- IP address (IPv4 and IPv6)
- Kernel version
- Keyboard indicators
- Keymap
- Load average
- Network speeds (RX and TX)
- Number of files in a directory (hint: Maildir)
- Memory status (free memory, percentage, total memory and used memory)
- Swap status (free swap, percentage, total swap and used swap)
- Temperature
- Uptime
- Volume percentage (ALSA)
- WiFi signal percentage and ESSID


# Requirements

Currently slstatus only works on Linux.  
In order to build slstatus you need the Xlib header files.  


# Installation
Edit **config.mk** to match your local setup (slstatus is installed into the /usr/local namespace by default).  

Afterwards enter the following command to build and install slstatus (if necessary as root):  `make clean install`  


# Running slstatus

See the man page for details.


# Configuration

slstatus can be customized by creating a custom config.h and (re)compiling the source code. This keeps it fast, secure and simple.
