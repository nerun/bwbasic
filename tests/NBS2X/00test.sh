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
# rm *.HTM
# rm *.MAN
rm *.DAT

# E058, E059
rm A

# ----------------------------------------------
# Regression Tests
# ----------------------------------------------
testcase()
{
   TESTCASE=${1}


   echo "TESTCASE=${TESTCASE}"
   ~/bwbasic --tape ${TESTCASE}.INP --profile B15.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT


   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile PAD80.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ----------------------------------------------
#
# Extra tests of BYWATER BASIC behavior
#
# ---------------------------------------------
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
echo this is the first > E044.DAT
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
# this test is only valid when MAX_LINE_LENGTH = 4096
# testcase E054
testcase E055
testcase E056
testcase E057
rm A
testcase E058
testcase E059
testcase E060
testcase E061
echo > E062.DAT
testcase E062
testcase E063
testcase E064
# this test depends upon MAXLEN
# testcase E065
testcase E066
testcase E067
testcase E068
testcase E069
testcase E070
testcase E071
testcase M001
testcase M002
testcase M003
testcase M004
testcase M005
testcase M006
testcase M007
testcase MID_1
testcase VIRT_1
testcase VIRT_2
testcase VIRT_3
testcase VIRT_4
testcase VIRT_5
testcase VIRT_6
echo ABCDEFGHIJKLMNOPQRSTUVWXYZ > VIRT_7.DAT
testcase VIRT_7

# ----------------------------------------------
#
# SYNTAX.BAS generated the following tests
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
# results depend upon sizeof(IntegerType)
testcase F_CINT_X_N
testcase F_CLNG_X_N
testcase F_COSH_X_N
testcase F_COS_X_N
testcase F_COT_X_N
testcase F_CSC_X_N
testcase F_CSNG_X_N
testcase F_CVC_A_N
testcase F_CVD_A_N
# results depend upon sizeof(IntegerType)
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
# results depend upon sizeof(IntegerType)
testcase F_HEX_X_S
testcase F_INSTR_A_B_N
# results depend upon MAXLEN
# testcase F_INSTR_X_A_B_N
testcase F_INT_X_N
testcase F_IP_X_N
testcase F_LCASE_A_S
# results depend upon MAXLEN
# testcase F_LEFT_A_X_S
testcase F_LEN_A_N
testcase F_LOG10_X_N
testcase F_LOG2_X_N
testcase F_LOG_X_N
testcase F_LTRIM_A_S
# results depend upon MAXLEN
# testcase F_MAXLEN_A_N
testcase F_MAX_X_Y_N
# results depend upon MAXLEN
# testcase F_MID_A_X_S
# results depend upon MAXLEN
# testcase F_MID_A_X_Y_S
testcase F_MIN_X_Y_N
testcase F_NULL_X_N
# results depend upon sizeof(IntegerType)
testcase F_OCT_X_S
testcase F_ORD_A_N
testcase F_RAD_X_N
testcase F_REMAINDER_X_Y_N
# this test is only valid when MAX_LINE_LENGTH = 4096
# testcase F_REPEAT_X_A_S
# this test is only valid when MAX_LINE_LENGTH = 4096
# testcase F_REPEAT_X_Y_S
# results depend upon MAXLEN
# testcase F_RIGHT_A_X_S
testcase F_ROUND_X_Y_N
testcase F_RTRIM_A_S
testcase F_SEC_X_N
testcase F_SGN_X_N
testcase F_SINH_X_N
testcase F_SIN_X_N
# this test is only valid when MAX_LINE_LENGTH = 4096
# testcase F_SPACE_X_S
testcase F_SQR_X_N
# this test is only valid when MAX_LINE_LENGTH = 4096
# testcase F_STRING_X_A_S
# this test is only valid when MAX_LINE_LENGTH = 4096
# testcase F_STRING_X_Y_S
testcase F_STR_X_S
testcase F_TANH_X_N
testcase F_TAN_X_N
testcase F_TRIM_A_S
testcase F_TRUNCATE_X_Y_N
testcase F_UCASE_A_S
testcase F_VAL_A_N

# ----------------------------------------------
# dump the command cross-reference
# ----------------------------------------------
echo "CMDS"
~/bwbasic CMDS.BAS > CMDS.OUT
~/bwbasic CMDS_HTM.BAS 2> ../DOCS/CMDS.HTM
# cp CMDS.HTM ..

# ----------------------------------------------
# dump the function cross-reference
# ----------------------------------------------
echo "FNCS"
~/bwbasic FNCS.BAS > FNCS.OUT
~/bwbasic FNCS_HTM.BAS 2> ../DOCS/FNCS.HTM
# cp FNCS.HTM ..

# ----------------------------------------------
# dump all interanl tables
# ----------------------------------------------
echo "MAINTAINER"
~/bwbasic MAINTAINER.BAS 2> MAINTAINER.OUT
# cp MAINTAINER.OUT ..

# ----------------------------------------------
# dump the master manual of all dialects
# ----------------------------------------------
echo "MANUAL"
~/bwbasic MANUAL.BAS 2> ../DOCS/ALL.txt

# ----------------------------------------------
# dump the user manuals for each dialect
# ----------------------------------------------
# echo "DIALECTS"
echo "MAINTAINER MANUAL                   " > DIALECT.BAS

echo "OPTION VERSION \"BYWATER\"          "
echo "OPTION VERSION \"BYWATER\"          "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/BYWATER.txt

echo "OPTION VERSION \"BYWATER-2\"        "
echo "OPTION VERSION \"BYWATER-2\"        "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/BYWATER-2.txt

echo "OPTION VERSION \"CALL/360\"          "
echo "OPTION VERSION \"CALL/360\"          " > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/CALL-360.txt

echo "OPTION VERSION \"CBASIC-II\"        "
echo "OPTION VERSION \"CBASIC-II\"        "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/CBASIC-II.txt

echo "OPTION VERSION \"DARTMOUTH\"        "
echo "OPTION VERSION \"DARTMOUTH\"        "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/DARTMOUTH.txt

echo "OPTION VERSION \"ECMA-55\"          "
echo "OPTION VERSION \"ECMA-55\"          "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/ECMA-55.txt

echo "OPTION VERSION \"ECMA-116\"         "
echo "OPTION VERSION \"ECMA-116\"         "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/ECMA-116.txt

echo "OPTION VERSION \"GCOS\"             "
echo "OPTION VERSION \"GCOS\"             "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/GCOS.txt

echo "OPTION VERSION \"HAARDT\"            "
echo "OPTION VERSION \"HAARDT\"            "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/HAARDT.txt

echo "OPTION VERSION \"HANDBOOK1\"        "
echo "OPTION VERSION \"HANDBOOK1\"        "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/HANDBOOK1.txt

echo "OPTION VERSION \"HANDBOOK2\"        "
echo "OPTION VERSION \"HANDBOOK2\"        "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/HANDBOOK2.txt

echo "OPTION VERSION \"HEATH\"            "
echo "OPTION VERSION \"HEATH\"            "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/HEATH.txt

echo "OPTION VERSION \"IBASIC\"           "
echo "OPTION VERSION \"IBASIC\"           "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/IBASIC.txt

echo "OPTION VERSION \"MARK-I\"           "
echo "OPTION VERSION \"MARK-I\"           "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/MARK-I.txt

echo "OPTION VERSION \"MARK-II\"          "
echo "OPTION VERSION \"MARK-II\"          "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/MARK-II.txt

echo "OPTION VERSION \"MBASIC\"           "
echo "OPTION VERSION \"MBASIC\"           "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/MBASIC.txt

echo "OPTION VERSION \"PDP-8\"            "
echo "OPTION VERSION \"PDP-8\"            "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/PDP-8.txt

echo "OPTION VERSION \"PDP-11\"           "
echo "OPTION VERSION \"PDP-11\"           "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/PDP-11.txt

echo "OPTION VERSION \"RBASIC\"           "
echo "OPTION VERSION \"RBASIC\"           "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/RBASIC.txt

echo "OPTION VERSION \"RSTS-11\"          "
echo "OPTION VERSION \"RSTS-11\"          "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/RSTS-11.txt

echo "OPTION VERSION \"SYSTEM/360\"       "
echo "OPTION VERSION \"SYSTEM/360\"       "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/SYSTEM-360.txt

echo "OPTION VERSION \"SYSTEM/370\"       "
echo "OPTION VERSION \"SYSTEM/370\"       "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/SYSTEM-370.txt

echo "OPTION VERSION \"TRS-80\"           "
echo "OPTION VERSION \"TRS-80\"           "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/TRS-80.txt

echo "OPTION VERSION \"VINTAGE\"          "
echo "OPTION VERSION \"VINTAGE\"          "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/VINTAGE.txt

echo "OPTION VERSION \"XBASIC\"           "
echo "OPTION VERSION \"XBASIC\"           "  > DIALECT.PRO
~/bwbasic --profile DIALECT.PRO DIALECT.BAS 2> ../DOCS/XBASIC.txt

# grep "SYNTAX:" MANUAL.OUT > SYNTAX.INP
# ~/bwbasic SYNTAX.BAS > SYNTAX.OUT

# ----------------------------------------------
# EOF
# ----------------------------------------------
