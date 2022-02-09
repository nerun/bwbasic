100 rem Purpose: Verify MAT READ
110 rem  Author: Howard Wulf, AF5NE
120 rem    Date: 2015-05-20
130 rem
140 option base 0
150 data 00,01,10,11,20,21,30,31,40,41,50,51
170 dim B(5,1)
180 mat read B
190 mat print B
999 end
