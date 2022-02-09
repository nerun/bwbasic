1     rem This is Master7 the Initialization Routines Menu

%INCLUDE ALL.BAS
500     RESTORE
1025 PRINT clear$:PRINT:PRINT
2000 PRINT "1 - CHANGE REPORT DATES"
2005 PRINT
2010 PRINT "2 - CHANGE DOLLAR VALUE OR PERCENTAGE FORMATS"
2015 PRINT
2020 PRINT "3 - CHANGE PAGE WIDTH, TABS OR COMPANY NAME IN USE TAG"
2025 PRINT
2030 PRINT "4 - CHANGE CHART OF ACCTS HEADINGS AND SUBHEADS"
2035 PRINT
2040 PRINT "5 - ADD NEW COMPANY NAMES"
2045 PRINT
2050 PRINT "6 - CHANGE OR LIST FEDERAL TAX TABLES"
2060 PRINT
2070 PRINT "7 - CHANGE OR LIST STATE TAX TABLES"
2080 PRINT
2090 PRINT "8 - CHANGE CHECK TABS AND PAYROLL CUTOFFS"
2100 PRINT
2110 PRINT "9 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>9 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150,9200,9250,9300,9350,9400
9000	print clear$:print
	print "Install appropriate disk in Drive B:"
	print:print "The date file must be on all file disks!!"
	print:input "Type return to continue.";line temp$:initialize
	chain "DAENTRY"
9050	print clear$:print
	print "Install appropriate disk in Drive B:"
	print:print "The format file must be on all file disks!!"
	print:input "Type return to continue.";line temp$:initialize
	chain "FMTENTRY"
9100	print clear$:print
	print "Install appropriate disk in Drive B:"
	print:print "The tabs file must be on all file disks!!"
	print:input "Type return to continue.";line temp$:initialize
	chain "TAENTRY"
9150 	initialize
	CHAIN "MASTER9"
9200	print clear$:print
	print "Install appropriate disk in Drive B:"
	print:print "The company name file must be on all file disks!!"
	print:input "Type return to continue.";line temp$:initialize
	chain "NAMENTRY"
9250	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EFENTRY"
9300	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "ESENTRY"
9350 	initialize
	CHAIN "MASTER15"
9400 print clear$:print:print "Replace disk #5 in drive A."
     print "Type return to continue."
     input line temp$
     initialize
     chain "bizmii"
9800 GOTO 1025
