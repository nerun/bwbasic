1     rem This is Master11 the General Journal Menu

%INCLUDE ALL.BAS
500     RESTORE
1020 CONSOLE 
1025 PRINT clear$:PRINT
1030 PRINT "*** GENERAL JOURNAL TRANSACTION MENU ***":PRINT
2000 PRINT "1 - ENTER GENERAL JOURNAL TRANSACTIONS"  : REM GLJOTRAN
2010 PRINT
2020 PRINT "2 - POST GENERAL JOURNAL TRANSACTIONS"   : REM GLJOPOST
2030 PRINT
2040 PRINT "3 - ENTER CHECK DISBURSEMENTS"           : REM GLCDTRAN
2050 PRINT
2060 PRINT "4 - POST CHECK DISBURSEMENTS"            : REM GLCDPOST
2070 PRINT
2120 PRINT "5 - PRINT DISBURSEMENT CHECKS"           : REM GDCHECKS
2130 PRINT
2140 PRINT "6 - FINISHED"                            : REM MASTER1
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>6 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150,9200,9250
9000	print clear$:print
	print "Install appropriate General Journal disk in Drive B:"
	print "This file should be on a separate disk from the General Ledger."
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLJOTRAN"
9050	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLJOPOST"
9100	print clear$:print
	print "Install appropriate Check Disbursements disk in Drive B:"
	print "This file should be on a separate disk from the General Ledger."
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLCDTRAN"
9150	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLCDPOST"
9200	print clear$:print
	print "Install appropriate Check Disbursements disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GDCHECKS"
9250 initialize
	CHAIN "MASTER1"
9600 GOTO 1025
