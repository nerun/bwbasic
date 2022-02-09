rem this is the name of the patch file
set bwbname=bwb1

rem rm -fr bwbasic
rem md bwbasic
rem cd bwbasic
rem unzip ..\%bwbname%
rem chmod -R 666 *
call zipall
call bwbasic3
call subjobs
