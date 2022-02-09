rem Filename:  BC31.BAT
rem  Purpose:  Build Bywater BASIC for MSDOS (8086) using Borland C++ 3.1
rem   Author:  Howard Wulf, AF5NE
rem     Date:  2015-01-29
rem   Uasage:  implementation defined
rem  Example:
rem            cd \sdcard\Download\BASIC\bwbasic3\
rem            BC31.BAT
rem
bcc.exe -mh -ebwbasic.exe -D_DOS bw*.c
rem EOF
