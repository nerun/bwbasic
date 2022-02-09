# Puropose:  Verify existing BWBASIC behavior
#   Author:  Howard Wulf, AF5NE
#     Date:  2014-03-28
#    Usage:  implementatino defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/BP100
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
   ~/bwbasic --tape ${TESTCASE}.INP --profile TRS-80.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile BYWATER.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ---------------------------------------------
#
# TRS-80 Model I/III/4 with 24 row x 80 column display
#
echo "OPTION VERSION TRS-80"  > TRS-80.PRO
echo "LET PI=3.141592654"    >> TRS-80.PRO
echo "WIDTH 24, 80"          >> TRS-80.PRO

echo "OPTION VERSION BYWATER" > BYWATER.PRO
echo "OPTION LABELS OFF"     >> BYWATER.PRO

testcase ACCTPAY
testcase ACCTREC
testcase ANNU1
testcase ANNUDEF
testcase ARBCOMP
testcase ASSIGN
testcase AUTOEXP
testcase BAYES
testcase BETAALPH
testcase BLACKSH
testcase BONDVAL
testcase BONDVAL2
testcase BREAKEVN
testcase BUSBUD
testcase CAP1
testcase CHECK2
testcase CHECKBK1
testcase COMPBAL
testcase CONDPROF
testcase CVP
testcase DATE
testcase DATEVAL
testcase DAYYEAR
testcase DEPLETE
testcase DEPRDB
testcase DEPRDDB
testcase DEPRSF
testcase DEPRSL
testcase DEPRSY
testcase DILANAL
testcase DISCBAL
testcase EFFECT
testcase ENVELOPE
testcase EOQ
testcase EPSEST
testcase EXPVAL
testcase FINRAT
testcase FQEOQ
testcase FQEOQPB
testcase FQEOWSH
testcase FUPRINF
testcase FVAL
testcase INSFILE
testcase INVENT2
testcase INVOICE
testcase LABEL1
testcase LABEL2
testcase LEASEINT
testcase LETWRT
testcase LOANAFFD
testcase LOANPAY
testcase MAILPAC
testcase MARKUP
testcase MERGANAL
testcase MORTGAGE
testcase MULTMON
testcase NCFANAL
testcase NPV
testcase OPTLOSS
testcase OPTWRITE
testcase PAYNET
testcase PAYROLL2
testcase PORTVAL9
testcase PRINDLAS
testcase PRINDPA
testcase PROFIND
testcase PVAL
testcase QUEUE1
testcase QUEUECB
testcase REGWITH
testcase RENTPRCH
testcase RRCONST
testcase RRCONVBD
testcase RRVARIN
testcase RTVAL
testcase RULE78
testcase SALELEAS
testcase SALVAGE
testcase SEASIND
testcase SELLPR
testcase SHARPE1
testcase SIMPDISK
testcase SIMPLEX
testcase SINKFUND
testcase SORT3
testcase STOCVAL1
testcase TAXDEP
testcase TELDIR
testcase TERMSPAY
testcase TIMECLCK
testcase TIMEMOV
testcase TIMETR
testcase TIMUSAN
testcase TRANS
testcase UPSZONE
testcase UTILITY
testcase VALADINF
testcase VALPRINF
testcase WACC
testcase WARVAL
# ----------------------------------------------
# EOF
# ----------------------------------------------
