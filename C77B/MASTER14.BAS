1     rem This is Master14 the Payroll Entry and Calculation Menu

%INCLUDE ALL.BAS
500     RESTORE
1025 PRINT clear$:PRINT
1030 PRINT "*** FEDERAL AND STATE FORMS PRINTING MENU ***":PRINT
2000 PRINT "1 - PRINT 941 FORMS"
2010 PRINT
2020 PRINT "2 - PRINT STATE TAX RETURNS"
2030 PRINT
2040 PRINT "3 - PRINT FEDERAL AND STATE W2 FORMS"
2050 PRINT
2060 PRINT "4 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>4 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150
9000	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "FED941PR"
9050 	print "This program not implemented!"
	CHAIN "MASTER5"
9100	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "W2PRINT"
9150	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "MASTER5"
9800 GOTO 1025
