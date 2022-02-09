# Puropose:  Verify existing BWBASIC behavior
#   Author:  Howard Wulf, AF5NE
#     Date:  2014-03-28
#    Usage:  implementatino defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/HB1
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
   ~/bwbasic --tape ${TESTCASE}.INP --profile HB1.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile PAD80.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ---------------------------------------------
testcase ABS_1
testcase ABS_2
testcase ABS_3
testcase AND_1
testcase AND_2
testcase AND_3
testcase AND_4
testcase ASC_1
testcase ASC_2
testcase ASC_3
testcase ASN_1
testcase ASN_2
testcase ATAN_1
testcase ATAN_2
testcase ATN_1
testcase AT_1
testcase AT_2
testcase AT_3
testcase AT_4
testcase AT_5
testcase AUTO_1
testcase BREAK_1
testcase CDBL_1
testcase CDBL_2
testcase CHAIN_1
testcase CHAIN_2
testcase CHAR_1
testcase CHAR_2
testcase CHAR_3
testcase CHAR_4
testcase CHR_1
testcase CHR_2
testcase CINT_1
testcase CINT_2
testcase CLEAR_1
testcase CLEAR_2
testcase CLEAR_3
testcase CLG_1
testcase CLG_2
# CLK should change
testcase CLK_1
testcase CLK_2
testcase CLK_3
testcase CLK_4
testcase CLOAD_1
testcase CLOAD_2
testcase CLOG_1
testcase CLOG_2
testcase CLR_1
testcase CLS_1
testcase CONT_1
testcase CONT_2
testcase COS_1
rm CSAVE_1.DAT
testcase CSAVE_1
testcase CSNG_1
testcase CSNG_2
testcase D_1
testcase DATA_1
testcase DATA_2
testcase DATA_3
testcase DEFDBL_1
# D is not supported
testcase DEFDBL_2
# D is not supported
testcase DEFDBL_3
testcase DEFINT_1
testcase DEFINT_2
testcase DEFINT_3
testcase DEFSNG_1
testcase DEFSNG_2
testcase DEFSNG_3
testcase DEFSTR_1
testcase DEFSTR_2
testcase DEFSTR_3
testcase DEF_1
testcase DEF_2
testcase DEF_3
testcase DEG_1
testcase DEG_2
testcase DEG_3
testcase DELETE_1
testcase DIM_1
testcase DIM_2
testcase DIM_3
testcase DIM_4
testcase DIM_5
testcase DIM_6
testcase DIM_7
testcase DSP_1
testcase E_1
# EDIT must be manually tested
# testcase EDIT_1
testcase ELSE_1
testcase ELSE_2
testcase END_1
testcase EQ_1
testcase ERL_1
testcase ERROR_1
testcase ERROR_2
testcase ERR_1
testcase EXAM_1
testcase EXP_1
testcase FETCH_1
testcase FILL_1
testcase FIX_1
testcase FNEND_1
testcase FN_1
testcase FOR_1
testcase FREE_1
testcase FREE_2
testcase FRE_1
testcase FRE_2
testcase GE_1
testcase GOSUB_1
testcase GOTO_1
testcase GOTO_OF_1
testcase GO_1
testcase GO_2
testcase GO_TO_OF_1
testcase GT_1
testcase IF_1
# IF-GOSUB not supported
# testcase IF_GOSUB_1
testcase IF_GOTO_1
# IF-LET not supported
# testcase IF_LET_1
testcase IF_THEN_1
testcase IF_THEN_2
testcase IF_THEN_3
# implied THEN not supported
# testcase IF_THEN_4
# INKEY$ must be manually tested
# testcase INKEY_1
testcase INPUT_1
testcase INP_1
testcase INP_2
testcase INT_1
testcase LEFT_1
testcase LEFT_2
testcase LEFT_3
testcase LEN_1
testcase LET_1
testcase LET_2
testcase LE_1
testcase LGT_1
testcase LGT_2
testcase LIN_1
testcase LIST_1
testcase LIST_2
testcase LN_1
testcase LN_2
testcase LOAD_1
testcase LOG10_1
testcase LOG10_2
testcase LOGE_1
testcase LOGE_2
testcase LOG_1
testcase LT_1
testcase MAN_1
# MAT INPUT (EXPLICIT DIM)
testcase MAT_INPUT_1
# MAT INPUT (IMPLICIT DIM)
testcase MAT_INPUT_2
# MAT INPUT (REQUEST < DIM)
testcase MAT_INPUT_3
# MAT INPUT (REQUEST = DIM)
testcase MAT_INPUT_4
# MAT INPUT (REQUEST > DIM)
testcase MAT_INPUT_5
# MAT PRINT (EXPLICIT DIM)
testcase MAT_PRINT_1
testcase MAT_PRINT_2
testcase MAT_PRINT_3
testcase MAT_PRINT_4
# MAT PRINT (REQUEST < DIM)
testcase MAT_PRINT_5
# MAT PRINT (REQUEST = DIM)
testcase MAT_PRINT_6
# MAT PRINT (REQUEST > DIM)
testcase MAT_PRINT_7
# MAT PRINT (IMPLICIT DIM)
testcase MAT_PRINT_8
# MAT READ (EXPLICIT DIM)
testcase MAT_READ_1
# MAT READ (IMPLICIT DIM)
testcase MAT_READ_2
# MAT READ (REQUEST < DIM)
testcase MAT_READ_3
# MAT READ (REQUEST = DIM)
testcase MAT_READ_4
# MAT READ (REQUEST > DIM)
testcase MAT_READ_5
testcase MEM_1
testcase MEM_2
testcase MID_1
testcase MID_2
testcase MOD_1
testcase MOD_2
testcase MOD_3
testcase NEW_1
testcase NEXT_1
testcase NEXT_2
testcase NEXT_3
testcase NE_1
testcase NOTRACE_1
testcase NOTRACE_2
testcase ON_ERROR_GOTO_1
testcase ON_GOSUB_1
testcase ON_GOTO_1
testcase ON_GOTO_2
testcase OUT_1
# this test takes a long time
# testcase OUT_2
testcase PDL_1
testcase PDL_2
testcase PEEK_1
testcase PIN_1
testcase PIN_2
testcase PI_1
testcase PI_2
# this test takes a long time
# testcase POKE_1
testcase PRINT_1
testcase PRINT_2
testcase PRINT_3
testcase PRINT_AT_1
testcase PRINT_AT_2
testcase PRINT_USING_1
testcase PRINT_USING_2
testcase PRINT_USING_3
testcase PRINT_USING_4
testcase PRINT_USING_5
testcase PRINT_USING_6
testcase PRINT_USING_7
testcase PRINT_USING_8
testcase PRINT_USING_9
testcase PRINT_USING_A
testcase PRINT_USING_B
testcase PRINT_USING_C
testcase PRINT_USING_D
testcase RANDOMIZE_1
testcase READ_1
testcase READ_2
testcase READ_3
testcase READ_4
testcase REMARK_1
testcase REM_1
# RENUM must be manually tested
# testcase RENUM_1
testcase RESTORE_1
testcase RESTORE_2
testcase RESUME_1
testcase RESUME_2
testcase RETURN_1
testcase RIGHT_1
testcase RIGHT_2
testcase RIGHT_3
testcase RND_1
testcase RND_2
testcase RUN_1
testcase RUN_2
testcase RUN_3
rm SAVE_1.DAT
testcase SAVE_1
testcase SIN_1
testcase SLEEP_1
testcase SLEEP_2
testcase SPACE_1
testcase SPACE_2
testcase SPACE_3
testcase SPA_1
testcase SPC_1
testcase SQRT_1
testcase SQRT_2
testcase SQR_1
testcase STEP_1
testcase STEP_2
testcase STEP_3
testcase STEP_4
testcase STOP_1
testcase STRING_1
testcase STRING_2
testcase STR_1
testcase STUFF_1
testcase SYSTEM_1
testcase TAB_1
testcase TAN_1
testcase TEXT_1
testcase THEN_1
testcase TIME_1
testcase TIME_2
testcase TIM_1
testcase TIM_2
testcase TIM_3
testcase TI_1
testcase TI_2
testcase TI_3
testcase TI_4
testcase TRACE_1
testcase TRACE_2
testcase TRACE_OFF_1
testcase TRACE_ON_1
testcase TROFF_1
testcase TRON_1
testcase TRON_2
testcase VAL_1
testcase VAL_2
testcase VTAB_1
testcase WAIT_1
testcase WAIT_2
testcase WAIT_3
# ----------------------------------------------
# EOF
# ----------------------------------------------
