1     rem This is the Invoice Number Sequencing Entry Program

     if end #1 then 2000
     open "inv" as 1
     read #1;n
     close 1
1030 PRINT "CURRENT INVOICE NUMBER IS ";N
1040 PRINT:PRINT "PLEASE TYPE IN THE NEW INVOICE SEQUENCE"
1050 PRINT "STARTING NUMBER."
1060 PRINT:PRINT "A ZERO VALUE WILL ALLOW THE MANUAL ENTRY OF"
1070 PRINT "NON-SEQUENTIAL INVOICE NUMBERS."
1080 PRINT:INPUT N
1090 open "inv" as 1
     print #1;n
     close 1
     stop
2000 create "inv" as 1
     close 1
     goto 1040
