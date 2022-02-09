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
testcase ACSD_1
testcase ACSG_1
testcase ACS_1
testcase APPEND_1
testcase ARCSIN_1
testcase ARCSIN_2
testcase ARCTAN_1
testcase ASCII_1
testcase ASCII_2
testcase ASND_1
testcase ASND_2
testcase ASNG_1
testcase ASNG_2
testcase ATND_1
testcase ATNG_1
testcase BYE_1
testcase CALL_1
testcase CHANGE_1
testcase CHANGE_2
testcase CODE_1
testcase CODE_2
testcase COMMON_1
testcase COSD_1
testcase COSG_1
testcase COSH_1
testcase COUNT_1
testcase CSH_1
testcase CUR_1
testcase DEGREE_1
testcase DEGREE_2
testcase DEGREE_3
testcase DET_2
testcase DIGITS_1
testcase ERASE_1
testcase ERRL_1
testcase ERRN_1
testcase EXCHANGE_1
testcase EXIT_1
testcase FLOW_1
testcase FLOW_2
testcase FRAC_1
testcase GOODBYE_1
testcase GOSUB_OF_1
testcase GRADIAN_1
testcase HOME_1
testcase IMAGE_1
testcase INDEX_1
# INPUT$() must be manually tested
# testcase INPUT5_1
testcase INSTR_1
testcase INSTR_2
testcase LINE_INPUT_1
testcase LINE_INPUT_2
testcase LLIST_1
testcase LLIST_2
testcase LPRINT_1
testcase MAT_ADDITION_1
testcase MAT_ASSIGN_1
testcase MAT_CON_1
testcase MAT_CON_2
testcase MAT_CON_3
testcase MAT_CON_4
testcase MAT_IDN_1
testcase MAT_IDN_2
testcase MAT_IDN_3
testcase MAT_IDN_4
testcase MAT_INV_1
testcase MAT_MULTIPLICATION_1
testcase MAT_MULTIPLICATION_2
testcase MAT_SUBTRACTION_1
testcase MAT_TRN_1
testcase MAT_ZER_1
testcase MAT_ZER_2
testcase MAT_ZER_3
testcase MAT_ZER_4
testcase MAX_1
testcase MAX_2
testcase MAX_3
testcase MIN_1
testcase MIN_2
testcase MIN_3
testcase NEW_1
testcase NOFLOW_1
testcase NOT_1
testcase NOT_2
testcase NOT_3
testcase NUM5_1
testcase NUM_1
testcase NUM_2
testcase NUM_3
testcase OR_1
testcase OR_2
testcase OR_3
testcase OR_4
testcase PAUSE_1
testcase PAUSE_2
testcase PI_1
testcase POP_1
testcase POS_1
testcase POS_2
testcase POS_3
testcase PRECISION_1
testcase RADIAN_1
testcase RAD_1
testcase RAD_2
testcase RECALL_1
testcase REPEAT5_1
testcase SCRATCH_1
testcase SCRATCH_2
testcase SEG5_1
testcase SEG_1
testcase SGN_1
testcase SIND_1
testcase SING_1
testcase SINH_1
testcase SINH_2
testcase SIN_1
testcase SNH_1
testcase SNH_2
testcase STORE_1
testcase STRING_1
testcase STR_1
testcase SWAP_1
testcase TAND_1
testcase TANG_1
testcase TANH_1
testcase TANH_2
testcase TIME_1
testcase TLOAD_1
testcase TOP_1
testcase TSAVE_1
testcase UNTIL_1
testcase WHILE_1
testcase XOR_1
testcase XRA_1
# ----------------------------------------------
# EOF
# ----------------------------------------------
