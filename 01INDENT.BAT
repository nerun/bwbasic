REM Filename:  00INDENT.BAT
REM  Purpose:  format source code of one file
REM   Author:  Howard Wulf, AF5NE
REM     Date:  2015-01-29
REM   Uasage:  implementation defined
REM  Example:
REM            CD \sdcard\Download\BASIC\bwbasic-2.62\
REM            01INDENT.BAT bwbasic.c
REM
indent.exe %1 -bl -ncdb  -nfc1 -nce -ei
xtabs.exe %1 to TEMP.TMP
del %1
ren TEMP.TMP %1
REM EOF
