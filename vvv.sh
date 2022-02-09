# Filename:  vvv.sh
#  Purpose:  source code review, similar to "lint"
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-01-29
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic-2.62/
#            ash ./vvv.sh
#
cppcheck --std=c89 --enable=all  . 1> cppcheck1.txt 2> cppcheck2.txt
sort < cppcheck2.txt  > cppcheck3.txt
# EOF
