# Puropose:  Verify existing BWBASIC behavior
#   Author:  Howard Wulf, AF5NE
#     Date:  2014-03-28
#    Usage:  implementatino defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/G65
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
   ~/bwbasic --tape ${TESTCASE}.INP --profile G65.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
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
# TRS-80 Model I/III/4 with 24 row x 80 column display
#
echo "OPTION VERSION MARK-I"  > G65.PRO

echo "OPTION VERSION BYWATER" > BYWATER.PRO
echo "OPTION LABELS OFF"     >> BYWATER.PRO

testcase BASICT
testcase GCN3NO
testcase HILTST
testcase LINEAR
testcase MATRIX1
testcase MATRIX2
testcase MAXSIN
testcase PRINTE
testcase REDIM1
testcase REDIM2
testcase REDIM3
testcase REDIM4
testcase REDIM5
testcase REDIM6
testcase REDIM7
testcase REDIM8
testcase REDIM9
testcase REDIMA
testcase REDIMB
testcase REDIMC
testcase RNDTES
testcase SALES1
testcase SAMPLE
# ----------------------------------------------
# EOF
# ----------------------------------------------
