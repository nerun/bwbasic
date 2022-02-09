# Filename:  xxx.sh
#  Purpose:  Build Bywater BASIC for Unix uwing GCC 4.8
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-01-29
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/
#            ash ./xxx.sh
#

# cleanup
rm -f bwbasic
rm -f *.o

# normal compile
gcc -o bwbasic -lm -ansi -pedantic -Wall -Werror bw*.c

# On my system, I cannot execute the binary from the source folder.
# So, I have to copy the binary to ~ which is
# /data/data/com.pdaxrom.cctools/root/cctools/home/
cp bwbasic ~/

# cleanup
rm -f bwbasic

# make the binary executable
chmod 764 ~/bwbasic

# start exectuable
~/bwbasic

# EOF
