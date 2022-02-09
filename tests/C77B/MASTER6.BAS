1     rem This is Master6 the Mailing List Menu

%INCLUDE ALL.BAS
500     RESTORE
1025 PRINT clear$:PRINT
1030 PRINT "*** MAILING LIST MENU ***":PRINT
2000 PRINT "1 - UPDATE MAILING LIST"
2010 PRINT
2020 PRINT "2 - PRINT MAILING LABELS"
2030 PRINT
2040 PRINT "3 - PRINT CUSTOMER LABELS"
2050 PRINT
2060 PRINT "4 - PRINT VENDOR LABELS"
2070 PRINT
2080 PRINT "5 - PRINT EMPLOYEE LABELS"
2090 PRINT
2100 PRINT "6 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>6 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150,9200,9250
9000	print clear$:print
	print "Install appropriate Mailing List disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "MAENTRY"
9050	print clear$:print
	print "Install appropriate Mailing List disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "MALABELS"
9100	print clear$:print
	print "Install appropriate Customer Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "CRLABELS"
9150	print clear$:print
	print "Install appropriate Vendor Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "VPLABELS"
9200	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EPLABELS"
9250 print clear$:print:print "Replace program disk #5 in drive A."
     print "Type return to continue."
     input line temp$
     initialize
     chain "bizmii"
9800 GOTO 1025
