  100 rem Purpose: Verify CIN()
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2016-09-15
  130 rem    Note: OPTION VERSION HEATH
  140 rem
  200 print "Reading [";
  210 open "CIN_1.DAT" for read as file #7
  220 let N = CIN( 7 )
  230 if N < 0 then 300
  240 print chr$( N );
  250 goto 220
  300 print "]"
  310 close #7
  999 end

