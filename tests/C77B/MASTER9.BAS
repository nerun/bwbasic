1     rem This is Master9 the General Ledger Heading Menu

%INCLUDE ALL.BAS
500     RESTORE
1025 PRINT clear$:PRINT:PRINT
2000 PRINT "1 - CHANGE GENERAL LEDGER HEADINGS"
2005 PRINT
2010 PRINT "2 - CHANGE GENERAL LEDGER SUBHEADINGS"
2015 PRINT
2020 PRINT "3 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>3 THEN 1025
4000 ON Z GOTO 9000,9050,9100
9000	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "GLHENTRY"
9050	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "GLSENTRY"
9100 	initialize
	CHAIN "MASTER7"
9800 GOTO 1025
