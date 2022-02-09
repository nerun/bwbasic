1     rem This is Master3 The Accounts Payable/Vendor Menu

%INCLUDE ALL.BAS
1025 PRINT clear$:PRINT
1030 PRINT "*** PURCHASE ORDER ENTRY / VENDOR PAYABLES MENU ***":PRINT
2000 PRINT "1 - ENTER NEW VENDOR INFORMATION"
2005 PRINT
2010 PRINT "2 - ENTER PURCHASE ORDERS"
2015 PRINT
2020 PRINT "3 - LIST PURCHASE ORDERS"
2025 PRINT
2030 PRINT "4 - PRINT PLAIN PAPER (FORMATTED) PURCHASE ORDERS"
2035 PRINT
2040 PRINT "5 - PRINT PRE-PRINTED PURCHASE ORDERS"
2045 PRINT
2050 PRINT "6 - POST PURCHASE ORDERS TO GENERAL LEDGER"
2055 PRINT
2060 PRINT "7 - PRINT AGED ACCOUNTS PAYABLE REPORT"
2065 PRINT
2070 PRINT "8 - PRINT VENDOR LABELS"
2075 PRINT
2080 PRINT "9 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>9 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150,9200,9250,9300,9350,9400
9000	print clear$:print
	print "Install appropriate Vendor Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "VPENTRY"
9050	print clear$:print
	print "Install appropriate Pur Order Register disk in Drive B:"
	print "This file should be on a separate disk from the Vendor"
	print "Information disk."
	print:Input "Type return to continue.";line temp$:initialize
	chain "VPJOTRAN"
9100	print clear$:print
	print "Install appropriate Vendor Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "VPJOLIST"
9150	print clear$:print
	print "Install appropriate Vendor Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "VPFMPO"
9200	print clear$:print
	print "Install appropriate Vendor Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "VPPPPO"
9250 CHAIN "VPJOPOST"
9300	print clear$:print
	print "Install appropriate Vendor Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "VPRPT"
9350	print clear$:print
	print "Install appropriate Vendor Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "VPLABELS"
9400 print clear$:print:print "Replace disk #5 in drive A."
     print "Type return to continue."
     input line temp$
     initialize
     chain "bizmii"
9800 GOTO 1025
