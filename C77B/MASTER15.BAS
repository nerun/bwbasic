1     rem This is Master15 the Check Tabs and Cutoffs Menu

%INCLUDE ALL.BAS
500     RESTORE
1025 PRINT clear$:PRINT
1030 PRINT "*** PAYROLL CHECK TABS AND CUTOFFS MENU ***":PRINT
2000 PRINT "1 - ENTER CHECK PRINTING TABS"
2010 PRINT
2020 PRINT "2 - ENTER PAYROLL CUTOFF FIGURES"
2030 PRINT
2040 PRINT "3 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>3 THEN 1025
4000 ON Z GOTO 9000,9050,9100
9000	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EPTABS"
9050	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EPCUTS"
9100 	initialize
	CHAIN "MASTER7"
9800 GOTO 1025
