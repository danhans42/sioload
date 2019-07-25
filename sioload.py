#!/usr/bin/env python3
# sioload.py serial EXE upload.
#
#
# Runs on Python 3.7 - requires the pyserial libaries to be installed (do this via PyPi/pip)
# by @danhans42 (instag/twitter/psxdev/@gmail.com)

import sys
import serial
import os
import time
from time import sleep

bps=115200

args = int(len(sys.argv))

def progress(count, total, status=''):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))
    percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)
    sys.stdout.write('- %s Data [%s] %s%s %s\r' % (stat,bar, percents, '%', status))
    sys.stdout.flush()

def usage():
	sys.stdout.write("   usage :  sioload.py <command> <port> <file>\n\n")
	sys.stdout.write("commands\n")
	sys.stdout.write("     -run : upload & execute PSX-EXE @115.2k (PSXSERIAL/UNIROM/HITSERIAL compatible)\n")
	sys.stdout.write("     -trun: upload & execute PSX-EXE @345.6k (SIOLOADER only)\n\n")
	sys.stdout.write("where <port> is the name of your serial port and <file> is the name of the file to upload/download\n\n")
	sys.stdout.write("      eg  : sioload.py -run /dev/ttyUSB0 greentro.exe\n")
	sys.stdout.write("            sioload.py -trun COM5 trancetro.exe\n")



def uploadexe():
	serialport = sys.argv[2]
	filename = sys.argv[3]
	filesize = os.path.getsize(filename)
	inputfile = open(filename, 'rb')
	inputfile.seek(0, 0)
	header = inputfile.read(2048)
	inputfile.seek(16,0)
	pc = inputfile.read(4)
	inputfile.seek(24,0)
	addr= inputfile.read(4)
	inputfile.seek(28,0)
	fsz = inputfile.read(4)
	blockcount = int (filesize /2048)
	sys.stdout.write("Port       : ")
	sys.stdout.write(serialport)
	sys.stdout.write("\nSpeed      : {}bps\n".format(bps))
	sys.stdout.write("EXE Name   : {}\n".format(filename))
	sys.stdout.write("EXE Size   : {} bytes\n\n".format(filesize))
	cont = 'y'
	if cont != 'y':
		quit()
	else:
		ser = serial.Serial(serialport,bps,writeTimeout = 1)
		ser.write(b'\x63')
		sys.stdout.write('- Sending Sync\n')
		sleep(1.0)
		ser.write(header)
		sys.stdout.write('- Sending Header\n')
		sleep(0.1)
		ser.write(pc)
		sys.stdout.write('- Sending Init PC\n')
		sleep(0.1)
		ser.write(addr)
		sys.stdout.write('- Sending Addr\n')
		sleep(0.1)
		ser.write(fsz)
		sys.stdout.write('- Sending Filesize\n')
		packet = 1
		offset = 2048
		while packet < blockcount:
			inputfile.seek(packet*2048,0)
			bin = inputfile.read(2048)
			progress(packet, blockcount-1, status='')
			ser.write(bin)
			offset += 2048
			packet += 1
		EOF = 0
		while EOF < 2048:
			ser.write(b'\xFF')
			EOF += 1
		sys.stdout.write('\n- Executing')
		sys.stdout.write('\n- Operation Complete\n')

#main

		
sys.stdout.write('\sioload.py - by @danhans42\n\n')

if args < 4:
	sys.stdout.write("error: insufficient parameters\n\n")
	usage()
	
else:
	command = sys.argv[1]
	serialport = sys.argv[2]
	file = sys.argv[3]
	
	if  command == "-run":
		stat = "Uploading"
		uploadexe()
	elif command == "-trun":
		stat = "Uploading"
		bps = 1036800
		uploadexe()
	else:
		sys.stdout.write("error: invalid command\n\n")
		usage()
	
	
	