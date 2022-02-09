   10 REM Purpose:  Verify 'CLOSE filename$'
   20 REM  Author:  Howard Wulf, AF5NE
   30 REM    Date:  2015-11-11
   40 REM    Note:  OPTION VERSION SYSTEM-360
   50 REM
   60 REM
  100 REM --- CONSTANT FILE NAME ---
  110 PUT "CLOSE_1.TXT", 1, 2, 3
  120 PUT "CLOSE_1.TXT", "hello world"
  130 REM PUT "CLOSE_1.TXT"
  140 PUT "CLOSE_1.TXT", "ANOTHER","LINE","IS","HERE"
  150 PUT "CLOSE_1.TXT", 1, "abc", 2, "def", 3, "ghi"
  200 REM --- VARIABLE FILE NAME ---
  210 LET F$ = "CLOSE_1.TXT"
  220 PUT F$, 4, 5, 6
  230 PUT F$, "hello world again"
  240 REM PUT F$
  250 PUT F$, "hello world in", f$, "we are", 1
  260 PUT F$, "hello world in", f$, "we are", 2
  270 PUT F$, "hello world in", f$, "we are", 3
  280 PUT F$, "hello world in", f$, "we are", 4
  290 PUT F$, "/*EOF*/"
  300 PRINT "COMPLETED"
 1000 REM
 1010 CLOSE "", "*", "invalid file name", "CLOSE_1.TXT"
 1020 REM 
 1100 REM --- CONSTANT FILE NAME ---
 1110 GET "CLOSE_1.TXT", A, B, C
 1115 PRINT "A=";A, "B=";B, "C=";C
 1120 GET "CLOSE_1.TXT", A$
 1125 PRINT "A$=[";A$;"]"
 1130 REM GET "CLOSE_1.TXT", B$
 1135 REM PRINT "B$=[";B$;"]"
 1140 GET "CLOSE_1.TXT", C1$,C2$,C3$,C4$
 1145 PRINT "C1$=[";C1$;"]", "C2$=[";C2$;"]", "C3$=[";C3$;"]", "C4$=[";C4$;"]"
 1150 GET "CLOSE_1.TXT", d1, d1$, d2, d2$, d3, d3$
 1155 PRINT "D1=";D1;"D1$=[";D1$;"]"   ,  "D2=";D2;"D2$=[";D2$;"]"  ,  "D3=";D3;"D3$=[";D3$;"]"
 1200 REM --- VARIABLE FILE NAME ---
 1210 LET F$ = "CLOSE_1.TXT"
 1220 GET F$, e1, e2, e3
 1225 PRINT "e1=";e1, "e2=";e2, "e3=";e3
 1230 GET F$, G$
 1235 PRINT "G$=[";G$;"]"
 1240 REM GET F$, H$
 1245 REM PRINT "H$=[";H$;"]"
 1250 GET F$, I1$, i2$, i3$, i
 1255 PRINT "i1$=[";i1$;"]", "I2$=[";I2$;"]", "i3$=[";i3$;"]", "i=";I
 1260 GET F$, j1$, j2$, J3$, J
 1265 PRINT "j1$=[";J1$;"]", "J2$=[";J2$;"]", "j3$=[";j3$;"]", "J=";j
 1270 GET F$, j1$, j2$, J3$, J
 1275 PRINT "j1$=[";J1$;"]", "J2$=[";J2$;"]", "j3$=[";j3$;"]", "J=";j
 1280 GET F$, j1$, j2$, J3$, J
 1285 PRINT "j1$=[";J1$;"]", "J2$=[";J2$;"]", "j3$=[";j3$;"]", "J=";j
 1290 GET F$, z$
 1295 PRINT "z$=[";Z$;"]"
 9999 END
