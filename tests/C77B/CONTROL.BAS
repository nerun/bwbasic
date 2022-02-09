20 rem This is CONTROL.BAS used to set up control characters for
 rem your terminal.

 clear$=chr$(27)+"+" : clear$ = "<CLS>"

 block.size=1024

 ON$=chr$(27)+chr$(41)  : ON$  = "<ON>"  : rem background ON
 OFF$=chr$(27)+chr$(40) : OFF$ = "<OFF>" : rem background OFF

 pagelen=51

 UP$=CHR$(11) : UP$ = "<UP>"
 CUR$=CHR$(27)+CHR$(61)+CHR$(44)+CHR$(32) : CUR$ = "<CUR>"
 GOTO 51
50 CUR2$=CHR$(27)+CHR$(61)+CHR$(CL+32)+CHR$(CC+32): CUR2$ = "<CUR2>" :RETURN
51 REM
