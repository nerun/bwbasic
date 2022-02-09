# Puropose: Verify existing BWBASIC behavior
#   Author: Howard Wulf
#     Date: 2014-03-31
#    Usage: implementatino defined
#  Example:
#           cd /sdcard/Download/BASIC/bwbasic3/SCBP
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
echo "OPTION LABELS OFF"     >> profile.bas




testcase ALFASORT
testcase AMORTTAB
testcase ANAL2VEC
testcase ANGLMET
testcase AREAPOLY
testcase BINODIST
testcase CHECWRIT
testcase CHI2DIST
testcase CHI2TEST
testcase COMMPAP
testcase CORDCONV
testcase CURVINT
testcase DAY2DAY
testcase DAYWEEK
testcase DEGRAD
testcase DEPAMT
testcase DEPRATE
testcase DEPRSKED
testcase DERIV
testcase EARNINT
testcase EFFINT
testcase EXPREGR
testcase FDIST
testcase FUTVALDE
testcase FUTVALIN
testcase GCD
testcase GEOMEAN
testcase GEOREGR
testcase GROWRATE
testcase INITINV
testcase INTGGAUS
testcase INTGSIMP
testcase INTGTRAP
testcase LABELS
testcase LASTPAY
testcase LINCORCO
testcase LINEPROG
testcase LININT
testcase LINREGR
testcase LOANBAL
testcase LOANPAY
testcase LOANRATE
testcase LOANTERM
testcase MANNWHIT
testcase MAPCHECK
testcase MATADD
testcase MATINV
testcase MATMULT
testcase MININV
testcase MLINREGR
testcase NOMINT
testcase NORDREGR
testcase NORMDIST
testcase OPS2VEC
testcase PARTSTRI
testcase PERMCOMB
testcase PLOTFUNC
testcase POISDIST
testcase PRIFACT
testcase PRINLOAN
testcase QUADROOT
testcase RADDEG
testcase RECICOST
testcase REGDEP
testcase REGWDRAW
testcase ROOTNEWT
testcase ROOTPOLY
testcase SALVAGE
testcase SIMLEQNS
testcase STDDEV
testcase SYSREL
testcase TDIST
testcase TDISTEST
testcase TRIGPOLY




cat *.dif > dif.OUT
less dif.OUT

# EOF
