1100 rem MAT PRINT
1110 rem Howard Wulf, AF5NE
1120 rem 2015-05-19
1130 rem
1140 option base 0
1150 dim A(5)
1160 for I=0 to 5
1170 A(I)=I*10
1180 next I
1190 mat print A
1200 mat print A,
1210 mat print A;
1999 rem end
2100 rem MAT PRINT
2110 rem Howard Wulf, AF5NE
2120 rem 2015-05-19
2130 rem
2140 rem option base 0
2150 dim B(5,2)
2160 for I=0 to 5
2170 for J=0 to 2
2180 B(I,J)=I*10 + J
2190 next J
2200 next I
2210 mat print B
2220 mat print B,
2230 mat print B;
2999 rem end
3100 rem MAT PRINT
3110 rem Howard Wulf, AF5NE
3120 rem 2015-05-19
3130 rem
3140 rem option base 0
3150 dim C(5,2,3)
3160 for I=0 to 5
3170 for J=0 to 2
3180 for K=0 to 3
3190 C(I,J,K) = I*100 + J*10 + K
3200 next K
3210 next J
3220 next I
3230 mat print C
3240 mat print C,
3250 mat print C;
3999 rem end
4100 rem MAT PRINT
4110 rem Howard Wulf, AF5NE
4120 rem 2015-05-19
4130 rem
4140 mat print A,A,
4150 mat print A;A;
4160 mat print B,B,
4170 mat print B;B;
4180 mat print C,C,
4190 mat print C;C;
4200 mat print A,B,C,
4210 mat print A;B;C;
4999 end
