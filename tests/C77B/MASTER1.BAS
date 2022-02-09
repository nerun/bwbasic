1     rem This is Master1 the General Ledger/General Journal Menu

%INCLUDE ALL.BAS
500  RESTORE
1020 CONSOLE 
1025 PRINT clear$:PRINT
1030 PRINT "*** GENERAL LEDGER - JOURNAL MENU ***":PRINT
2000 PRINT "1 - ENTER G/L ACCOUNTS - MAKE ADJUSTING ENTRIES" : REM GLENTRY
2010 PRINT
2020 PRINT "2 - LIST THE CHART OF ACCOUNTS"                  : REM GLCHART
2030 PRINT
2040 PRINT "3 - ENTER DISBURSEMENT TRANSACTIONS"             : REM MASTER11
2050 PRINT
2060 PRINT "4 - ENTER RECEIPTS TRANSACTIONS"                 : REM MASTER12
2070 PRINT
2080 PRINT "5 - PRINT TRIAL BALANCE"                         : REM GLTBAL
2090 PRINT
2100 PRINT "6 - PRINT BALANCE SHEET"                         : REM GLBALSHT
2110 PRINT
2120 PRINT "7 - PRINT INCOME STATEMENTS"                     : REM GLINCOME
2130 PRINT
2140 PRINT "8 - PRINT BUDGET ANALYSIS"                       : REM GLBUDGET
2150 PRINT
2160 PRINT "9 - FINISHED"                                    : REM BIZMII
3000 PRINT
	A(9)=0
3010 INPUT Z
3020 IF Z<1 OR Z>9 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150,9200,9250,9300,9350,9400
9000	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLENTRY"
9050	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLCHART"
9100 initialize
	CHAIN "MASTER11"
9150 initialize
	CHAIN "MASTER12"
9200	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLTBAL"
9250	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLBALSHT"
9300	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLINCOME"
9350	print clear$:print
	print "Install appropriate General Ledger disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "GLBUDGET"
9400 print clear$:print:print "Replace program disk #5 in drive A."
     print "Type return to continue."
     input line temp$
     initialize
     chain "bizmii"
9600 GOTO 1025
