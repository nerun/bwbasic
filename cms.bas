rem Purpose: re-define externals to only 6 characters for CMS
rem  Author: Howard Wulf, AF5NE
rem    Date: 2015-02-10
rem   Usage:  implementation defined
rem Example:
rem          ~/bwbasic cms.bas
rem
rem -------------------------------------------------------------------------------
rem File      File Name     IN/OUT   Description
rem #1        "bwbasic.h"    INPUT   read looking for "extern" statements
rem #2        "cms.h"       OUTPUT   written with "#define" statements
rem -------------------------------------------------------------------------------
rem Variable  Description
rem N         The number of "extern" statements so far processed
rem E$        The constant value "extern "
rem E         The length of E$
rem L$        The input line read from "bwbasic.h"
rem M$        The next line from "bwbasic.h" when L$ does not contain a semicolon
rem X         The location of special characters in L$
rem H$        The hexadecimal value of N
rem -------------------------------------------------------------------------------
rem
let N = 0
let E$ = "extern "
let E = len( E$ )
open "bwbasic.h" for input as #1
open "cms.txt" for output as #2
while not eof( #1 )
  line input #1, L$
  L$ = trim$( L$ )
  if left$( L$, E ) = E$ then
    rem extern ....
    while instr( L$, ";" ) = 0
      ' read more lines until we get a semicolon
      line input #1, M$
      M$ = trim$( M$ )
      L$ = L$ + " " + M$
    wend
    rem extern ...;
    L$ = trim$(mid$( L$, E + 1 ))
    ' truncate trailing semicolon
    X = instr( L$, ";" )
    if X > 0 then
      L$ = trim$(left$( L$, X - 1 ))
    end if
    ' truncate trailing parenthesis
    X = instr( L$, "(" )
    if X > 0 then
      L$ = trim$(left$( L$, X - 1 ))
    end if
    ' truncate trailing bracket
    X = instr( L$, "[" )
    if X > 0 then
      L$ = trim$(left$( L$, X - 1 ))
    end if
    ' find the last word
    X = instr(L$, " " )
    while X > 0
      L$ = trim$(mid$( L$, X + 1 ))
      X = instr(L$, " " )
    wend
    ' skip leading asterick
    while left$( L$, 1 ) = "*"
      L$ = trim$(mid$( L$, 2 ))
    wend
    if L$ = "main" then
      ' ignore magic function name
    else
      ' pad for alignment
      REM L$ = L$ + space$(32)
      REM L$ = left$( L$, 32 )
      REM H$ = "00000" + hex$(N)
      REM H$ = right$( H$, 5 )
      REM print #2, "#define ";L$;" X";H$
      REM N = N + 1
      if len( L$ ) > 0 then
        print #2, L$
      end if
    end if
  end if
wend
close #2
close #1
REM sort before assigning value
if shell( "sort < cms.txt > cms.out" ) = 0 then
  N = 0
  open "cms.out" for input  as #1
  open "cms.h"   for output as #2
  while not eof(#1)
    line input #1, L$
    L$ = trim$(L$)
    if len(L$) then
      ' pad for alignment
      L$ = L$ + space$(32)
      L$ = left$( L$, 32 )
      H$ = "00000" + hex$(N)
      H$ = right$( H$, 5 )
      print #2, "#define ";L$;" X";H$
      N = N + 1
    end if
  wend
  close #2
  close #1
  rem Cleanup temporary files
  kill "cms.txt"
  kill "cms.out"
end if
end
