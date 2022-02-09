   10 OPEN "I", #1, "A"
   20 OPEN "O", #2,  "A.TXT"
   30 WHILE NOT EOF(1)
   40   LINE INPUT #1, A$
   50   B$ = "testcase " + A$
   60   PRINT #2, B$
   70 WEND
   80 CLOSE 2
   90 CLOSE 1
   99 END
