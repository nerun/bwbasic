# Puropose:  Verify existing BWBASIC behavior
#   Author:  Howard Wulf, AF5NE
#     Date:  2014-03-28
#    Usage:  implementatino defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/D71
#            ash ./00test.sh
#

rm *.80
rm *.OUT
rm *.LPT
rm *.dif

# ----------------------------------------------
# Regression Tests
# ----------------------------------------------
testcase()
{
   TESTCASE=${1}
   echo "TESTCASE=${TESTCASE}"
   ~/bwbasic --tape ${TESTCASE}.INP --profile D73.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT


   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile PAD80.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ---------------------------------------------


#
# PTP
#

rm PTP
testcase PTP_1
# creates file PTP and PTP.OUT

#
# LPT
#
testcase LPT_1
# created file LPT.OUT and LPT_1.LPT


#
# PTR
#
echo PTR LINE 1  > PTR
echo PTR LINE 2 >> PTR
echo PTR LINE 3 >> PTR
testcase PTR_1
testcase PTR_2
testcase PTR_3
testcase PTR_4
testcase PTR_5



#
# TTY (tested above)
#
# tested in LPT_1
# tested in PTP_1
testcase TTY_1


#
# TTY IN
#
testcase TTY_IN_1

#
# TTY OUT 
#
# tested in LPT_1
# tested in PTP_1

echo A > PUT_GET_1.PTR
testcase PUT_GET_1















# ----------------------------------------------
# EOF
# ----------------------------------------------
