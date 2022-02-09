010 rem verify MID$ command
100 let a$ = "123456789"
110 mid$( a$, 3, 4 ) = "abcd"
120 print a$
130 if a$ = "12abcd789" then 
140   PRINT "*** TEST PASSED ***"
150 else
160   PRINT "*** TEST PASSED ***"
170 end if
999 END
