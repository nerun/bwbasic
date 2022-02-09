# Filename:  zzz.sh
#  Purpose:  execute all automated regression tests
#   Author:  Howard Wulf, AF5NE
#     Date:  2015-05-14
#   Uasage:  implementation defined
#  Example:
#            cd /sdcard/Download/BASIC/bwbasic3/
#            ash ./zzz.sh

# ----------------------------------------------
# Run Automated Regression Tests
# ----------------------------------------------

cd B15A/
ash ./00test.sh
cd ..

cd B15B/
ash ./00test.sh
cd ..

cd B93/
ash ./00test.sh
cd ..

cd C77A/
ash ./00test.sh
cd ..

cd C77B/
ash ./00test.sh
cd ..

# cd D71/
# ash ./00test.sh
# cd ..

cd D73/
ash ./00test.sh
cd ..

cd G65/
ash ./00test.sh
cd ..

cd H80/
ash ./00test.sh
cd ..

cd HB1/
ash ./00test.sh
cd ..

cd HB1X/
ash ./00test.sh
cd ..

cd HB2/
ash ./00test.sh
cd ..

cd HB2X/
ash ./00test.sh
cd ..

cd I70/
ash ./00test.sh
cd ..

cd M80/
ash ./00test.sh
cd ..

cd NBS2/
ash ./00test.sh
cd ..

cd NBS2X/
ash ./00test.sh
cd ..

cd R86/
ash ./00test.sh
cd ..

cd T80A/
ash ./00test.sh
cd ..

cd T80B/
ash ./00test.sh
cd ..

cd T80C/
ash ./00test.sh
cd ..


# ----------------------------------------------
# Review Errors
# ----------------------------------------------

cd B15A/
ash ./01test.sh
cd ..

cd B15B/
ash ./01test.sh
cd ..

cd B93/
ash ./01test.sh
cd ..

cd C77A/
ash ./01test.sh
cd ..

cd C77B/
ash ./01test.sh
cd ..

# cd D71/
# ash ./01test.sh
# cd ..

cd D73/
ash ./01test.sh
cd ..

cd G65/
ash ./01test.sh
cd ..

cd H80/
ash ./01test.sh
cd ..

cd HB1/
ash ./01test.sh
cd ..

cd HB1X/
ash ./01test.sh
cd ..

cd HB2/
ash ./01test.sh
cd ..

cd HB2X/
ash ./01test.sh
cd ..

cd I70/
ash ./01test.sh
cd ..

cd M80/
ash ./01test.sh
cd ..

cd NBS2/
ash ./01test.sh
cd ..

cd NBS2X/
ash ./01test.sh
cd ..

cd R86/
ash ./01test.sh
cd ..

cd T80A/
ash ./01test.sh
cd ..

cd T80B/
ash ./01test.sh
cd ..

cd T80C/
ash ./01test.sh
cd ..

# ----------------------------------------------
# EOF
# ----------------------------------------------
