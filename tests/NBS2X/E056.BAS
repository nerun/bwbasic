REM PURPOSE: verify user function parameters are range checked
REM  AUTHOR: Howard Wulf, AF5NE
REM    DATE: 2014-01-26
REM 
REM Here we replace the functions  not implemented on this platform
REM   INP(), OUT(), WAIT(), PEEK(), POKE()
REM
REM These should succeed without any errors
let a = INP( 1 )
OUT 2, 3
let b = PEEK( 4 )
POKE 5, 6

print "a=";a
print "b=";b

REM These should cause ERROR 5 ' Illegal Function Call
on error goto hell

REM TEST 1
let T = 1
BeforeTest1:
let a = INP( MAXINT + 100000 )
AfterTest1:

REM TEST 2
let T = 2
BeforeTest2:
let a = INP( MININT - 100000 )
AfterTest2:

REM TEST 3
let T = 3
BeforeTest3:
let X3 = MAXINT + 100000
OUT X3, 0
AfterTest3:

REM TEST 4
let T = 4
BeforeTest4:
let Y4 = MAXINT + 100000
OUT 0, Y4
AfterTest4:

print "*** ALL TESTS PASSED ***"

goto 999


hell:
if ERR = 5 then
  select case T
  case 1
    if ERL > BeforeTest1 and ERL < AfterTest1 then
      REM This is where we expect the ERROR to occur
      print "*** TEST 1 PASSED ***"
    else
      print "*** TEST 1 FAILED ***"
      stop
    end if
  case 2
    if ERL > BeforeTest2 and ERL < AfterTest2 then
      REM This is where we expect the ERROR to occur
      print "*** TEST 2 PASSED ***"
    else
      print "*** TEST 2 FAILED ***"
      stop
    end if
  case 3
    if ERL > BeforeTest3 and ERL < AfterTest3 then
      REM This is where we expect the ERROR to occur
      print "*** TEST 3 PASSED ***"
    else
      print "*** TEST 3 FAILED ***"
      stop
    end if
  case 4
    if ERL > BeforeTest4 and ERL < AfterTest4 then
      REM This is where we expect the ERROR to occur
      print "*** TEST 4 PASSED ***"
    else
      print "*** TEST 4 FAILED ***"
      stop
    end if
  case else
    print "Unexpected TEST code:"; T
    print "*** TEST FAILED ***"
    stop
  end select
  resume next
end if
print "Unexpected ERROR code:"; ERR
print "*** TEST FAILED ***"
stop



REM ---------------------------------------------------------------
REM Direct hardware access is only supported on CP/M-86 derivatives
REM ---------------------------------------------------------------

function INP( X% )
    print "Fake INP"
    print "X=";X%
    INP = 77
end function
sub OUT( X%, Y% )
    print "Fake OUT"
    print "X=";X%
    print "Y=";Y%
end sub
function PEEK( X% )
    print "Fake PEEK"
    print "X=";X%
    PEEK = 88
end function
sub POKE( X%, Y% )
    print "Fake POKE"
    print "X=";X%
    print "Y=";Y%
end sub
stop

999 end
