120 open "I", #7, "E044.DAT"
130 let b$ = input$( 17, #7 )
140 print b$; len( b$ )
145 if b$ <> "this is the first" then
       ERROR 1
    end if
150 close #7
160 print "*** TEST PASSED ***"
999 END
