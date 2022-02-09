100 rem Purpose: Verify multi-line DEF ... FNEND
110 rem  Author: Howard Wulf, AF5NE
120 rem    Date: 2015-05-22
130 rem
135 rem single-line without parameter
140 def FNA=1.234
145 rem single-line with parameter
150 def FNB(X)=X*2
160 def FNC
165 rem multi-line without parameter
170 let FNC=3
180 fnend
190 def FND(X)
200 rem multi-line with parameter
210 if X > 10 then 240
220 let FND=X*2
230 goto 250
240 let FND=X/2
250 fnend
260 rem
270 rem
300 print fna,fnb(2),fnc,fnd(5),fnd(15)
310 end
