100 rem Purpose: Verify MAT READ
110 rem  Author: Howard Wulf, AF5NE
120 rem    Date: 2015-05-20
130 rem
140 option base 0
150 data "0,0","0,1","1,0","1,1","2,0","2,1","3,0","3,1","4,0","4,1","5,0","5,1"
170 dim B$(5,1)
180 mat read B$
190 mat print B$
999 end
