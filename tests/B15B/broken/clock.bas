200 REM   BEFORE RUNNING THESE LINES, READ THE CLOCK AND OBTAIN THE 
210 REM   UT DATE AND TIME IN THE FORM Y,M,D,H1,M1,S1
220 REM
230 D=D+H1/24+M1/1440+S1/86400
250 G=1: GOSUB 820: V=(J+F)/29.53058868: C=V-INT(V)
260 IF C<0.02 THEN 340  
270 IF C<0.11 THEN PRINT "LAST QUARTER MOON": GOTO 350  
280 IF C<0.27 THEN PRINT "MOON IS A MORNING CRESCENT": GOTO 350  
290 IF C<0.35 THEN PRINT "NEW MOON": GOTO 350  
300 IF C<0.51 THEN PRINT "MOON IS AN EVENING CRESCENT": GOTO 350  
310 IF C<0.61 THEN PRINT "FIRST QUARTER MOON": GOTO 350  
320 IF C<0.77 THEN PRINT "MOON IS WAXING GIBBOUS": GOTO 350  
330 IF C<0.85 THEN PRINT "FULL MOON": GOTO 350  
340 PRINT "MOON IS WANING GIBBOUS"
350 END
867 RETURN
900 REM *************************************************************
901 REM   FROM SKY & TELESCOPE, MAY, 1987, PAGE 536-537.  FOR APPLE
902 REM   COMPUTERS ONLY. MERGE WITH CALJD.BAS (MAY, 1984, PAGE 455).
903 REM *************************************************************
