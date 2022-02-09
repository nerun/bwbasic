1     rem This is Master16 the Fixed Asset Accounting Menu

%INCLUDE ALL.BAS
500     RESTORE
1025 PRINT clear$:PRINT
1030 PRINT "*** FIXED ASSET ACCOUNTING MENU ***":PRINT
2000 PRINT "1 - ENTER OR MODIFY CAPITAL GOODS"
2010 PRINT
2020 PRINT "2 - CALCULATE YEARS DEPRECIATION"
2030 PRINT
2040 PRINT "3 - PRINT DEPRECIATION REPORT"
2050 PRINT
2060 PRINT "4 - PRINT IRS FORM 4562 - DEPRECIATION"
2070 PRINT
2080 PRINT "5 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>5 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150,9200
9000	print clear$:print
	print "Install appropriate Capital Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "CGENTRY"
9050	print clear$:print
	print "Install appropriate Capital Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "CGDEPCAL"
9100	print clear$:print
	print "Install appropriate Capital Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "CGRPT"
9150	print clear$:print
	print "Install appropriate Capital Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "CGFM4562"
9200 	initialize
	CHAIN "MASTER4"
9800 GOTO 1025
