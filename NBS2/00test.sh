# Puropose:  Verify existing BWBASIC behavior
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-01-29
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic/NBS2
#            ash ./00test.sh
#
#

rm *.OUT
rm *.LPT
rm *.80
rm *.dif


# ----------------------------------------------
# Regression Tests
# ----------------------------------------------
testcase()
{
   TESTCASE=${1}
   echo "TESTCASE=${TESTCASE}"
   ~/bwbasic --tape ${TESTCASE}.INP --profile ECMA-55.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
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
#           ANSI/ECMA Minimal BASIC tests
#
# ----------------------------------------------
echo "OPTION VERSION ECMA-55" > ECMA-55.PRO

echo "OPTION VERSION BYWATER" > BYWATER.PRO
echo "OPTION LABELS OFF"     >> BYWATER.PRO

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
testcase P131
# echo P131 - RANDOMMIZE - MANUAL REVIEW REQUIRED
#  echo "OPTION VERSION ECMA-55" > profile.bas
#  ~/bwbasic P131 > P131.OUT
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
# EOF
# ----------------------------------------------
