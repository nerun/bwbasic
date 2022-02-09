1     rem This is Master4 the Inventory Menu

%INCLUDE ALL.BAS
500     RESTORE
1025 PRINT clear$:PRINT
1030 PRINT "*** FINISHED / CAPITAL / RAW GOODS INVENTORY MENU ***":PRINT
2000 PRINT "1 - UPDATE FINISHED GOODS INVENTORY"
2010 PRINT
2020 PRINT "2 - UPDATE RAW GOODS INVENTORY"
2030 PRINT
2040 PRINT "3 - PRINT FINISHED GOODS REPORT"
2050 PRINT
2060 PRINT "4 - PRINT RAW GOODS REPORT"
2070 PRINT
2080 PRINT "5 - PRINT FINISHED GOODS ALERT"
2090 PRINT
2100 PRINT "6 - PRINT RAW GOODS ALERT"
2110 PRINT
2120 PRINT "7 - FIXED ASSET OPERATIONS"
2130 PRINT
2140 PRINT "8 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>8 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150,9200,9250,9300,9350
9000	print clear$:print
	print "Install appropriate Finished Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "FGENTRY"
9050	print clear$:print
	print "Install appropriate Raw Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "RGENTRY"
9100	print clear$:print
	print "Install appropriate Finished Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "FGRPT"
9150	print clear$:print
	print "Install appropriate Raw Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "RGRPT"
9200	print clear$:print
	print "Install appropriate Finished Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "FGALERT"
9250	print clear$:print
	print "Install appropriate Raw Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "RGALERT"
9300	print clear$:print
	print "Install appropriate Capital Goods disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "MASTER16"
9350 print clear$:print:print "Replace program disk #5 in drive A."
     print "Type return to continue."
     input line temp$
     initialize
     chain "bizmii"
9800 GOTO 1025
