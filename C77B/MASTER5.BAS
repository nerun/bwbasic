1     rem This is Master5 the Employee Payroll Menu

%INCLUDE ALL.BAS
500     RESTORE
1025 PRINT clear$:PRINT
1030 PRINT "*** EMPLOYEE PAYROLL SYSTEM MENU ***":PRINT
2000 PRINT "1 - UPDATE EMPLOYEE RECORDS"
2010 PRINT
2020 PRINT "2 - ENTER EMPLOYEE TIME CARDS"
2030 PRINT
2040 PRINT "3 - CALCULATE EMPLOYEE PAYROLL"
2050 PRINT
2060 PRINT "4 - PRINT PAYROLL SUMMARY"
2070 PRINT
2080 PRINT "5 - PRINT EMPLOYEE CHECKS"
2090 PRINT
2100 PRINT "6 - POST PAYROLL OPERATIONS"
2110 PRINT
2120 PRINT "7 - RECORD FEDERAL DEPOSITS (501)"
2130 PRINT
2140 PRINT "8 - PRINT FEDERAL AND STATE TAX FORMS (941/W2, ETC)"
2150 PRINT
2160 PRINT "9 - FINISHED"
3000 PRINT
3010 INPUT Z
3020 IF Z<1 OR Z>9 THEN 1025
4000 ON Z GOTO 9000,9050,9100,9150,9200,9250,9300,9350,9400
9000	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EPENTRY"
9050	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "TMENTRY"
9100	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EPJOTRAN"
9150	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EPSUMARY"
9200	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EPCHECKS"
9250	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EPJOPOST"
9300	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "EPDPOSIT"
9350	print clear$:print
	print "Install appropriate Employee Information disk in Drive B:"
	print:input "Type return to continue.";line temp$:initialize
	chain "MASTER14"
9400 print clear$:print:print "Replace disk #5 in drive A."
     print "Type return to continue."
     input line temp$
     initialize
     chain "bizmii"
9800 GOTO 1025
