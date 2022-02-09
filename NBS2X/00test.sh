# Filename:  00test.sh
#  Purpose:  automted regression testing
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-01-29
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic/NBS2X
#            ash ./00test.sh
#
#

rm *.OUT
rm *.LPT
rm *.80
rm *.dif
rm *.HTM

# E058, E059
rm A

# ----------------------------------------------
# Regression Tests
# ----------------------------------------------
testcase_old()
{
  TESTCASE=${1}
  echo "TESTCASE=${TESTCASE}"
  case ${TESTCASE} in
  E067)
    echo "OPTION VERSION BYWATER" > profile.bas
    echo "OPTION COMMENT \"!\""      >> profile.bas
    ;;
  E068)
    echo "OPTION VERSION BYWATER" > profile.bas
    echo "OPTION STATEMENT \"\\\""   >> profile.bas
    ;;
  F_VAL_A_N)
    echo "OPTION VERSION BYWATER" > profile.bas
    echo "OPTION BUGS OFF"       >> profile.bas
    ;;
  *)
    echo "OPTION VERSION BYWATER" > profile.bas
    echo "OPTION ROUND MATH"     >> profile.bas
    ;;
  esac
  ~/bwbasic ${TESTCASE} > ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
  echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
  echo "OPTION VERSION BYWATER"  > profile.bas
  ~/bwbasic PAD80.BAS
  diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
  if test -s  ${TESTCASE}.dif
  then
    echo    less ${TESTCASE}.dif
  fi
}

testcase()
{
   TESTCASE=${1}
   echo "TESTCASE=${TESTCASE}"
   ~/bwbasic --tape ${TESTCASE}.INP --profile BW.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile BYWATER.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ----------------------------------------------
#
# Extra tests of bwBASIC behavior
#
# ----------------------------------------------
echo "OPTION VERSION BYWATER" > BW.PRO



echo "OPTION VERSION BYWATER" > BYWATER.PRO
echo "OPTION LABELS OFF"     >> BYWATER.PRO

testcase E001
testcase E002
testcase E003
testcase E004
testcase E005
testcase E006
testcase E007
testcase E008
testcase E009
testcase E010
testcase E011
testcase E012
testcase E013
testcase E014
testcase E015
testcase E016
testcase E017
testcase E018
testcase E019
testcase E020
testcase E021
testcase E022
testcase E023
testcase E024
testcase E025
testcase E026
testcase E027
testcase E028
testcase E029
testcase E030
testcase E031
testcase E032
testcase E033
testcase E034
testcase E035
testcase E036
testcase E037
testcase E038
testcase E039
testcase E040
testcase E041
testcase E042
testcase E043
testcase E044
testcase E045
testcase E046
testcase E047
testcase E048
testcase E049
testcase E050
testcase E051
testcase E052
testcase E053
testcase E054
testcase E055
testcase E056
testcase E057
testcase E058
testcase E059
testcase E060
testcase E061
testcase E062
testcase E063
testcase E064
testcase E065
testcase E066
testcase E067
testcase E068
testcase E069
testcase E070
testcase E071


testcase M001
testcase M002
testcase M003
# ON TIMER ...
# testcase M004
# ON TIMER ...
# testcase M005
testcase M006
testcase M007
testcase MID_1

# ----------------------------------------------
#
#    SYNTAX.BAS generates the following tests
#
# ----------------------------------------------
testcase F_ABS_X_N
testcase F_ACOS_X_N
testcase F_ANGLE_X_Y_N
testcase F_ASC_A_N
testcase F_ASIN_X_N
testcase F_ATN_X_N
testcase F_CCUR_X_N
testcase F_CDBL_X_N
testcase F_CEIL_X_N
testcase F_CHR_X_S
testcase F_CINT_X_N
testcase F_CLNG_X_N
testcase F_COSH_X_N
testcase F_COS_X_N
testcase F_COT_X_N
testcase F_CSC_X_N
testcase F_CSNG_X_N
testcase F_CVC_A_N
testcase F_CVD_A_N
testcase F_CVI_A_N
testcase F_CVL_A_N
testcase F_CVS_A_N
testcase F_DEG_X_N
testcase F_EPS_X_N
testcase F_ERROR_X_A_N
testcase F_ERROR_X_N
testcase F_EXP_X_N
testcase F_FIX_X_N
testcase F_FP_X_N
testcase F_FRE_A_N
testcase F_FRE_X_N
testcase F_HEX_X_S
testcase F_INSTR_A_B_N
testcase F_INSTR_X_A_B_N
testcase F_INT_X_N
testcase F_IP_X_N
testcase F_LCASE_A_S
testcase F_LEFT_A_X_S
testcase F_LEN_A_N
testcase F_LOG10_X_N
testcase F_LOG2_X_N
testcase F_LOG_X_N
testcase F_LTRIM_A_S
testcase F_MAXLEN_A_N
testcase F_MAX_X_Y_N
testcase F_MID_A_X_S
testcase F_MID_A_X_Y_S
testcase F_MIN_X_Y_N
testcase F_NULL_X_N
testcase F_OCT_X_S
testcase F_ORD_A_N
testcase F_RAD_X_N
testcase F_REMAINDER_X_Y_N
testcase F_REPEAT_X_A_S
testcase F_REPEAT_X_Y_S
testcase F_RIGHT_A_X_S
testcase F_ROUND_X_Y_N
testcase F_RTRIM_A_S
testcase F_SEC_X_N
testcase F_SGN_X_N
testcase F_SINH_X_N
testcase F_SIN_X_N
testcase F_SPACE_X_S
testcase F_SQR_X_N
testcase F_STRING_X_A_S
testcase F_STRING_X_Y_S
testcase F_STR_X_S
testcase F_TANH_X_N
testcase F_TAN_X_N
testcase F_TRIM_A_S
testcase F_TRUNCATE_X_Y_N
testcase F_UCASE_A_S
testcase F_VAL_A_N
# ----------------------------------------------
# other things we want to do here
# ----------------------------------------------
# echo "OPTION VERSION BYWATER" > profile.bas
# echo "OPTION BUGS OFF"       >> profile.bas

echo "CMDS"
~/bwbasic CMDS.BAS > CMDS.OUT
~/bwbasic CMDS_HTM.BAS 2> CMDS.HTM

echo "FNCS"
~/bwbasic FNCS.BAS > FNCS.OUT
~/bwbasic FNCS_HTM.BAS 2> FNCS.HTM

echo "MAINTAINER"
~/bwbasic MAINTAINER.BAS 2> MAINTAINER.OUT

echo "MANUAL"
~/bwbasic MANUAL.BAS 2> MANUAL.OUT


# grep "SYNTAX:" MANUAL.OUT > SYNTAX.INP
# ~/bwbasic SYNTAX.BAS > SYNTAX.OUT

# ----------------------------------------------
# EOF
# ----------------------------------------------
