  100 rem Purpose: verify OPTION ROUND ...
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-07-01
  130 rem
  140 rem ------------------------------------
  150 option base 1
  160 dim I as integer
  170 dim X as double
  180 width 128
  200 rem ------------------------------------
  210 print "OPTION ROUND COMPARISONS"
  220 print    ,         ,         , "BANK"    , "MATH"    , "TRUNCATE"
  230 print "X", "int(X)", "fix(X)", "cint(X)" , "cint(X)" , "cint(X)"
  240 restore
  250 for I = 1 to 17
  260   read X
  270   print X, int(X), fix(X),
  280   option round BANK
  290   print cint(X),
  300   option round MATH
  310   print cint(X),
  320   option round TRUNCATE
  330   print cint(X)
  340 next I
  900 rem ------------------------------------
  910 data -2,-1.6,-1.5,-1.4,-1,-.6,-.5,-.4
  920 data 0
  930 data .4,.5,.6,1,1.4,1.5,1.6,2
  999 end

