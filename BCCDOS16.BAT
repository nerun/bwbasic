@echo off
rem Filename:  BCCDOS16.BAT
rem  Purpose:  Build Bywater BASIC for MS-DOS (16-bit) using Borland C++ Version 3.1
rem   Author:  Howard Wulf, AF5NE
rem     Date:  2015-01-29
rem   Uasage:  implementation defined
rem  Example:
rem            cd \sdcard\Download\BASIC\bwbasic3\
rem            BCCDOS16.BAT
rem
rem This is the location of BCC.EXE
rem
set BINDIR=C:\DOS\BC31\BIN
rem
rem Cleanup before compile
rem
DEL BW*.EXE 1> NUL 2> NUL 
DEL BW*.OBJ 1> NUL 2> NUL 
DEL BW*.MAP 1> NUL 2> NUL 
rem
rem Compile
rem
set OLDPATH=%PATH%
set PATH=%BINDIR%;%PATH%
bcc.exe                                            > BCCDOS16.TXT
bcc.exe -mh -eBWBASIC.EXE -DHAVE_MSDOS=1 -Y bw*.c >> BCCDOS16.TXT
set PATH=%OLDPATH%
set OLDPATH=
set BINDIR=
rem
rem Cleanup after compile
rem
DEL BW*.OBJ 1> NUL 2> NUL 
DEL BW*.MAP 1> NUL 2> NUL 
rem EOF
