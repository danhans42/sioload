# sioloader v1.0

A EXE Loader for Sony PlayStation Consoles that utilises the SIO (serial) port.

Basically its the source of Serial v1.3 by Hitmen but with higher speeds

Uses a very simple serial cable (rxd/txd/gnd) that is easy to build - see over on psxdev.net for more info.

The client is written in Python and has been tested on Python 3.7 on both Windows and Linux. It requires pyserial, which isnt always included in your distribution. Google can help if you get stuck. The python application does ZERO checks! Make sure your EXE is a pukka PSX-EXE and is size aligned to 2048bytes (PsyQ does this).

The client supports two speeds - 115,200bps or 1,036,800bps. the slower option is if you want to use the python client with PSXSERIAL/UNIROM etc). Always use the '-trun' option with SIOLOADER.

commands
     -run : upload & execute PSX-EXE @115.2k (PSXSERIAL/UNIROM/HITSERIAL compatible)
     -trun: upload & execute PSX-EXE @1345.6k (SIOLOADER only)

      eg  : sioload.py -run /dev/ttyUSB0 greentro.exe
            sioload.py -trun COM5 trancetro.exe

Few notes :-

Size limit should be similar to Serial v1.3 ~1700k. 

I have only tested this with a *GENUINE* FTDI USB UARTs. If you have issues edit the speed in the source or just use psxserial. Using a cheap ass FTDI clone can cause issues with closing/locking the port - particularly on Linux I found. Alternatively, I have also used CH340 & CP2103 type USB UARTS with success.

I can personally recommend the Sparkfun FTDI breakout board ~£12 from a local hobby supplier or Mouser etc..









