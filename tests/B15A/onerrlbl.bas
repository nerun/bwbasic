1 OPTION ERROR GOSUB
1000 rem onerrlbl.bas -- test bwBASIC ON ERROR GOSUB statement with label
1010 print "Test bwBASIC ON ERROR GOSUB statement"
1020 on error gosub handler
1030 print "The next line will include an error"
1040 READ d$ ' if d$ = 78.98 then print "This should not print"
1050 print "This is the line after the error"
1060 end
1070 handler:
1080 print "This is the error handler"
1090 print "The error number is ";err
1100 print "The error line   is ";erl
1110 return
