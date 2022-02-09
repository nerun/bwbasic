  100 rem Purpose: Verify DIM ... AS ...
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-06-07
  130 rem
  135 dim Z
  137 print "Iteration";Z
  140 dim A as integer
  150 dim B as long
  160 dim C as single
  170 dim D as double
  180 dim E as string
  190 dim F(20) as integer
  200 dim G(20) as long
  210 dim H(20) as single
  220 dim I(20) as double
  230 dim J(20) as string
  240 let A=Z*1
  250 let B=Z*2
  260 let C=Z*3
  270 let D=Z*4
  280 let E=STR$(Z*5)
  285 vars
  287 maintainer debug on
  290 let F(Z)=Z*1
  300 let G(Z)=Z*2
  310 let H(Z)=Z*3
  320 let I(Z)=Z*4
  330 let J(Z)=STR$(Z*5)
  340 let Z = Z + 1
  350 if Z <= 20 then 100
  360 rem
  999 end

