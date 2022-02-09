1000 rem THIS IS THE EMPLOYEE PAYROLL CLEARING PROGRAM (EPCLEAR)
1300 DIM N(2,5),N$(250)
     z5$="ep"
     if end #1 then 10000
1500 open z5$ recl 512 as 1
     z3=(size(z5$)*2048)/512
     for z2=1 to z3
     read #1,z2;n(2,1),n(2,2),n$
     if n(2,2)=0 then 1530
     next z2
1530 z3=int(z3)-1
     close 1
1610 z2=z2-1
6000 open z5$ recl 512 as 1
6010 N$=""
6020 FOR I=1 TO Z2
     read #1,I;n(2,1),n(2,2),n$,n,r,h1,h2,h3,e0,e1,e2,f1,f2,f3,          e3,e4,e5,e6,e7,e8,s1,s2,s3,m1,m2,m3
8000 H1=0:H2=0:H3=0:E0=0:E1=0:E2=0:E3=0:E4=0:E5=0:E6=0:E7=0:E8=0
8010 F1=0:F2=0:F3=0:S1=0:S2=0:S3=0:M1=0:M2=0:M3=0
     print #1,I;n(2,1),n(2,2),n$,n,r,h1,h2,h3,e0,e1,e2,f1,f2,f3,          e3,e4,e5,e6,e7,e8,s1,s2,s3,m1,m2,m3
9015 N$=""
9020 NEXT I
     STOP
10000 print:print:print "Please install employee payroll file in"
      print "drive B, then type return to continue.":print
      input line temp$
      goto 1500
