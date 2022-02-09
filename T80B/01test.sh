# Filename:  01test.sh
#  Purpose:  automted regression review
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-01-29
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic/SCBP
#            ash ./01test.sh
#
#

# review ERRORS
cat *.dif > dif.OUT
if test -s  dif.OUT
then
    less dif.OUT
fi
# ----------------------------------------------
# EOF
# ----------------------------------------------
