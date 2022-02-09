200	rem This is CHECK.BAS used to check for needed files


	IF BIZMII.FLAG=1 THEN CHECK5=1 :GOTO 360
300	CHECK1=SIZE("DATE")
	CHECK2=SIZE("GLF")
	CHECK3=SIZE("GLT")
	CHECK4=SIZE("NM")
	CHECK5=1
	IF CHECK1<1 OR CHECK2<1 OR CHECK3<1 OR CHECK4<1 THEN CHECK5=0
	IF CHECK5 THEN 350
	AAA=AAA+1
	IF AAA>1 THEN 360
	print clear$
	print "Here are the files that are missing from your data disk."
	print "That's drive 'B':"
	print
	IF CHECK1<1 THEN PRINT "1 - CHANGE REPORT DATE"
	IF CHECK2<1 THEN PRINT "2 - CHANGE DOLLAR VALUE"
	IF CHECK3<1 THEN PRINT "3 - CHANGE PAGE WIDTH ,TABS OR COMPANY"
	IF CHECK4<1 THEN PRINT "5 - ADD NEW COMPANY NAMES"
	PRINT
	print "You can get to these programs by typing '7' in the"
	print "main menu. If you have already used these programs,"
	print "then the files you need are on your data disk #5"
	print
	print "Remember that these four files must be on your ";
	print "data disk #5 :"
	print
	print "            DATE"
	print "            GLF"
	print "            GLT"
	print "            NM"
	print
	PRINT "Put the appropriate disk in ";
	input "drive 'B' then (return)";LINE JRF$
	print clear$
	INITIALIZE
	GOTO 300
350	REM WE MADE IT
	IF BIZMII.FLAG<>1 THEN CHAIN "READFILE"
360	REM
