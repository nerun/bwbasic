# Puropose: Verify existing BWBASIC behavior
#   Author: Howard Wulf
#     Date: 2014-03-28
#    Usage: implementatino defined
#  Example:
#           cd /sdcard/Download/BASIC/bwbasic3/HB1X
#           ash ./00test.sh
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
   ~/bwbasic --tape ${TESTCASE}.INP --profile HB1.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
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
# The BASIC Handbook, 1st Edition
#
echo "OPTION VERSION HANDBOOK1" > HB1.PRO

echo "OPTION VERSION BYWATER" > BYWATER.PRO
echo "OPTION LABELS OFF"     >> BYWATER.PRO


testcase AS_1
testcase AS_2
testcase AS_3
testcase AS_4
testcase AS_5
testcase BREAK_1
# EDIT must be manually tested
# testcase EDIT_1
# testcase EDIT_2
# testcase EDIT_3
testcase FNEND
testcase GOSUB_OF
testcase GOSUB_X
testcase GOTO_OF
testcase GOTO_X
testcase LIKE_1
testcase MAT_INPUT_1_N_0
testcase MAT_INPUT_1_S_1
testcase MAT_PRINT_1_N_0
testcase MAT_PRINT_1_S_0
testcase MAT_PRINT_2_N_0
testcase MAT_PRINT_2_S_0
testcase MAT_PRINT_3_N_0
testcase MAT_PRINT_3_S_0
testcase MAT_PRINT_4
testcase MAT_READ_1_N_0
testcase MAT_READ_1_N_1
testcase MAT_READ_1_S_0
testcase MAT_READ_1_S_1
testcase MAT_READ_2_N_0
testcase MAT_READ_2_S_0
testcase MAT_READ_3_N_0
testcase MAT_READ_3_S_0
testcase REM_1
testcase USING_1
testcase USING_2
# ----------------------------------------------
# EOF
# ----------------------------------------------
