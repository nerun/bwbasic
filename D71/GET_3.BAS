  100 rem Purpose: Verify OPEN - PUT - GET - CLOSE sequence
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2016-01-12
  130 rem    Note: OPTION VERSION RSTS-11
  140 rem
  200 let F$ = "GET_3.DAT"
  210 let F% = 123
  220 open F$ as file F% , recordsize 1, clustersize 2, filesize 3, mode 4
  225 field F%, 128 as F1$, 128 as F2$, 128 as F3$, 128 as F4$
  226 let F1$ = "1"
  227 let F2$ = "2"
  228 let F3$ = "3"
  229 let F4$ = "4"
  230 for R% = 1 to 16
  240   put F%, record R%
  250   get F%, record R%
  260 next R%
  270 close F%
  999 end

