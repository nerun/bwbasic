REM PURPOSE:  Verify function overloading, variant functions, and related items.
REM AUTHOR: Howard Wulf, AF5NE
REM DATE: 2015-01-23
REM 
REM ----------------------------------------------------------------------------------------------
REM 
REM Verify function overloading with mqany parameters.
REM    Functions are selected based upon the number of parameters and the type of each parameter.
REM Verify variant function can co-exist with normal functions.
REM    Variant functions are declared using (...) instead of typical variable names.
REM Verify variant is only called when there is not a matching normal function.
REM    Variant functions are NOT called is there is a matching normal function.
REM Verify ARGC, ARGT, ARGV, and ARGV$ functions.
REM    These are used to extract the parameter information for variant functions.
REM 
REM ----------------------------------------------------------------------------------------------
REM 
function tom( ... )
    REM This is a variant function.  The BASIC program must determine:
    REM     the number of parameters (ARGC)
    REM     the type of each parameter (ARGT$)
    REM     the value of each parameter (using ARGV or ARGV$ depending upon ARGT)
    REM 
    print "VARIANT"
    let N = ARGC
    print "ARGC=";N
    FOR I = 1 TO N
        LET T$ = ARGT$( I )
        PRINT "PARAMETER";I;"TYPE ";T$;" ";
        if T$ = "$" THEN
            let A$ = ARGV$( I )
            PRINT "IS A STRING WITH THE VALUE ";A$
        elseif T$ = "#"
            let N = ARGV( I )
            PRINT "IS A NUMBER WITH THE VALUE ";N
        else
            print "*** ERROR - INVALID PARAMETER TYPE ***"
        end if
    NEXT I
    tom = 99
end function
REM 
REM ----------------------------------------------------------------------------------------------
REM 
function tom( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z )
    print "26 numeric"
    print "ARGC=";ARGC
    print "a=";a
    print "z=";z
    tom = a + z
end function
REM 
REM ----------------------------------------------------------------------------------------------
REM 
function tom( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, a27, a28, a29, a30, a31, a32 )
    print "32 numeric"
    print "ARGC=";ARGC
    print "a=";a
    print "a32=";a32
    tom = a + a32
end function
REM 
REM ----------------------------------------------------------------------------------------------
REM 
function tom( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z$ )
    print "26 mixed"
    print "ARGC=";ARGC
    print "a=";a
    print "z$=";z$
    tom = a + y
end function
REM 
REM ----------------------------------------------------------------------------------------------
REM 
function tom( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, a27, a28, a29, a30, a31, a32$ )
    print "32 numeric"
    print "ARGC=";ARGC
    print "a=";a
    print "a32$=";a32$
    tom = a + a31
end function
REM 
REM ----------------------------------------------------------------------------------------------
REM 
rem 26 number parameters
az = tom( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 )
print "az=";az
rem 26 mixed parameters
az = tom( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,"X" )
print "az=";az
rem 32 nuber parameters
az = tom( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 )
print "az=";az
rem 129 mixed parameters
az = tom( "zaa", 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,"xyz",22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,"anything",28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , "abcdefg" )
print "az=";az
rem 641 mixed parameters
az = tom( 1, 2, 3, 4, 5, "abc", 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,"jkl",21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 ,"abc" )
print "az=";az
rem 32 mixed parameters
az = tom( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,"A" )
print "az=";az
rem 4 number parameters
az = tom( 1,2,3,4 )
print "az=";az
rem 4 string parameters
az = tom( "a","b","c","d" )
print "az=";az
rem 32 mixed parameters
az = tom( 1, 2, 3, 4, 5, 6, 7, 8, 9, "a",11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,"z" )
print "az=";az
rem 0 parameters
az = tom( )
print "az=";az
rem 0 parameters
az = tom
print "az=";az
rem 
    print "not in a FUNCTION or SUB, should be -1"
    print "ARGC=";ARGC
rem 
end
