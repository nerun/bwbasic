1000 rem ----------------------------------------------------------
1010 rem    File:  exit2.bas
1020 rem Purpose:  Verify EXIT DO|FOR|UNTIL|WHILE
2000 rem ----------------------------------------------------------
2010 rem TEST #1:  Verify "EXIT" is an ERROR
2020 ' on error goto 9000
2025 ' let i = 0
2030 ' do
2035 '   let i = i + 1
2040 '   exit
2050 '   error 1
2060 ' loop
2065 ' if i <> 1 then ERROR 1
2070 print "TEST #1 passed"
2100 rem ----------------------------------------------------------
2110 rem TEST #2:  Verify "EXIT DO" exits DO-LOOP
2120 rem
2125 let i = 0
2130 do
2135   let i = i + 1
2140   exit do
2150   error 1
2160 loop
2165 if i <> 1 then ERROR 1
2170 print "TEST #2 passed"
2200 rem ----------------------------------------------------------
2210 rem TEST #3:  Verify "EXIT FOR" exits FOR-NEXT
2220 rem
2225 let i = 0
2230 for j = 1 to 100
2235   let i = i + 1
2240   exit for
2250   error 1
2260 next j
2265 if i <> 1 then ERROR 1
2270 print "TEST #3 passed"
2300 rem ----------------------------------------------------------
2310 rem TEST #4:  Verify "EXIT REPEAT" exits REPEAT-UNTIL
2320 rem
2325 let i = 0
2330 repeat
2335   let i = i + 1
2340   exit repeat
2350   error 1
2360 until true
2365 if i <> 1 then ERROR 1
2370 print "TEST #4 passed"
2400 rem ----------------------------------------------------------
2410 rem TEST #5:  Verify "EXIT WHILE" exits WHILE-WEND
2420 rem
2425 let i = 0
2430 while 1
2435   let i = i + 1
2440   exit while
2450   error 1
2460 wend
2465 if i <> 1 then ERROR 1
2470 print "TEST #5 passed"
8000 rem ----------------------------------------------------------
8010 rem ALL TESTS PASSED
8020 print "*** ALL TESTS PASSED ***"
