rem Filename:  OW19.BAT
rem  Purpose:  Build Bywater BASIC for MSDOS (80386) using Open Watcom 1.9
rem   Author:  Howard Wulf, AF5NE
rem     Date:  2015-01-29
rem   Uasage:  implementation defined
rem  Example:
rem            cd \sdcard\Download\BASIC\bwbasic3\
rem            OW19.BAT
rem
wcl386 -bc -c -ox -q -wx -za -DHAVE_MSDOS=1 -D__WATCOM__ bw*.c
wlink option quiet file bw*.obj name bwbasic.exe
rem EOF
