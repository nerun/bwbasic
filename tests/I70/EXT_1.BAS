  100 REM PURPOSE: VERIFY 'ALPHABET EXTENDERS'
  110 REM  AUTHOR: HOWARD WULF, AF5NE
  120 REM    DATE: 2017-03-23
  130 REM    NOTE: OPTION VERSION "CALL/360"
  135 REM
  140 LET $ = "A"
  150 LET # = 2
  160 LET @ = 3
  165 VARS
  170 PRINT "$-STRING", " #-NUMBER", " @-NUMBER"
  180 PRINT $, #, @
  190 LET $ = $ + $
  200 LET # = # + #
  210 LET @ = @ + @
  220 PRINT $, #, @
  230 LET $$ = "AB"
  240 LET ## = 11
  250 LET @@ = 99
  255 VARS
  260 PRINT "$$-STRING", " ##-NUMBER", " @@-NUMBER"
  270 PRINT $$, ##, @@
  280 LET $$ = $$ + $$
  290 LET ## = ## + ##
  300 LET @@ = @@ + @@
  310 PRINT $$, ##, @@
  320 LET @#$ = "AAA"
  330 LET #$@ = 222
  340 LET $@# = 333
  345 VARS
  350 PRINT "@#$-STRING", "#$@-NUMBER", "$@#-NUMBER"
  360 PRINT @#$, #$@, $@#
  370 LET @#$ = @#$ + @#$
  380 LET #$@ = #$@ + #$@
  390 LET $@# = $@# + $@#
  400 PRINT @#$, #$@, $@#
  999 END

