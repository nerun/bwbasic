100 rem Purpose: Verify MAT READ
110 rem  Author: Howard Wulf, AF5NE
120 rem    Date: 2015-05-20
130 rem
140 option base 0
150 data zero,one,two,three,four,five
160 dim A$(5)
170 mat read A$
180 mat print A$
190 end
