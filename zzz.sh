# Filename:  zzz.sh
#  Purpose:  execute all regression tests
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-05-09
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic-2.62/
#            ash ./zzz.sh

cd BP100
ash ./00test.sh
cd ..

cd SCBP/
ash ./00test.sh
cd ..

cd SKYTEL/
ash ./00test.sh
cd ..

cd bwbtest/
ash ./00test.sh
cd ..

cd NBS2/
ash ./00test.sh
ash ./01test.sh
cd ..

# EOF
