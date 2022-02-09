100 rem Purpose: Verify GOTO X OF line,...
110 rem  Author: Howard Wulf, AF5NE
120 rem    Date: 2015-05-22
130 rem
140 let x=5
150 goto X of 1100,1200,1300,1400,1500
160 print "GOTO X OF FAILED"
170 stop
1100 print "1"
1110 goto 9999
1200 print "2"
1210 goto 9999
1300 print "3"
1310 goto 9999
1400 print "4"
1410 goto 9999
1500 print "5"
1510 goto 9999
9999 end
