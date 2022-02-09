# Filename:  yyy.sh
#  Purpose:  clean all automated regression tests
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-12-24
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/
#            ash ./yyy.sh

# ----------------------------------------------
# Run Automated Regression Tests
# ----------------------------------------------

cd B15A/
ash ./99test.sh
cd ..

cd B15B/
ash ./99test.sh
cd ..

cd C77A/
ash ./99test.sh
cd ..

cd C77B/
ash ./99test.sh
cd ..

cd G65/
ash ./99test.sh
cd ..

cd HB1/
ash ./99test.sh
cd ..

cd HB1X/
ash ./99test.sh
cd ..

cd HB2/
ash ./99test.sh
cd ..

cd HB2X/
ash ./99test.sh
cd ..

cd I70/
ash ./99test.sh
cd ..

cd M80/
ash ./99test.sh
cd ..

cd NBS2/
ash ./99test.sh
cd ..

cd NBS2X/
ash ./99test.sh
cd ..

cd T80B/
ash ./99test.sh
cd ..

cd T80A/
ash ./99test.sh
cd ..

cd T80C/
ash ./99test.sh
cd ..


# ----------------------------------------------
# EOF
# ----------------------------------------------

