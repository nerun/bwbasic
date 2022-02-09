REM PURPOSE: verify user functions supercedes intrinsic function
REM    NOTE: they must have the same parameter signature
REM  AUTHOR: Howard Wulf, AF5NE
REM    DATE: 2014-01-24
REM 
REM Here we replace the functions  not implemented on this platform
REM   INP(), OUT(), WAIT(), PEEK(), POKE()
REM
let a = INP( 1 )
OUT 2, 3
let b = PEEK( 4 )
POKE 5, 6
WAIT 7, 8
WAIT 9, 10, 11

print "a=";a
print "b=";b
 
goto 999

REM ---------------------------------------------------------------
REM Direct hardware access is only supported on CP/M-86 derivatives
REM ---------------------------------------------------------------

function INP( x )
    print "Fake INP"
    print "x=";x
    INP = 77
end function
sub OUT( X, Y )
    print "Fake OUT"
    print "X=";X
    print "Y=";Y
end sub
sub WAIT( X, Y )
    print "Fake WAIT"
    print "X=";X
    print "Y=";Y
end sub
sub WAIT( X, Y, Z )
    print "Fake WAIT #2"
    print "X=";X
    print "Y=";Y
    print "Z=";Z
end sub
function PEEK( X )
    print "Fake PEEK"
    print "X=";X
    PEEK = 88
end function
sub POKE( X, Y )
    print "Fake POKE"
    print "X=";X
    print "Y=";Y
end sub
stop

999 end
