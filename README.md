# Introduction

Prayer notifyer is a daemon program that calculates islamic prayers' times and counts down to
the next prayer.
The program uses external API services to extract some important values for calculation given your locality,
however you can provide those values manually without using the services. Look into config.h for more information.


# Installation

```sh
git clone https://github.com/DeepBleu64/PrayerNotifyer && cd PrayerNotifyer && sudo make install 
```

# Uninstallation

```sh
sudo make uninstall
```

# Motivation

The idea of displaying the time remaining for the next prayer as one of the information on my DWM status bar has
always intrigued me, and since there's no such thing that meets my needs i've written the program.

# Usage

The program is meant to be primarily run as a daemon to display information continuously on your window manager's status bar.
The way i've managed to accomplish that is through UNIX named pipe.

I have the following snippet right before the infinite while loop in my xsetroot.sh file.

```sh
[[ -p /tmp/prayer.fifo  ]] || mkfifo /tmp/prayer.fifo

prayer-notifyer > /tmp/prayer.fifo &


display_prayer() {
    read LINE < /tmp/prayer.fifo
	echo $LINE
}
```
