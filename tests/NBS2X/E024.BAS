00100 REM --------------------------------------
00110 REM FILENAME:  do1.bas
00120 REM  PURPOSE:  Verify "DO-LOOP" with "EXIT DO"
00130 REM   AUTHOR:  Howard Wulf AF5NE
00140 REM     DATE:  October 17th, 2014
00150 REM --------------------------------------
01000 REM # do           loop
01010 REM  1 -----------  -----------
01020 REM  2 -----------  until FALSE
01030 REM  3 -----------  until TRUE 
01040 REM  4 -----------  while FALSE
01050 REM  5 -----------  while TRUE 
01060 REM  6 until FALSE  -----------
01070 REM  7 until FALSE  until FALSE
01080 REM  8 until FALSE  until TRUE 
01090 REM  9 until FALSE  while FALSE
01100 REM 10 until FALSE  while TRUE 
01110 REM 11 until TRUE   -----------
01120 REM 12 until TRUE   until FALSE
01130 REM 13 until TRUE   until TRUE 
01140 REM 14 until TRUE   while FALSE
01150 REM 15 until TRUE   while TRUE 
01160 REM 16 while FALSE  -----------
01170 REM 17 while FALSE  until FALSE
01180 REM 18 while FALSE  until TRUE 
01190 REM 19 while FALSE  while FALSE
01200 REM 20 while FALSE  while TRUE 
01210 REM 21 while TRUE   -----------
01220 REM 22 while TRUE   until FALSE
01230 REM 23 while TRUE   until TRUE 
01240 REM 24 while TRUE   while FALSE
01250 REM 25 while TRUE   while TRUE 
09000 let TRUE = 1
09010 let FALSE = 0
10100 REM --------------------------------------
10105 PRINT "TEST #1"
10110 let n = 0
10120 do
10125 let n = n + 1
10130 print "Loop #"; n
10135 exit do
10140 goto TestFailed
10145 loop
10150 if n <> 1 then goto TestFailed
10155 print "*** TEST #1 PASSED ***"
10200 REM --------------------------------------
10205 PRINT "TEST #2"
10210 let n = 0
10220 do
10225 let n = n + 1
10230 print "Loop #"; n
10235 exit do
10240 goto TestFailed
10245 loop until FALSE
10250 if n <> 1 then goto TestFailed
10255 print "*** TEST #2 PASSED ***"
10300 REM --------------------------------------
10305 PRINT "TEST #3"
10310 let n = 0
10320 do
10325 let n = n + 1
10330 print "Loop #"; n
10335 exit do
10340 goto TestFailed
10345 loop until TRUE
10350 if n <> 1 then goto TestFailed
10355 print "*** TEST #3 PASSED ***"
10400 REM --------------------------------------
10405 PRINT "TEST #4"
10410 let n = 0
10420 do
10425 let n = n + 1
10430 print "Loop #"; n
10435 exit do
10440 goto TestFailed
10445 loop while FALSE
10450 if n <> 1 then goto TestFailed
10455 print "*** TEST #4 PASSED ***"
10500 REM --------------------------------------
10505 PRINT "TEST #5"
10510 let n = 0
10520 do
10525 let n = n + 1
10530 print "Loop #"; n
10535 exit do
10540 goto TestFailed
10545 loop while TRUE
10550 if n <> 1 then goto TestFailed
10555 print "*** TEST #5 PASSED ***"
10600 REM --------------------------------------
10605 PRINT "TEST #6"
10610 let n = 0
10620 do until FALSE
10625 let n = n + 1
10630 print "Loop #"; n
10635 exit do
10640 goto TestFailed
10645 loop
10650 if n <> 1 then goto TestFailed
10655 print "*** TEST #6 PASSED ***"
10700 REM --------------------------------------
10705 PRINT "TEST #7"
10710 let n = 0
10720 do until FALSE
10725 let n = n + 1
10730 print "Loop #"; n
10735 exit do
10740 goto TestFailed
10745 loop until FALSE
10750 if n <> 1 then goto TestFailed
10755 print "*** TEST #7 PASSED ***"
10800 REM --------------------------------------
10805 PRINT "TEST #8"
10810 let n = 0
10820 do until FALSE
10825 let n = n + 1
10830 print "Loop #"; n
10835 exit do
10840 goto TestFailed
10845 loop until TRUE
10850 if n <> 1 then goto TestFailed
10855 print "*** TEST #8 PASSED ***"
10900 REM --------------------------------------
10905 PRINT "TEST #9"
10910 let n = 0
10920 do until FALSE
10925 let n = n + 1
10930 print "Loop #"; n
10935 exit do
10940 goto  TestFailed
10945 loop while FALSE
10950 if n <> 1 then goto TestFailed
10955 print "*** TEST #9 PASSED ***"
11000 REM --------------------------------------
11005 PRINT "TEST #10"
11010 let n = 0
11020 do until FALSE
11025 let n = n + 1
11030 print "Loop #"; n
11035 exit do
11040 goto TestFailed
11045 loop while TRUE
11050 if n <> 1 then goto TestFailed
11055 print "*** TEST #10 PASSED ***"
11100 REM --------------------------------------
11105 PRINT "TEST #11"
11110 let n = 0
11120 do until TRUE 
11125 let n = n + 1
11130 print "Loop #"; n
11135 exit do
11140 goto TestFailed
11145 loop
11150 if n <> 0 then goto TestFailed
11155 print "*** TEST #11 PASSED ***"
11200 REM --------------------------------------
11205 PRINT "TEST #12"
11210 let n = 0
11220 do until TRUE 
11225 let n = n + 1
11230 print "Loop #"; n
11235 exit do
11240 goto TestFailed
11245 loop until FALSE
11250 if n <> 0 then goto TestFailed
11255 print "*** TEST #12 PASSED ***"
11300 REM --------------------------------------
11305 PRINT "TEST #13"
11310 let n = 0
11320 do until TRUE 
11325 let n = n + 1
11330 print "Loop #"; n
11335 exit do
11340 goto TestFailed
11345 loop until TRUE
11350 if n <> 0 then goto TestFailed
11355 print "*** TEST #13 PASSED ***"
11400 REM --------------------------------------
11405 PRINT "TEST #14"
11410 let n = 0
11420 do until TRUE 
11425 let n = n + 1
11430 print "Loop #"; n
11435 exit do
11440 goto TestFailed
11445 loop while FALSE
11450 if n <> 0 then goto TestFailed
11455 print "*** TEST #14 PASSED ***"
11500 REM --------------------------------------
11505 PRINT "TEST #15"
11510 let n = 0
11520 do until TRUE 
11525 let n = n + 1
11530 print "Loop #"; n
11535 exit do
11540 goto TestFailed
11545 loop while TRUE
11550 if n <> 0 then goto TestFailed
11555 print "*** TEST #15 PASSED ***"
11600 REM --------------------------------------
11605 PRINT "TEST #16"
11610 let n = 0
11620 do while FALSE
11625 let n = n + 1
11630 print "Loop #"; n
11635 exit do
11640 goto TestFailed
11645 loop
11650 if n <> 0 then goto TestFailed
11655 print "*** TEST #16 PASSED ***"
11700 REM --------------------------------------
11705 PRINT "TEST #17"
11710 let n = 0
11720 do while FALSE
11725 let n = n + 1
11730 print "Loop #"; n
11735 exit do
11740 goto TestFailed
11745 loop until FALSE
11750 if n <> 0 then goto TestFailed
11755 print "*** TEST #17 PASSED ***"
11800 REM --------------------------------------
11805 PRINT "TEST #18"
11810 let n = 0
11820 do while FALSE
11825 let n = n + 1
11830 print "Loop #"; n
11835 exit do
11840 goto TestFailed
11845 loop until TRUE
11850 if n <> 0 then goto TestFailed
11855 print "*** TEST #18 PASSED ***"
11900 REM --------------------------------------
11905 PRINT "TEST #19"
11910 let n = 0
11920 do while FALSE
11925 let n = n + 1
11930 print "Loop #"; n
11935 exit do
11940 goto TestFailed
11945 loop while FALSE
11950 if n <> 0 then goto TestFailed
11955 print "*** TEST #19 PASSED ***"
12000 REM --------------------------------------
12005 PRINT "TEST #20"
12010 let n = 0
12020 do while FALSE
12025 let n = n + 1
12030 print "Loop #"; n
12035 exit do
12040 goto TestFailed
12045 loop while TRUE
12050 if n <> 0 then goto TestFailed
12055 print "*** TEST #20 PASSED ***"
12100 REM --------------------------------------
12105 PRINT "TEST #21"
12110 let n = 0
12120 do while TRUE 
12125 let n = n + 1
12130 print "Loop #"; n
12135 exit do
12140 goto TestFailed
12145 loop
12150 if n <> 1 then goto TestFailed
12155 print "*** TEST #21 PASSED ***"
12200 REM --------------------------------------
12205 PRINT "TEST #22"
12210 let n = 0
12220 do while TRUE 
12225 let n = n + 1
12230 print "Loop #"; n
12235 exit do
12240 goto TestFailed
12245 loop until FALSE
12250 if n <> 1 then goto TestFailed
12255 print "*** TEST #22 PASSED ***"
12300 REM --------------------------------------
12305 PRINT "TEST #23"
12310 let n = 0
12320 do while TRUE 
12325 let n = n + 1
12330 print "Loop #"; n
12335 exit do
12340 goto TestFailed
12345 loop until TRUE
12350 if n <> 1 then goto TestFailed
12355 print "*** TEST #23 PASSED ***"
12400 REM --------------------------------------
12405 PRINT "TEST #24"
12410 let n = 0
12420 do while TRUE 
12425 let n = n + 1
12430 print "Loop #"; n
12435 exit do
12440 goto TestFailed
12445 loop while FALSE
12450 if n <> 1 then goto TestFailed
12455 print "*** TEST #24 PASSED ***"
12500 REM --------------------------------------
12505 PRINT "TEST #25"
12510 let n = 0
12520 do while TRUE 
12525 let n = n + 1
12530 print "Loop #"; n
12535 exit do
12540 goto TestFailed
12545 loop while TRUE
12550 if n <> 1 then goto TestFailed
12555 print "*** TEST #25 PASSED **
18000 rem ------------------------------------------
18010 TestPassed:
18020 print "*** ALL TESTS PASSED ***"
18030 goto TheEnd
18500 rem ------------------------------------------
18510 TestFailed:
18520 print "*** TEST FAILED ***"
18530 goto TheEnd
19000 rem ------------------------------------------
19010 TheEnd:
19999 end
