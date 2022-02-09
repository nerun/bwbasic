# Filename:  xxx.sh
#  Purpose:  compile source code
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-01-29
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic-2.62/
#            ash ./yyy.sh
#

# cleanup
rm -f bwbasic
rm -f *.o

# for normal compile
# gcc -DHAVE_ISNAN -o bwbasic -lm -ansi -pedantic -Wall -Werror bw*.c


# for broken strpcy()
# gcc -DHAVE_ISNAN -DBROKEN_STRCPY -o bwbasic -lm -ansi -pedantic -Wall -Werror bw*.c



# for regression testing
gcc -DHAVE_ISNAN -DAUTOMATED_REGRESSION -o bwbasic -lm -ansi -pedantic -Wall -Werror bw*.c


# for regression testing with broken strpcy()
# gcc -DHAVE_ISNAN -DAUTOMATED_REGRESSION -DBROKEN_STRCPY -o bwbasic -lm -ansi -pedantic -Wall -Werror bw*.c


# On my system, I have to copy the binary to another location
cp bwbasic ~/

# cleanup
rm -f bwbasic

# make executable
chmod 764 ~/bwbasic

# start exectuable
~/bwbasic

# EOF
