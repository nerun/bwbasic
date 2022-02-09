# Filename:  yyy.sh
#  Purpose:  automted regression testing
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-01-29
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic/
#            ash ./yyy.sh
#
#

# cd NBS2

rm *.OUT
rm *.80
rm *.dif

# E058, E059
rm A

# ----------------------------------------------
# Regression Tests
# ----------------------------------------------
testcase()
{
  TESTCASE=${1}
  echo "TESTCASE=${TESTCASE}"
  case ${TESTCASE} in
  P* )
    echo "OPTION VERSION ECMA-55" > profile.bas
    ;;
  E067)
    echo "OPTION VERSION BYWATER" > profile.bas
    echo "OPTION COMMENT !"      >> profile.bas
    ;;
  E068)
    echo "OPTION VERSION BYWATER" > profile.bas
    echo "OPTION STATEMENT \\"   >> profile.bas
    ;;
  F_VAL_A_N)
    echo "OPTION VERSION BYWATER" > profile.bas
    echo "OPTION BUGS OFF"       >> profile.bas
    ;;
  *)
    echo "OPTION VERSION BYWATER" > profile.bas
    ;;
  esac
  ~/bwbasic ${TESTCASE} > ${TESTCASE}.OUT
  echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
  echo "OPTION VERSION BYWATER"  > profile.bas
  ~/bwbasic PAD80.BAS
  diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
  if test -s  ${TESTCASE}.dif
  then
    echo    less ${TESTCASE}.dif
  fi
}
# ----------------------------------------------
#
#           ANSI/ECMA Minimal BASIC tests
#
# ----------------------------------------------
testcase P001
testcase P002
testcase P003
testcase P004
testcase P005
testcase P006
testcase P007
testcase P008
testcase P009
testcase P010
testcase P011
testcase P012
testcase P013
testcase P014
testcase P015
testcase P016
testcase P017
testcase P018
testcase P019
testcase P020
testcase P021
testcase P022
testcase P023
testcase P024
testcase P025
testcase P026
testcase P027
testcase P028
testcase P029
testcase P030
testcase P031
testcase P032
testcase P033
testcase P034
testcase P035
testcase P036
testcase P037
testcase P038
testcase P039
testcase P040
testcase P041
testcase P042
testcase P043
testcase P044
testcase P045
testcase P046
testcase P047
testcase P048
testcase P049
testcase P050
testcase P051
testcase P052
testcase P053
testcase P054
testcase P055
testcase P056
testcase P057
testcase P058
testcase P059
testcase P060
testcase P061
# ----------------------------------------------
#
#       P062 should ERROR on line 550
#
#            This is by design 
#
#        DIM is an executed command
#
# ----------------------------------------------
echo "By design, P062 SHOULD ERROR on line 550: SUBSCRIPT OUT OF RANGE E(14)"
testcase P062
testcase P063
testcase P064
testcase P065
testcase P066
testcase P067
testcase P068
testcase P069
testcase P070
testcase P071
testcase P072
testcase P073
testcase P074
testcase P075
testcase P076
testcase P077
testcase P078
testcase P079
testcase P080
testcase P081
testcase P082
testcase P083
testcase P084
testcase P085
testcase P086
testcase P087
testcase P088
testcase P089
testcase P090
testcase P091
testcase P092
testcase P093
testcase P094
testcase P095
testcase P096
testcase P097
testcase P098
testcase P099
testcase P100
testcase P101
testcase P102
testcase P103
testcase P104
testcase P105
testcase P106
testcase P107
testcase P108
testcase P109
testcase P110
testcase P111
testcase P112
testcase P113
testcase P114
testcase P115
testcase P116
testcase P117
testcase P118
testcase P119
testcase P120
testcase P121
testcase P122
testcase P123
testcase P124
testcase P125
testcase P126
testcase P127
testcase P128
testcase P129
testcase P130
# ----------------------------------------------
#
#    The results of P131 should chaange 
#    every time the program runs.
#
# ----------------------------------------------
echo P131 - RANDOMMIZE - MANUAL REVIEW REQUIRED
# testcase P131
  ~/bwbasic P131 > P131.OUT
testcase P132
testcase P133
testcase P134
testcase P135
testcase P136
testcase P137
testcase P138
testcase P139
testcase P140
testcase P141
testcase P142
testcase P143
testcase P144
testcase P145
testcase P146
testcase P147
testcase P148
testcase P149
testcase P150
testcase P151
testcase P152
testcase P153
testcase P154
testcase P155
testcase P156
testcase P157
testcase P158
testcase P159
testcase P160
testcase P161
testcase P162
testcase P163
testcase P164
testcase P165
testcase P166
testcase P167
testcase P168
testcase P169
testcase P170
testcase P171
testcase P172
testcase P173
testcase P174
testcase P175
testcase P176
testcase P177
testcase P178
testcase P179
testcase P180
testcase P181
testcase P182
testcase P183
testcase P184
testcase P185
testcase P186
testcase P187
testcase P188
testcase P189
testcase P190
testcase P191
testcase P192
testcase P193
testcase P194
testcase P195
testcase P196
testcase P197
testcase P198
testcase P199
testcase P200
testcase P201
testcase P202
testcase P203
testcase P204
testcase P205
testcase P206
testcase P207
testcase P208
# ----------------------------------------------
#
# Extra tests of bwBASIC behavior
#
# ----------------------------------------------
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
# EOF
# ----------------------------------------------
