How to run old TRS-80 BASIC programs using BWBASIC.
Here is a step-by-step guide to running old TRS-80 BASIC programs.
We will use the Tandy "Inventory Control System (126608)" for the TRS-80.


1. Get the original BASIC source code and data files in ASCII format.
   a. Download the files from http://www.willus.com/trs80/?q=Inventory
   b. For the BASIC programs (DATASORT and ICS),
      choose "Show as: BASIC program", then copy and paste the source into a text file.
      I saved them as "DATASORT.TXT" and "ICS.TXT".
   c. For the BINARY data files, 
      choose "Show as: Hex file",
      then copy and paste int a text file.
      I saved them as "DATAFILE.TXT", "INDEXFIL.TXT", "INFOFILE.TXT" and "STOCKNUM.TXT".
2. If you look at the BASIC source files 
   then you will observe there are NOT spaces surrounding keywords.
   As a result, the next step is to add the missing spaces surrounding keywords.
   You can edit the file by hand or use a utility such as "mod4basc.bas" to do this.
   I prefer to use "mod4basc.bas", since it works well.
3. How to run "mod4basc.bas" on "DATASORT.TXT".
   a. We first create a profile for the TRS-80 dialect that "mod4basc.bas" expects.
   b. This profile will be named "TRS-80.PRO" and contains:
      OPTION VERSION TRS-80
      WIDTH 24,80
   c. The command line is "bwbasic -p TRS-80.PRO mod4basc.bas".
   d. The input file is "DATASORT.TXT".
   e. The output file is "DATASORT.BAS".
4. How to run "mod4basc.bas" on "ICS.TXT".
   a. We first create a profile for the TRS-80 dialect that "mod4basc.bas" expects.
   b. This profile will be named "TRS-80.PRO" and contains:
      OPTION VERSION TRS-80
      WIDTH 24,80
   c. The command line is "bwbasic -p TRS-80.PRO mod4basc.bas".
   d. The input file is "ICS.TXT".
   e. The output file is "ICS.BAS".
   f. This ERRORS on line 38.  The reason it ERRORS is because of the multiline PRINT items.
   g. So, we copy "ICS.TXT" to "ICS2.TXT" and edit "ICS2.TXT" using a text editor to add
      the necessary PRINT statements and quotes.
   h. Now, we use the command line "bwbasic -p TRS-80.PRO mod4basc.bas".
   i. The input file is "ICS2.TXT".
   j. The output file is "ICS2.BAS".
   k. Copy "ICS2.BAS" to "ICS3.BAS" for fixups using a text editor.
5. Using a text editor on "ICS3.BAS"
   a. Fix-up 'IN PUT" to "INPUT" on lines 50, 60, 2690, 2700, 2710, 
   b. Fix-up ": ELSE " to " ELSE " on lines 130, 190, 300, 320, 340, 390, 400, 450, 460, 490, ...
   
   

