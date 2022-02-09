# Puropose: Verify existing BWBASIC behavior
#   Author: Howard Wulf
#     Date: 2014-02-28
#    Usage: implementatino defined
#  Example:
#           cd /sdcard/Download/BASIC/bwbasic3/bwbtest
#           ash ./00test.sh
#

rm *.80
rm *.OUT
rm *.dif

# ----------------------------------------------
# Regression Tests
# ----------------------------------------------
testcase()
{
  TESTCASE=${1}
  echo "TESTCASE=${TESTCASE}"
  ~/bwbasic ${TESTCASE} > ${TESTCASE}.OUT
  echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
  ~/bwbasic PAD80.BAS
  diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
  if test -s  ${TESTCASE}.dif
  then
    echo    less ${TESTCASE}.dif
  fi
}

# ---------------------------------------------
echo "OPTION VERSION BYWATER" > profile.bas

testcase abs
testcase assign
testcase callfunc
testcase callsub
testcase chain1
testcase chain2
testcase dataread
testcase deffn
testcase dim
testcase doloop
testcase dowhile
testcase elseif
testcase end
testcase err
testcase fncallfn
testcase fornext
testcase function
testcase gosub
testcase gotolabl
testcase ifline
testcase input
testcase lof
testcase loopuntl
testcase main
testcase mlifthen
testcase on
testcase onerr
testcase onerrlbl
testcase ongosub
testcase opentest
testcase option
testcase pascaltr
testcase putget
testcase random
testcase selcase
testcase snglfunc
testcase stop
testcase term
testcase whilwend
testcase width
testcase writeinp


cat *.dif > dif.OUT
less dif.OUT

# EOF
