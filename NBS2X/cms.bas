rem PUrpose: re-define externals to only 6 characters for CMS
rem  Author: Howard Wulf, AF5NE
rem    Date: 2015-02-10
rem   Usage:  implementation defined
rem Example:
rem          ~/bwbasic cms.bas
rem
let N = 0
let E$ = "extern "
let E = len( E$ )
open "bwbasic.h" for input as #1
open "cms.h" for output as #2
while not eof( #1 )
  line input #1, L$
  L$ = trim$( L$ )
  if left$( L$, E ) = E$ then
    rem extern ....
    while instr( L$, ";" ) = 0
      ' read more text to get semicolon
      line input #1, M$
      M$ = trim$( M$ )
      L$ = L$ + " " + M$
    wend
    rem extern ...;
    L$ = trim$(mid$( L$, E + 1 ))
    ' truncate semicolon
    X = instr( L$, ";" )
    if X > 0 then
      L$ = trim$(left$( L$, X - 1 ))
    end if
    ' truncate parenthesis
    X = instr( L$, "(" )
    if X > 0 then
      L$ = trim$(left$( L$, X - 1 ))
    end if
    ' truncate bracket
    X = instr( L$, "[" )
    if X > 0 then
      L$ = trim$(left$( L$, X - 1 ))
    end if
    ' find last word
    X = instr(L$, " " )
    while X > 0
      L$ = trim$(mid$( L$, X + 1 ))
      X = instr(L$, " " )
    wend
    ' skip astericks
    while left$( L$, 1 ) = "*"
      L$ = trim$(mid$( L$, 2 ))
    wend
    if L$ = "main" then
      ' ignore
    else
      ' pad for alignment
      L$ = L$ + space$(32)
      L$ = left$( L$, 32 )
      H$ = "00000" + hex$(N)
      H$ = right$( H$, 5 )
      print #2, "#define ";L$;" X";H$
      N = N + 1
    end if
  end if
wend
close #2
close #1
end
