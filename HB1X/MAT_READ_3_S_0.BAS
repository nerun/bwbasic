100 rem Purpose: Verify MAT READ
110 rem  Author: Howard Wulf, AF5NE
120 rem    Date: 2015-05-20
130 rem
140 option base 0
150 data 111,112,113,114
151 data 121,122,123,124
152 data 131,132,133,134
153 data 211,212,213,214
154 data 221,222,223,224
155 data 231,232,233,234
170 dim C$(1,2,3)
180 mat read C$
190 mat print C$
999 end
