rem Purpose: compare debug output from different systems
rem  Author: Howard Wulf, AF5NE
rem    Date: 2015-04-22
rem   Usage:  implementation defined
rem Example:
rem          ~/bwbasic difflogs.bas
rem
line input "six   input file name:", A$
line input "text  input file name:", B$
line input "diff output file name:", C$
C = 32
rem
open A$ for input  as #1
open B$ for input  as #2
open C$ for output as #3
rem
do until eof( #1 ) or eof( #2 )
line input #1, A$
line input #2, B$
A$ = trim$( A$ )
B$ = trim$( B$ )
A$ = left$( A$ + space$(C), C )
B$ = left$( B$ + space$(C), C )
C$ = A$ + B$
if A$ <> B$ then
  C$ = C$ + "<--- DIFFERENT"
end if
print #3, C$
loop
rem
close #1
close #2
close #3
rem
end

