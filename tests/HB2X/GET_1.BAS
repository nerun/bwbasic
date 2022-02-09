   10 REM Purpose:  Verify 'GET filename$'
   20 REM  Author:  Howard Wulf, AF5NE
   30 REM    Date:  2015-11-11
   40 REM    Note:  OPTION VERSION SYSTEM-360
   50 REM    Note:  read the values written by PUT_1.BAS
   60 REM
  100 REM --- CONSTANT FILE NAME ---
  110 GET "PUT_1.TXT", A, B, C
  115 PRINT "A=";A, "B=";B, "C=";C
  120 GET "PUT_1.TXT", A$
  125 PRINT "A$=[";A$;"]"
  130 REM GET "PUT_1.TXT", B$
  135 REM PRINT "B$=[";B$;"]"
  140 GET "PUT_1.TXT", C1$,C2$,C3$,C4$
  145 PRINT "C1$=[";C1$;"]", "C2$=[";C2$;"]", "C3$=[";C3$;"]", "C4$=[";C4$;"]"
  150 GET "PUT_1.TXT", d1, d1$, d2, d2$, d3, d3$
  155 PRINT "D1=";D1;"D1$=[";D1$;"]"   ,  "D2=";D2;"D2$=[";D2$;"]"  ,  "D3=";D3;"D3$=[";D3$;"]"
  200 REM --- VARIABLE FILE NAME ---
  210 LET F$ = "PUT_1.TXT"
  220 GET F$, e1, e2, e3
  225 PRINT "e1=";e1, "e2=";e2, "e3=";e3
  230 GET F$, G$
  235 PRINT "G$=[";G$;"]"
  240 REM GET F$, H$
  245 REM PRINT "H$=[";H$;"]"
  250 GET F$, I1$, i2$, i3$, i
  255 PRINT "i1$=[";i1$;"]", "I2$=[";I2$;"]", "i3$=[";i3$;"]", "i=";I
  260 GET F$, j1$, j2$, J3$, J
  265 PRINT "j1$=[";J1$;"]", "J2$=[";J2$;"]", "j3$=[";j3$;"]", "J=";j
  270 GET F$, j1$, j2$, J3$, J
  275 PRINT "j1$=[";J1$;"]", "J2$=[";J2$;"]", "j3$=[";j3$;"]", "J=";j
  280 GET F$, j1$, j2$, J3$, J
  285 PRINT "j1$=[";J1$;"]", "J2$=[";J2$;"]", "j3$=[";j3$;"]", "J=";j
  290 GET F$, z$
  295 PRINT "z$=[";Z$;"]"
  999 END
