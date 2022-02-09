100 rem Purpose: Verify GOSUB-OF
110 rem  Author: Howard Wulf, AF5NE
120 rem    Date: 2015-05-22
130 rem
135 print "ENTER A NUMBER (-999 TO END)";
140 input A
145 if a=-999 then 9999
150 gosub A of 1100,1200,1300,1400,1500
160 goto 135
1100 print "1"
1110 return
1200 print "2"
1210 return
1300 print "3"
1310 return
1400 print "4"
1410 return
1500 print "5"
1510 return
9999 end
