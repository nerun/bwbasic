# Filename:  zzz.sh
#  Purpose:  automted regression review
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-01-29
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic-2.62/
#            ash ./zzz.sh
#
#

# cd NBS2

echo "OPTION VERSION BYWATER" > profile.bas
echo "OPTION BUGS OFF"       >> profile.bas

~/bwbasic FNCS.BAS > FNCS.OUT

~/bwbasic CMDS.BAS > CMDS.OUT

~/bwbasic MAINTAINER.BAS > MAINTAINER.OUT
mv LPRINT.OUT MAINTAINER.OUT

~/bwbasic MANUAL.BAS > MANUAL.OUT
mv LPRINT.OUT MANUAL.OUT

# grep "SYNTAX:" LPRINT.OUT > SYNTAX.INP
# ~/bwbasic SYNTAX.BAS > SYNTAX.OUT

cat *.dif > dif.OUT
if test -s  dif.OUT
then
    less dif.OUT
fi

# EOF
