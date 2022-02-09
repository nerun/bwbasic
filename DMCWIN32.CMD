@echo off
rem Filename:  DMCWIN32.CMD
rem  Purpose:  Build Bywater BASIC for MS-WINDOWS (32-bit) using Digital Mars Compiler Version 8.42n
rem   Author:  Howard Wulf, AF5NE
rem     Date:  2015-01-29
rem   Uasage:  implementation defined
rem  Example:
rem            cd \sdcard\Download\BASIC\bwbasic3\
rem            DMCWIN32.CMD
rem
rem This is the location of DMC.EXE
rem
set BINDIR=C:\DOS\dm\bin
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
dmc.exe > DMCWIN32.TXT
dmc.exe -mn -A89 -oBWBASIC.EXE -DHAVE_MSDOS=1 bwbasic.c bwb_cmd.c bwb_cnd.c bwb_dio.c bwb_exp.c bwb_fnc.c bwb_inp.c bwb_int.c bwb_prn.c bwb_stc.c bwb_str.c bwb_tbl.c bwb_var.c bwd_cmd.c bwd_fun.c bwx_tty.c >> DMCWIN32.TXT
set PATH=%OLDPATH%
set OLDPATH=
set BINDIR=
rem
rem Cleanup after compile
rem
DEL BW*.OBJ 1> NUL 2> NUL 
DEL BW*.MAP 1> NUL 2> NUL 
rem EOF
