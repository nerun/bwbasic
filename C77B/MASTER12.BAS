1     rem This is Master12 the Check-Cash Reciepts-Disbursements Menu

%INCLUDE ALL.BAS
500     RESTORE
1020 CONSOLE  
1025 PRINT clear$:PRINT
1030 PRINT "*** CHECK-CASH RECEIPTS MENU ***":PRINT
2000 PRINT "1 - ENTER CHECK RECEIPTS"               REM:GLCKTRAN
2010 PRINT
2020 PRINT "2 - POST CHECK RECEIPTS JOURNAL"        REM:GLCKPOST
2030 PRINT
2080 PRINT "3 - FINISHED"                           REM:MASTER1
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>3 THEN 1025
4000 ON Z GOTO 9000,9050,9100
9000	print clear$:print
	print "Install appropriate Check Receipts disk in Drive B:"
	print "This file should be on a separate disk from the General Ledger."
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLCKTRAN"
9050	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLCKPOST"
9100 initialize
	CHAIN "MASTER1"
9600 GOTO 1025
