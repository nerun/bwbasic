  100 REM PURPOSE: VERIFY 'EDIT$' FUNCTION
  110 REM  AUTHOR: HOWARD WULF, AF5NE
  120 REM    DATE: 2017-04-06
  130 REM    NOTE: OPTION VERSION "HAARDT"
  140 REM
  150 REM
  160 REM DATA FORMAT IS: A$, X, E$
  170 REM
  200 REM --------------------
  210 READ N, A$, X, E$
  215 IF N < 0 THEN 999
  217 PRINT "TEST"; N;
  220 LET S$ = EDIT$( A$, X )
  230 IF S$ <> E$ THEN 300
  235 PRINT "OK"
  240 GOTO 200
  300 REM -------------------------
  310 PRINT "FAILED {"; S$; "}"
  320 GOTO 999
  400 REM -------------------
  410 DATA 1, "ABC",   1, "ABC"
  420 DATA 2, "ABC",   2, "ABC"
  430 DATA 3, "ABC",   4, "ABC"
  440 DATA 4, "ABC",   8, "ABC"
  450 DATA 5, "ABC",  16, "ABC"
  460 DATA 6, "ABC",  32, "ABC"
  470 DATA 7, "ABC",  64, "ABC"
  480 DATA 8, "ABC", 128, "ABC"
  490 DATA 9, "ABC", 256, "ABC"
  500 REM -------------------
  510 DATA 11, "   ABC",   1, "   ABC"
  520 DATA 12, "   ABC",   2, "ABC"
  530 DATA 13, "   ABC",   4, "   ABC"
  540 DATA 14, "   ABC",   8, "ABC"
  550 DATA 15, "   ABC",  16, " ABC"
  560 DATA 16, "   ABC",  32, "   ABC"
  570 DATA 17, "   ABC",  64, "   ABC"
  580 DATA 18, "   ABC", 128, "   ABC"
  590 DATA 19, "   ABC", 256, "   ABC"
  600 REM -------------------
  610 DATA 21, "   ABC   ",   1, "   ABC   "
  620 DATA 22, "   ABC   ",   2, "ABC"
  630 DATA 23, "   ABC   ",   4, "   ABC   "
  640 DATA 24, "   ABC   ",   8, "ABC   "
  650 DATA 25, "   ABC   ",  16, " ABC "
  660 DATA 26, "   ABC   ",  32, "   ABC   "
  670 DATA 27, "   ABC   ",  64, "   ABC   "
  680 DATA 28, "   ABC   ", 128, "   ABC"
  690 DATA 29, "   ABC   ", 256, "   ABC   "
  700 REM -------------------
  710 DATA 31, " '  [  ABC  ]' []  '[]'  ",   1, " '  [  ABC  ]' []  '[]'  "
  720 DATA 32, " '  [  ABC  ]' []  '[]'  ",   2
  730 DATA 33, " '  [  ABC  ]' []  '[]'  ",   4, "   ABC   "
  740 DATA 34, " '  [  ABC  ]' []  '[]'  ",   8, "ABC   "
  750 DATA 35, " '  [  ABC  ]' []  '[]'  ",  16, " ABC "
  760 DATA 36, " '  [  ABC  ]' []  '[]'  ",  32, "   ABC   "
  770 DATA 37, " '  [  ABC  ]' []  '[]'  ",  64, "   ABC   "
  780 DATA 38, " '  [  ABC  ]' []  '[]'  ", 128, "   ABC"
  790 DATA 39, " '  [  ABC  ]' []  '[]'  ", 256, "   ABC   "





















  899 DATA -1, "", 0, ""
  999 END

