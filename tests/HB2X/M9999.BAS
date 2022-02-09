  100 rem Purpose: Determine maximum recursion
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-10-03
  130 rem
  200 def fna( X )
  210   print "X=";X
  220   let fna = fna( X + 1 )
  230 fnend
  300 let x = fna( 1 )
  999 end

