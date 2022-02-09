rem gcc -w -c -ansi -I . %1 %2 %3 %4 %5 %6 %7 %8 %9
rem bcc32 -w- -A -c -I. %1 %2 %3 %4 %5 %6 %7 %8 %9
gccmvs -w -S -ansi -nostdinc -I ../pdos/pdpclib -I . %1 %2 %3 %4 %5 %6 %7 %8 %9
