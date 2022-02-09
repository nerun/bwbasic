# Puropose:  Verify existing BWBASIC behavior
#   Author:  Howard Wulf, AF5NE
#     Date:  2014-05-11
#    Usage:  implementatino defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/CPM
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
   ~/bwbasic --tape ${TESTCASE}.INP --profile M80.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT


   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile PAD80.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ---------------------------------------------
testcase 555-ic
testcase 567-ic
testcase airmiles
testcase astrnmy2
testcase atten
testcase audiolab
testcase bigcal2
testcase budget
testcase deprec
testcase finance
testcase hurricpm
testcase ic-timer
testcase interest
testcase intrate
testcase loan2
testcase log10k
testcase mortgage
# testcase ohmslaw
testcase rc5
testcase satelite
testcase xformer
# ----------------------------------------------
# EOF
# ----------------------------------------------
