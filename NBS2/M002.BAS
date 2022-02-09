010 rem verify CONT command
020 rem this must be executed manually
090 let ITERATION = ITERATION + 1
100 print "100. Enter CONT at the command line.  Next line should be 120"
110 stop
120 print "120. Enter CONT at the command line.  Next line should be 140"
130 end
140 print "140. Enter CONT at the command line.  Next line should be 100"
900 REM Quit when we have executed this complete program 3 times
910 if ITERATION > 2 then 
920   QUIT
930 end if
