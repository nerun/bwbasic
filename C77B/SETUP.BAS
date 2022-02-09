REM Purpose: Create the data files for regression testing
REM  Author: Howard Wulf, AF5NE
REM    Date: 2015-12-30
REM    Note: This is NOT used except for regression testing
REM

create "ap" as 4
print #4;0,0,0,0,0
close 4

create "cgsize" as 1
print #1; 2,20
close 1

create "cg" recl 256 as 1
