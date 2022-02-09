100 rem MAT PRINT
110 rem Howard Wulf, AF5NE
120 rem 2015-05-19
130 rem
140 option base 0
150 dim B$(5,2)
160 for I=0 to 5
170 for J=0 to 2
180 B$(I,J)=STR$(I*10 + J)
190 next J
200 next I
210 mat print B$
220 mat print B$,
230 mat print B$;
999 end
