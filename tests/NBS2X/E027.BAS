00100 REM -----------------------------------------
00110 REM FILENAME:  exit1.bas
00120 REM  PURPOSE:  Verify EXIT DO/FOR/WHILE
00130 REM   AUTHOR:  Howard Wulf AF5NE
00140 REM     DATE:  October 9th, 2014
01000 REM -----------------------------------------
01010 REM DO
01020 REM -----------------------------------------
01030 print "The following should only print 1,3"
01040 do
01050   print "1"
01060   exit do
01070   print "2" : ERROR 1
01080 loop
01090 print "3"
02000 REM -----------------------------------------
02010 REM DO,FOR
02020 REM -----------------------------------------
02030 print "The following should only print 1,2,5"
02040 do
02050   print "1"
02060   for I = 1 to 100
02070     print "2"
02080     exit do
02090     print "3" : ERROR 1
02100   next I
02110   print "4" : ERROR 1
02120 loop
02130 print "5"
03000 REM -----------------------------------------
03010 REM DO,FOR,WHILE
03020 REM -----------------------------------------
03030 print "The following should only print 1,2,3,7"
03040 do
03050   print "1"
03060   for I = 1 to 100
03070     print "2"
03080     while 1
03090       print "3"
03100       exit do
03110       print "4" : ERROR 1
03120     wend
03130     print "5" : ERROR 1
03140   next I
03150   print "6" : ERROR 1
03160 loop
03170 print "7"
04000 REM -----------------------------------------
04010 REM DO,WHILE
04020 REM -----------------------------------------
04030 print "The following should only print 1,2,5"
04040 do
04050   print "1"
04060   while 1
04070     print "2"
04080     exit do
04090     print "3" : ERROR 1
04100   wend
04110   print "4" : ERROR 1
04120 loop
04130 print "5"
05000 REM -----------------------------------------
05010 REM DO,WHILE,FOR
05020 REM -----------------------------------------
05030 print "The following should only print 1,2,3,7"
05040 do
05050   print "1"
05060   while 1
05070     print "2"
05080     for I = 1 to 100
05090       print "3"
05100       exit do
05110       print "4" : ERROR 1
05120     next I
05130     print "5" : ERROR 1
05140   wend
05150   print "6" : ERROR 1
05160 loop
05170 print "7"
06000 REM -----------------------------------------
06010 REM FOR
06020 REM -----------------------------------------
06030 print "The following should only print 1,3"
06040 for I = 1 to 100
06050   print "1"
06060   exit for
06070   print "2" : ERROR 1
06080 next I
06090 print "3"
07000 REM -----------------------------------------
07010 REM FOR,DO
07020 REM -----------------------------------------
07030 print "The following should only print 1,2,5"
07040 for I = 1 to 100
07050   print "1"
07060   do
07070     print "2"
07080     exit for
07090     print "3" : ERROR 1
07100   loop
07110   print "4" : ERROR 1
07120 next I
07130 print "5"
08000 REM -----------------------------------------
08010 REM FOR,DO,WHILE
08020 REM -----------------------------------------
08030 print "The following should only print 1,2,3,7"
08040 for I = 1 to 100
08050   print "1"
08060   do
08070     print "2"
08080     while 1
08090       print "3"
08100       exit for
08110       print "4" : ERROR 1
08120     wend
08130     print "5" : ERROR 1
08140   loop
08150   print "6" : ERROR 1
08160 next I
08170 print "7"
09000 REM -----------------------------------------
09010 REM FOR,WHILE
09020 REM -----------------------------------------
09030 print "The following should only print 1,2,5"
09040 for I = 1 to 100
09050   print "1"
09060   while 1
09070     print "2"
09080     exit for
09090     print "3" : ERROR 1
09100   wend
09110   print "4" : ERROR 1
09120 next I
09130 print "5"
10000 REM -----------------------------------------
10010 REM FOR,WHILE,DO
10020 REM -----------------------------------------
10030 print "The following should only print 1,2,3,7"
10040 for I = 1 to 100
10050   print "1"
10060   while 1
10070     print "2"
10080     do
10090       print "3"
10100       exit for
10110       print "4" : ERROR 1
10120     loop
10130     print "5" : ERROR 1
10140   wend
10150   print "6" : ERROR 1
10160 next I
10170 print "7"
11000 REM -----------------------------------------
11010 REM WHILE
11020 REM -----------------------------------------
11030 print "The following should only print 1,3"
11040 while 1
11050   print "1"
11060   exit while
11070   print "2" : ERROR 1
11080 wend
11090 print "3"
12000 REM -----------------------------------------
12010 REM WHILE,DO
12020 REM -----------------------------------------
12030 print "The following should only print 1,2,5"
12040 while 1
12050   print "1"
12060   do
12070     print "2"
12080     exit while
12090     print "3" : ERROR 1
12100   loop
12110   print "4" : ERROR 1
12120 wend
12130 print "5"
13000 REM -----------------------------------------
13010 REM WHILE,DO,FOR
13020 REM -----------------------------------------
13030 print "The following should only print 1,2,3,7"
13040 while 1
13050   print "1"
13060   do
13070     print "2"
13080     for I = 1 to 100
13090       print "3"
13100       exit while
13110       print "4" : ERROR 1
13120     next I
13130     print "5" : ERROR 1
13140   loop
13150   print "6" : ERROR 1
13160 wend
13170 print "7"
14000 REM -----------------------------------------
14010 REM WHILE,FOR
14020 REM -----------------------------------------
14030 print "The following should only print 1,2,5"
14040 while 1
14050   print "1"
14060   for I = 1 to 100
14070     print "2"
14080     exit while
14090     print "3" : ERROR 1
14100   next I
14110   print "4" : ERROR 1
14120 wend
14130 print "5"
15000 REM -----------------------------------------
15010 REM WHILE,FOR,DO
15020 REM -----------------------------------------
15030 print "The following should only print 1,2,3,7"
15040 while 1
15050   print "1"
15060   for I = 1 to 100
15070     print "2"
15080     do
15090       print "3"
15100       exit while
15110       print "4" : ERROR 1
15120     loop
15130     print "5" : ERROR 1
15140   next I
15150   print "6" : ERROR 1
15160 wend
15170 print "7"
19900 REM ----------------------------------------------------
19910 REM *** ALL TESTS PASSED ***
19920 REM ----------------------------------------------------
19930 PRINT "*** ALL TESTS PASSED ***"
19999 QUIT
