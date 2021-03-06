# Puropose: Verify existing BWBASIC behavior
#   Author: Howard Wulf
#     Date: 2014-03-28
#    Usage: implementatino defined
#  Example:
#           cd /sdcard/Download/BASIC/bwbasic3/SKYTEL
#           ash ./00test.sh
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
   ~/bwbasic --tape ${TESTCASE}.INP --profile B15.PRO --profile ${TESTCASE}.PRO ${TESTCASE}.BAS 1> ${TESTCASE}.OUT 2> ${TESTCASE}.LPT
   
   
   echo "500 DATA ${TESTCASE}.OUT, ${TESTCASE}.80" > PAD80.INP
   ~/bwbasic --profile PAD80.PRO PAD80.BAS


   diff ${TESTCASE}.run ${TESTCASE}.80 > ${TESTCASE}.dif
   if test -s  ${TESTCASE}.dif
   then
      echo    less ${TESTCASE}.dif
   fi
}

# ---------------------------------------------
#
# various dialects of BASIC
#


testcase addmag
testcase altaz
testcase altaz2
testcase angsep
testcase asteroid
testcase binary
testcase blkhole1
testcase blkhole2
testcase blkhole3
testcase calendar
testcase caljd
testcase capture
testcase ccdlimi2
testcase ccdlimit
testcase chance
testcase chart
testcase circle
testcase comet
testcase crater
testcase daysold
testcase dial
testcase difpat
testcase easter
testcase extinc
testcase facecirc
testcase fireball
testcase fracts
testcase gflash
testcase glob1
testcase gmst
testcase gwmonth
testcase interp
testcase jdcal
testcase jmerid
testcase kepler
testcase lens
testcase lheight
testcase limmag
testcase lookback
testcase lookbak2
testcase lunar
testcase mallam
testcase mars
testcase meteor
testcase moonfx
testcase moons
testcase moonup
testcase msachart
testcase obscur
testcase occvis
testcase orbits
testcase path
testcase period
testcase photom
testcase precess
testcase refr1
testcase rocket
testcase rotate
testcase saros
testcase satrings
testcase scales
testcase shadow
testcase shower
testcase shuttr
testcase solarecl
testcase space
testcase sphe
testcase stay
testcase steppr
testcase stereo
testcase sunshine
testcase suntan
testcase sunup
testcase supernum
testcase surf
testcase taki
testcase track
testcase vislimit
testcase wavel
testcase xyz
# ----------------------------------------------
# EOF
# ----------------------------------------------
