520 DIM N(2,20)
530 DIM K$(6,20)
 open "glsize" as 1:read #1;z2,z3:close 1
     open "gl" recl 138 as 1
1510 FOR Z2=1 TO Z3
1515 read #1,z2;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),          n(2,7),n(2,8),n(2,9),n(2,10)
1516 FOR Z=4 TO 10:N(2,Z)=0:NEXT Z
1515 print #1,z2;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),          n(2,7),n(2,8),n(2,9),n(2,10)
1520 IF n(2,2)=0 THEN 1535
     n$=""
1525 NEXT Z2
1535 CLOSE 1
 END
