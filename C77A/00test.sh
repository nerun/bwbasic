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
rm *.TXT

# ----------------------------------------------
# Regression Tests
# ----------------------------------------------
testcase()
{
   TESTCASE=${1}
   echo "TESTCASE=${TESTCASE}"
   ~/bwbasic --tape ${TESTCASE}.INP --profile C77.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile BYWATER.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ---------------------------------------------
echo "OPTION VERSION CBASIC-II"  > C77.PRO

echo "OPTION VERSION BYWATER" > BYWATER.PRO
echo "OPTION LABELS OFF"     >> BYWATER.PRO

testcase FILE_1
testcase FILE_2
testcase FILE_3
testcase FILE_4
testcase FILE_5
testcase FILE_6
testcase QUOTE_1
testcase QUOTE_2
testcase QUOTE_3
testcase INPUT_1
testcase CONSOLE_1
testcase LPRINTER_1
testcase CREATE_1
testcase CREATE_2
testcase IF_END_1
testcase IF_END_2
testcase RETURN_1
testcase RETURN_2
testcase RETURN_3
testcase DELETE_1
testcase MATCH_1
testcase USING_1
testcase READ_1
testcase READ_2
testcase EX_1
testcase EX_2
testcase EX_3
testcase EX_4
testcase SIZE_1

# ----------------------------------------------
# EOF
# ----------------------------------------------
