100 REM           ASTROMETRIC REDUCTION
105 REM
110 DIM XZ(10),YZ(10),X1(10),Y1(10)
115 DIM AA(10),DXZ(10),RR(10,9),RAA(10),RDXZ(10)
120 DEFDBL A-H,K-Z
125 ' PI=3.141592653589793: 
126 DR=PI/180
130 INPUT "Camera focal length";L
135 PRINT "R.A. of plate center (h,m,s)";
140 GOSUB 630: A0=V*15*DR
145 PRINT "Dec. of plate center (d,m,s)";
150 GOSUB 630: D0=V*DR: SD=SIN(D0): CD=COS(D0)
160 INPUT "Equinox, epoch";EQ,EP
165 T=EP-EQ
170 INPUT "How many stars (4-10)";N
175 FOR J=1 TO N
180 PRINT
185 PRINT "R.A. of star ";J;"(h,m,s)";
190 GOSUB 630
195 INPUT "Proper motion (sec/yr)";M1
200 AA(J)=(V+T*M1/3600)*15*DR
205 PRINT "Dec. (d,m,s)";
210 GOSUB 630
215 INPUT "Proper motion (arcsec/yr)";M2
220 DXZ(J)=(V+T*M2/3600)*DR: SJ=SIN(DXZ(J)): CJ=COS(DXZ(J))
225 H=SJ*SD+CJ*CD*COS(AA(J)-A0)
230 X1(J)=CJ*SIN(AA(J)-A0)/H
235 Y1(J)=(SJ*CD-CJ*SD*COS(AA(J)-A0))/H
240 INPUT "Measured X,Y";XZ(J),YZ(J)
245 NEXT J
250 PRINT
255 INPUT "Measured X,Y of target";X,Y
260 R1=0: R2=0: R3=0: R7=0: R8=0: R9=0: XS=0: YS=0
265 FOR J=1 TO N
270 XS=XS+XZ(J): YS=YS+YZ(J): RR(J,1)=XZ(J)*XZ(J)
275 R1=R1+RR(J,1): RR(J,2)=YZ(J)*YZ(J): R2=R2+RR(J,2)
280 RR(J,3)=XZ(J)*YZ(J): R3=R3+RR(J,3)
285 RR(J,7)=Y1(J)-YZ(J)/L: R7=R7+RR(J,7)
290 RR(J,8)=RR(J,7)*XZ(J): R8=R8+RR(J,8)
295 RR(J,9)=RR(J,7)*YZ(J): R9=R9+RR(J,9)
300 NEXT J
305 REM    Now solve for D, E, F, by Cramer's Rule
310 DD=R1*(R2*N-YS*YS)-R3*(R3*N-XS*YS)+XS*(R3*YS-XS*R2)
315 D=R8*(R2*N-YS*YS)-R3*(R9*N-R7*YS)+XS*(R9*YS-R7*R2)
320 E=R1*(R9*N-R7*YS)-R8*(R3*N-XS*YS)+XS*(R3*R7-XS*R9)
325 F=R1*(R2*R7-YS*R9)-R3*(R3*R7-XS*R9)+R8*(R3*YS-XS*R2)
330 D=D/DD: E=E/DD: F=F/DD
335 REM
340 R4=0: R5=0: R6=0
345 FOR J=1 TO N
350 RR(J,4)=X1(J)-XZ(J)/L: R4=R4+RR(J,4)
355 RR(J,5)=RR(J,4)*XZ(J): R5=R5+RR(J,5)
360 RR(J,6)=RR(J,4)*YZ(J): R6=R6+RR(J,6)
365 NEXT J
370 REM     Now solve for A ,B, C, by Cramer's Rule
375 A=R5*(R2*N-YS*YS)-R3*(R6*N-R4*YS)+XS*(R6*YS-R4*R2)
380 B=R1*(R6*N-R4*YS)-R5*(R3*N-XS*YS)+XS*(R3*R4-XS*R6)
385 C=R1*(R2*R4-YS*R6)-R3*(R3*R4-XS*R6)+R5*(R3*YS-XS*R2)
390 A=A/DD: B=B/DD: C=C/DD
395 PRINT
400 PRINT "        Plate Constants "
405 PRINT "     R.A.              Dec.
410 PRINT USING "A = ##.#####      D = ##.#####";A;D
415 PRINT USING "B = ##.#####      E = ##.#####";B;E
420 PRINT USING "C = ##.#####      F = ##.#####";C;F
425 REM
430 REM     NOW FIND RESIDUALS
435 AS=0: DS=0
440 FOR J=1 TO N
445 RAA(J)=XZ(J)-L*(X1(J)-(A*XZ(J)+B*YZ(J)+C))
450 RDXZ(J)=YZ(J)-L*(Y1(J)-(D*XZ(J)+E*YZ(J)+F))
455 AS=AS+((RAA(J)/L)*3600/(DR*15*COS(D0)))^2
460 DS=DS+((RDXZ(J)/L)*3600/DR)^2
465 NEXT J
470 S1=SQR(AS/(N-3)): S2=SQR(DS/(N-3))
475 PRINT
480 PRINT "Residuals        R.A.     Dec."
482 A$=" Star ##     #.#####   #.#####"
485 FOR J=1 TO N
490 PRINT USING A$;J;RAA(J);RDXZ(J)
495 NEXT J
500 PRINT
505 REM  Find standard coordinates of target
510 XX=A*X+B*Y+C+X/L:   YY=D*X+E*Y+F+Y/L
515 B=CD-YY*SD: G=SQR(XX*XX+B*B)
520 REM   
525 REM   Find right ascension of target
530 A5=ATN(XX/B): IF B<0 THEN A5=A5+PI
535 A6=A5+A0: IF A6>2*PI THEN A6=A6-2*PI
540 IF A6<0 THEN A6=A6+2*PI
545 V=A6/(DR*15): GOSUB 660: A1=V1: A2=V2: A3=V3
550 REM
555 REM   Find declination of target
560 D6=ATN((SD+YY*CD)/G): V=D6/DR: GOSUB 660
570 D1=V1: D2=V2: D3=V3
575 REM
580 PRINT "For target:"
585 PRINT
590 PRINT "Right ascension   ";
592 PRINT USING "## ## ##.###";A1;A2;A3
595 PRINT USING "  Std. dev.             ##.###";S1
600 PRINT
605 PRINT "Declination       ";S$;
610 PRINT USING "## ## ##.##";D1;D2;D3
615 PRINT USING "  Std. dev.              ##.##";S2
620 END
625 REM
630 REM   Input of sexagesimal values
635 INPUT V$,V2,V3
640 S=1: IF LEFT$(V$,1)="-" THEN S=-1
645 V1=ABS(VAL(V$)): V=S*(V1+V2/60+V3/3600)
655 RETURN
660 REM   Output of sexagesimal values
665 S$="+": IF V<0 THEN S$="-"
670 V=ABS(V): V1=INT(V): VM=60*(V-V1)
675 V2=INT(VM): V3=60*(VM-V2)
680 RETURN
690 REM
700 REM  This program is used to analyze measurements
710 REM  of minor planet or comet positions on a 
720 REM  photographic plate and deduce precise 
730 REM  coordinates.  Written by Jordan D. Marche
740 REM  and explained by him in Sky & Telescope
750 REM  for July, 1990, page 71.
