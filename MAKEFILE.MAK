# ----------------------------
# PlayStation 1 Psy-Q MAKEFILE
# ----------------------------
all:
	del sioload.exe
	ccpsx -O2 -Xo$801D0000 sioload.c -osioload.cpe,sioload.sym,mem.map
	cpe2x /ce sioload.cpe
	del sioload.cpe
    del sioload.sym
    del mem.map
rom:	
	copy /B romprod/romprod.rom + sioload.exe sioload.rom


