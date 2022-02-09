rem Purpose: convert 6 character (CMS/MVS) debug output to text
rem  Author: Howard Wulf, AF5NE
rem    Date: 2015-04-22
rem   Usage:  implementation defined
rem Example:
rem          ~/bwbasic six2text.bas
rem
line input " six  input file name:", A$
A  = len( A$ )
line input "text output file name:", B$
B  = len( B$)
REM line input " six lookup file name:", C$
C$ = "bwd_six.h"
C  = len( C$ )
D$ = "#define "
D  = len( D$ )
X$ = "X"
X  = len( X$ )
Z$ = ""
Z  = 0
rem Open files
open A$ for input  as #1
open B$ for output as #2
open C$ for input  as #3
rem For Each line in the six debug output
do until eof( #1 )
    line input #1, A$
    rem A$ = "X123456"
    rem print A$
    Z = Z + 1
    print "line #"; Z
    A$ = trim$( A$ )
    A = len( A$ )
    rem default the output
    B$ = A$
    if A = 6 and left$( A$, X ) = X$ then
        rem A$ = "X123456"
        rem Search "bwd_six.h" for a line that looks like "#define ... X123456"
        seek #3, 1
        do until eof( #3 )
            line input #3, C$
            C$ = trim$( C$ )
            C  = len( C$ )
            if left$( C$, D ) = D$ and right$( C$, A ) = A$ then
                rem C$ = "#define ... X123456"
                rem print C$
                B$ = mid$( C$, D, C - D - A )
                B$ = trim$( B$ )
                rem FOUND
                exit do
            end if
        loop
    end if
    print #2, B$
loop
rem
close #1
close #2
close #3
rem
rem Variable Usage
rem
rem A$ Input File Name; Input Text Line
rem A  Length of A$
rem B$ Output File Name; Output Text Line
rem B  Length of B$
rem C$ Lookup file Name; Lookup Text Line
rem C  Length of C$
rem D$ Constant "#define "
rem D  Length of D$
rem X$ Constant "X"
rem X  Length of X$
rem Z$ Not Used
rem Z  Line Count
rem
end
