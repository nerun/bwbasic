  100 rem Purpose: Verify OPEN command
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2016-09-15
  130 rem    Note: OPTION VERSION HEATH
  140 rem
  200 let f = 7
  210 let f$ = "OPEN_1.DAT"
  220 unsave f$
  230 open f$ for write as file #f
  240 print #f, "this is a test"
  250 close #f
  260 rem
  300 let f = 1
  310 open f$ for read as file #f
  320 line input #f, L$
  330 print L$
  340 close #f
  350 unsave f$
  999 end

