# Puropose:  Verify existing BWBASIC behavior
#   Author:  Howard Wulf, AF5NE
#     Date:  2014-03-28
#    Usage:  implementatino defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/HB2
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
   ~/bwbasic --tape ${TESTCASE}.INP --profile HB2.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile PAD80.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ---------------------------------------------
# TESTS
# ----------------------------------------------
testcase AMP_1
testcase BASE_1
# testing the function COMMAND$() requires additional parameters
# testcase COMMAND5_1
testcase FILE_1
testcase IF_END_8
testcase IF_MORE_8
testcase LBOUND
testcase MAIN_2
testcase MAT_INPUT_7
testcase MAT_INV_1
testcase MAT_MULT_1
testcase MAT_READ_7
testcase MAT_READ_8
testcase MAT_WRITE_8
testcase POP_1
testcase RESTORE_3
testcase ROUND_1
testcase TIM_77
#
# SYSTEM/360 file i/o
#
rm       PUT_1.TXT
testcase PUT_1
testcase GET_1
testcase RESET_1
testcase PUT_2
testcase GET_2
testcase CLOSE_1
rm       MAT_PUT_1.TXT
testcase MAT_PUT_1
testcase MAT_GET_1
testcase PAUSE_1
testcase PRINT_USING_1
testcase PRINT_USING_2
testcase COLLATE_1
testcase SYNTAX_1
testcase SYNTAX_2
testcase SYNTAX_3
#
# Added in Version 3.20
#
# testcase BASIC
testcase DIMTO
testcase MATTO
testcase CONST_1
testcase CHR4_ASC
testcase RECLEN

# ----------------------------------------------
# EOF
# ----------------------------------------------
