100 rem MAT PRINT
110 rem Howard Wulf, AF5NE
120 rem 2015-05-19
130 rem
140 option base 0
150 dim C(5,2,3)
160 for I=0 to 5
170 for J=0 to 2
180 for K=0 to 3
190 C(I,J,K) = I*100 + J*10 + K
200 next K
210 next J
220 next I
230 mat print C
240 mat print C,
250 mat print C;
999 end
