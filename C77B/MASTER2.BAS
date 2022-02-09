1     rem This is Master2 The Accounts Receivable/Customer Menu

%INCLUDE ALL.BAS
500     RESTORE
1025 PRINT clear$:PRINT
1030 PRINT "*** SALES ORDER ENTRY / CUSTOMER RECEIVABLES MENU ***"
1035 PRINT
2000 PRINT "1 - ENTER NEW CUSTOMER INFORMATION"
2005 PRINT
2010 PRINT "2 - ENTER CUSTOMER ORDERS"
2015 PRINT
2020 PRINT "3 - LIST CUSTOMER ORDERS"
2025 PRINT
2030 PRINT "4 - PRINT PLAIN PAPER (FORMATTED) INVOICES"
2035 PRINT
2040 PRINT "5 - PRINT PRE-PRINTED INVOICES"
2045 PRINT
2050 PRINT "6 - POST ORDERS TO GENERAL LEDGER"
2055 PRINT
2060 PRINT "7 - PRINT AGED ACCOUNTS RECEIVABLE REPORT"
2065 PRINT
2070 PRINT "8 - PRINT CUSTOMER LABELS"
2075 PRINT
2080 PRINT "9 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>9 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150,9200,9250,9300,9350,9400
9000	print clear$:print
	print "Install appropriate Customer Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "CRENTRY"
9050	print clear$:print
	print "Install appropriate Invoice Register disk in Drive B:"
	print "This file should be on a separate disk from the Customer"
	print "Information disk."
	print:Input "Type return to continue.";line temp$:initialize
	chain "CRJOTRAN"
9100	print clear$:print
	print "Install appropriate Customer Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "CRJOLIST"
9150	print clear$:print
	print "Install appropriate Customer Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "CRFMINV"
9200	print clear$:print
	print "Install appropriate Customer Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "CRPPINV"
9250 CHAIN "CRJOPOST"
9300	print clear$:print
	print "Install appropriate Customer Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "CRRPT"
9350	print clear$:print
	print "Install appropriate Customer Information disk in Drive B:"
	print:Input "Type return to continue.";line temp$:initialize
	chain "CRLABELS"
9400 print clear$:print:print "Replace disk #5 in drive A."
     print "Type return to continue."
     input line temp$
     initialize
     chain "bizmii"
9800 GOTO 1025
