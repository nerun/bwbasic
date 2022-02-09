# Puropose:  Verify existing BWBASIC behavior
#   Author:  Howard Wulf, AF5NE
#     Date:  2014-03-28
#    Usage:  implementatino defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/BP100
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
   ~/bwbasic --tape ${TESTCASE}.INP --profile TRS-80.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile BYWATER.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ---------------------------------------------
#
# TRS-80 Model I/III/4 with 16 row x 64 column display
#

echo "OPTION VERSION TRS-80"  > TRS-80.PRO
echo "OPTION INDENT 2      " >> TRS-80.PRO
echo "OPTION TERMINAL ANSI " >> TRS-80.PRO
echo "WIDTH 16, 64         " >> TRS-80.PRO



echo "OPTION VERSION BYWATER"  > BYWATER.PRO
echo "OPTION LABELS OFF     " >> BYWATER.PRO

testcase SETUP
testcase ICS
# ----------------------------------------------
# EOF
# ----------------------------------------------
