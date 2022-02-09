 100 rem Purpose: Verify MID$ statement
 110 rem Author: Howard Wulf, AF5NE
 120 rem Date: 2016-03-28
 130 rem
 140 rem MID$( target$, start [, length ] ) = source$
 150 rem
 160 rem -------------------------------------
 170 rem
 180 rem 
 190 on error goto 750
 200 gosub 800
 : mid$( A$, -1, 1 ) = "ab"
 : gosub 900
 : rem start < 0 should cause ERROR 5
 210 gosub 800
 : mid$( A$, 0, 1 ) = "ab"
 : gosub 900
 : rem start = 0 should cause ERROR 5
 220 gosub 800
 : mid$( A$, 1, -1 ) = "ab"
 : gosub 900
 : rem length < 0 should cause ERROR 5
 230 gosub 800
 : mid$( A$, 1, 0 ) = "ab"
 : gosub 900
 : rem length = 0 should have no effect
 240 gosub 800
 : mid$( A$, 2, 0 ) = "ab"
 : gosub 900
 : rem length = 0 should have no effect
 250 gosub 800
 : mid$( A$, LEN(A$), 0 ) = "ab"
 : gosub 900
 : rem start = LEN( target$ ) 
 260 gosub 800
 : mid$( A$, LEN(A$), 1 ) = "ab"
 : gosub 900
 : rem start = LEN( target$ ) 
 280 gosub 800
 : mid$( A$, LEN(A$), 2 ) = "ab"
 : gosub 900
 : rem start = LEN( target$ )
 290 gosub 800
 : mid$( A$, LEN(A$) + 1, 1 ) = "ab"
 : gosub 900
 : rem start > LEN( target$ ) should cause ERROR 5
 300 gosub 800
 : mid$( A$, 1, 2 ) = "ab"
 : gosub 900
 310 gosub 800
 : mid$( A$, 8, 2 ) = "cd"
 : gosub 900
 320 gosub 800
 : mid$( A$, 1, 9 ) = "ab"
 : gosub 900
 330 gosub 800
 : mid$( a$ , 2, 2 ) = "cd"
 : gosub 900
 340 gosub 800
 : mid$( a$, 9, 2 ) = "de"
 : gosub 900
 : rem 9 + 2 > len( A$ )
 350 gosub 800
 : mid$( A$, 10, 2 ) = "ef"
 : gosub 900
 : rem 10 > len( A$ )
 360 gosub 800
 : mid$( A$, 11, 2 ) = "ef"
 : gosub 900
 : rem 11 > len( A$ )
 700 rem -------------------------------------
 710 goto 999
 750 rem -------------------------------------
 760 print "ERR="; ERR, "ERL="; ERL
 770 resume next
 800 rem -------------------------------------
 810 rem Assign default value
 820 let A$ = "123456789"
 830 return
 900 rem -------------------------------------
 910 rem Print resulting value
 920 print "["; A$; "]"
 930 return
 999 end

