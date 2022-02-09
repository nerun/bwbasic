print "before Prior declaration"
Sub Prior
   Print "This is a subroutine prior to MAIN."
   Print "This should not print."
End Sub
print "between Prior and Main declaration"
Sub Main
   Print "This is the MAIN subroutine."
   Print "This should print."
End Sub
print "between Main and Subsequent declaration"
Sub Subsequent
   Print "This is a subroutine subsequent to MAIN."
   Print "This should not print."
End Sub
print "after Subsequent declaration"
Main
print "after Main call"
