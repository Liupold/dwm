dwm - dynamic window manager (liupold's build)
============================
dwm is an extremely fast, small, and dynamic window manager for X.

### Based on dwm-6.2

# Features
* Clean bar, (no window name)
* Only show occupied tags
* Gaps
* Default bar (No bloat),`xsetroot`
* xrdb (`pywal` supported)
* Key bindings for media key (volume, brightness)
* swallowing patch
* `TERMCMD` launch program in terminal (`config.h`)

# Fonts

```
static const char *fonts[] = { "Fira Code:size=10", "Font Awesome 5 Free Solid:size=10" };
```

# Screenshot

![alt text](2020-10-03-085254_1920x1080_scrot.png)

Requirements
------------
In order to build dwm you need the Xlib header files.


Installation
------------
Edit config.mk to match your local setup (dwm is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install dwm (if
necessary as root):

    make clean install


Running dwm
-----------
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:

    DISPLAY=foo.bar:1 exec dwm

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:

    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm


Configuration
-------------
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code.
