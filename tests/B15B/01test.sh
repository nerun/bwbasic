# Filename:  01test.sh
#  Purpose:  automted regression review
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-01-29
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic/NBS2
#            ash ./01test.sh
#
#

# review ERRORS
cat *.dif > dif.OUT
if test -s  dif.OUT
then
    pwd > dif.OUT
    cat *.dif >> dif.OUT
    less dif.OUT
fi
# ----------------------------------------------
# EOF
# ----------------------------------------------
