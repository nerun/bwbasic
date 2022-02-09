5 PRINT CHR$(27);CHR$(42)
10 REM Audiolab Program
20 PRINT TAB(20);"-=*AUDIO LAB PROGRAM*=-"
25 REM---------Menu---------
30 PRINT"Select the program you require from the following menu"
50 PRINT TAB(10);"(0) Exit from program. (Changed my mind)"
70 PRINT TAB(10);"(1) Calculate power, given voltage and impedance"
90 PRINT TAB(10);"(2) Calculate voltage, given power and impedance"
110 PRINT TAB(10);"(3) Calculate the dB ratio of two voltages"
130 PRINT TAB(10);"(4) Calculate the dB ratio of two power values"
150 PRINT TAB(10);"(5) Change percent (%), to dB"
170 PRINT TAB(10);"(6) Change dB to percent (%)"
190 PRINT TAB(10);"(7) Calculate dBf, given a number in microvolts"
210 PRINT TAB(10);"(8) Calculate microvolts (uV), given dBf"
230 PRINT TAB(10);"(9) Damping factor of an amplifier with 8-ohm loads"
235 PRINT:PRINT:PRINT:PRINT:PRINT:PRINT
240 PRINT"ENTER THE NUMBER OF YOUR CHOICE AND PRESS 'RETURN'"
250 INPUT C
260 IF C=0 GOTO 1430 REM (exit)
270 ON C GOTO 290,420,550,680,810,900,990,1110,1220
280 IF C>9 GOTO 20
285 REM--------Power/voltage
290 PRINT"To calculate POWER enter the VOLTAGE"
295 LPRINT CHR$(&H1F);CHR$(1);
300 LPRINT"To calculate POWER enter the VOLTAGE"
310 INPUT V
320 LPRINT V;" volts"
330 PRINT"Now enter the impedance, in ohms"
340 LPRINT"Now enter the impedance, in ohms"
350 INPUT R
360 LPRINT R;" ohms"
370 LET P=V^2/R
380 PRINT"Power equals> ";P;"< watts"
390 LPRINT"Power equals> ";P;"< WATTS"
400 LPRINT
401 PRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)":INPUT ANS$
402 LPRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)"
405 IF ANS$="Y" THEN 290
407 LPRINT ANS$
408 PRINT
410 GOTO 1370 REM (again? or exit program)
420 PRINT"To calculate VOLTAGE,for a given power, enter POWER, in watts"
430 LPRINT"To calculate VOLTAGE, for a given power, enter POWER, in watts"
440 INPUT P
450 LPRINT P;" watts"
460 PRINT"Now enter the load impedance, in ohms"
470 LPRINT"Now enter the load impedance, in ohms"
480 INPUT Z
490 LPRINT Z;" ohms"
500 LET V=SQR(P*Z)
510 PRINT"Voltage for that power equals> ";V;"< volts"
520 LPRINT"Voltage for that power equals> ";V;"< volts"
530 LPRINT
531 PRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)":INPUT ANS$
532 LPRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)"
533 IF ANS$="Y" THEN 420
538 LPRINT ANS$
539 PRINT
540 GOTO 1370
545 REM--------dB/voltage ratios--------
550 PRINT"To calculate the dB ratio of two voltages, enter larger voltage"
560 LPRINT"To calculate the dB ratio of two voltages, enter larger voltage"
570 INPUT V1
580 LPRINT V1;" volts"
590 PRINT"Now enter the smaller voltage"
600 LPRINT"Now enter the smaller voltage"
610 INPUT V2
620 LPRINT V2;" volts"
630 LET DB=20*LOG(V1/V2)/LOG(10)
640 PRINT"The voltage ratio, in decibels, is> ";DB;"< dB"
650 LPRINT"The voltage ratio, in decibels, is> ";DB;"< dB"
660 LPRINT
661 PRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)":INPUT ANS$
662 LPRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)
663 IF ANS$="Y" THEN 550
664 LPRINT ANS$
667 PRINT
670 GOTO 1370
675 REM--------dB/power ratios---------
680 PRINT"To calculate the dB ratio of two powers, enter larger power"
690 LPRINT"To calculate the dB ratio of two powers, enter larger power"
700 INPUT P1
710 LPRINT P1;" watts"
720 PRINT"Now enter the smaller power value"
730 LPRINT"Now enter the smaller power value"
740 INPUT P2
750 LPRINT P2;" watts
760 LET DB=10*LOG(P1/P2)/LOG(10)
770 PRINT"The power ratio, in dB, is> ";DB;"< dB"
780 LPRINT"The power ratio, in dB, is> ";DB;"< dB"
790 LPRINT
791 PRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)":INPUT ANS$
792 LPRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)"
793 IF ANS$="Y" THEN 680
794 LPRINT ANS$
797 PRINT
800 GOTO 1370
805 REM--------dB/percent--------
810 PRINT"To change percent to dB, enter the percentage (numerals only)"
820 LPRINT"To change percent to dB, enter the percentage (numerals only)"
830 INPUT P
840 LPRINT P;" %"
850 LET DB=20*LOG(1/(P/100))/LOG(10)
860 PRINT"The equivalent decibel ratio is> ";DB;"< dB"
870 LPRINT"The equivalent decibel ratio is> ";DB;"< dB"
880 LPRINT
881 PRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)":INPUT ANS$
882 LPRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)"
883 IF ANS$="Y" THEN 810
884 LPRINT ANS$
887 PRINT
890 GOTO 1370
900 PRINT"To change dB to percent (%), enter the value in dB"
910 LPRINT"To change dB to percent (%), enter the value in dB"
920 INPUT DB
930 LPRINT DB;" dB
940 LET P=100*10^(-DB/20)
950 PRINT"The equivalent percentage is> ";P;"< %"
960 LPRINT"The equivalent percentage is> ";P;"< %"
970 LPRINT
971 PRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)":INPUT ANS$
972 LPRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)"
973 IF ANS$="Y" THEN 900
974 LPRINT ANS$
977 PRINT
980 GOTO 1370
985 REM--------uV/dBf-------
990 PRINT"To convert microvolts (uV), to dBf, enter the value in uV"
1000 LPRINT"To convert microvolts (uV), to dBf, enter the value in uV"
1010 INPUT UV
1020 LPRINT UV;" uV"
1030 LET DB=20*LOG(UV/.55)/LOG(10)
1040 PRINT"For a 300-ohm input, the dBf value is> ";DB;"< dBf"
1050 LPRINT"For a 300-ohm input, the dBf, value is> ";DB;"< dBf"
1060 PRINT
1070 PRINT"For a 75-ohm input, the dBf value is> ";DB+6;"< dBf"
1080 LPRINT"For a 75-ohm input, the dBf value is> ";DB+6;"< dBf"
1090 LPRINT
1091 PRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)":INPUT ANS$
1092 LPRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)"
1093 IF ANS$="Y" THEN 990
1094 LPRINT ANS$
1097 PRINT
1100 GOTO 1370
1110 PRINT"To convert dBf to microvolts, (uV), enter the number of dBf"
1120 LPRINT"To convert dBf to microvolts, (uV), enter the number of dBf"
1130 INPUT DB
1140 LPRINT DB;" dBf"
1150 LET UV=10^(DB/20)*.55
1160 PRINT"For a 300-ohm input, the microvolt value is> ";UV;"< uV"
1170 LPRINT"For a 300-ohm input, the microvolt value is> ";UV;"< uV"
1180 PRINT
1190 PRINT"For a 75-ohm input, the microvolt value is> ";UV/2;"< uV"
1200 LPRINT"For a 75-ohm input, the microvolt value is> ";UV/2;"< uV"
1203 LPRINT
1205 PRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)":INPUT ANS$
1206 LPRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)"
1207 IF ANS$="Y" THEN 1110
1208 LPRINT ANS$
1209 PRINT
1210 GOTO 1370
1215 REM--------Damping factor--------
1220 PRINT"To calculate the damping factor of an amplifier working into"
1230 LPRINT"To calculate the damping factor of an amplifier working into"
1240 PRINT"8-ohm loads, ENTER THE VOLTAGE UNDER NO-LOAD CONDITIONS"
1250 LPRINT"8-ohm loads, ENTER THE VOLTAGE UNDER NO-LOAD CONDITIONS"
1260 INPUT V1
1270 LPRINT V1;" no-load volts"
1280 PRINT"Now ENTER VOLTAGE WHEN LOAD IS APPLIED (should be lower value)"
1290 LPRINT"Now ENTER VOLTAGE WHEN LOAD IS APPLIED (should be lower value)"
1300 INPUT V2
1310 LPRINT V2;" volts, under load"
1320 LET D=V2/(V1-V2)
1330 PRINT"The damping factor for the amplifier is> ";D;"< ."
1340 LPRINT"The damping factor for the amplifier is> ";D;"< "
1350 LPRINT
1355 PRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)":INPUT ANS$
1356 LPRINT"DO YOU WISH TO DO THIS CALCULATION AGAIN? (Y/N)"
1357 IF ANS$="Y" THEN 1220
1358 LPRINT ANS$
1359 PRINT
1360 GOTO 1370
1370 PRINT
1375 REM-------reselect/exit----------
1380 PRINT"Do you wish to return to the menu? (Y/N)"
1390 LPRINT"Do you wish to return to the menu? (Y/N)"
1400 INPUT A$
1410 LPRINT A$
1420 IF A$="Y" GOTO 5
1430 PRINT"Happy to have been of service!! SEE YOU LATER!!"
1440 LPRINT"Happy to have been of service!! SEE YOU LATER!!"
1450 END
