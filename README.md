# sioload
SIO Loader for Sony PlayStation Consoles

Basically its the source of Serial v1.3 by Hitmen but with selectabe serial speed.

Uses a very simple serial cable (rxd/txd/gnd) that is easy to build. 

The client is written in Python 3, and has been tested on Python 3.7 on both Windows and Linux. It requires pyserial, which isnt always included. Google can help if you get stuck.

You can use two speeds - 115,200bps or 345,600bps. Once you have selected the requried speed on the PSX, use the corresponding on the client..

   usage :  sioload.py <command> <port> <file>

commands
     -run : upload & execute PSX-EXE @115.2k (PSXSERIAL/UNIROM/HITSERIAL compatible)
     -trun: upload & execute PSX-EXE @345.6k (SIOLOADER only)

where <port> is the name of your serial port and <file> is the name of the file to upload/download

      eg  : sioload.py -run /dev/ttyUSB0 greentro.exe
            sioload.py -trun COM5 trancetro.exe

Few notes :-

Size limit should be similar to Serial v1.3 ~1700k. 

345,600bps - I have only tested this with a *GENUINE* FTDI USB UART. If you have issues switch to 115,200bps.







