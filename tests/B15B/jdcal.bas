900 REM   JD --> CALENDAR
905 REM
910 INPUT "J,F ";J,F
915 INPUT "JC (0) OR GC (1) ";G
920 F=F+0.5
925 IF F<1 THEN 935
930 F=F-1: J=J+1
935 IF G=1 THEN 945
940 A=J: GOTO 955
945 A1=INT((J/36524.25)-51.12264)
950 A=J+1+A1-INT(A1/4)
955 B=A+1524
960 C=INT((B/365.25)-0.3343)
965 D=INT(365.25*C)
970 E=INT((B-D)/30.61)
975 D=B-D-INT(30.61*E)+F
980 M=E-1: Y=C-4716
985 IF E>13.5 THEN M=M-12
990 IF M<2.5 THEN Y=Y+1
995 PRINT "DATE: ";Y;M;D
997 END
1000 REM  ------------------------
1001 REM  APPEARED IN ASTRONOMICAL
1002 REM  COMPUTING, SKY & TELE-
1003 REM  SCOPE, MAY, 1984
1004 REM  ------------------------