REM function overloading with mqany parameters
REM Howard Wulf, AF5NE
REM 
function tom( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z )
    print "a=";a
    print "z=";z
    tom = a + z
end function

function tom( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, a27, a28, a29, a30, a31, a32 )
    print "a=";a
    print "a32=";a32
    tom = a + a32
end function

function tom( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z$ )
    print "a=";a
    print "z$=";z$
    tom = a + y
end function

function tom( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, a27, a28, a29, a30, a31, a32$ )
    print "a=";a
    print "a32$=";a32$
    tom = a + a31
end function


rem 
az = tom( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 )
print "az=";az
rem 
az = tom( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,"X" )
print "az=";az
rem 
az = tom( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 )
print "az=";az
rem 
az = tom( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,"A" )
print "az=";az



end
