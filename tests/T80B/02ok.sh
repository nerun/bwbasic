# Puropose: Promote current results to regression
#   Author: Howard Wulf
#     Date: 2014-03-28
#    Usage: implementatino defined
#  Example:
#           cd /sdcard/Download/BASIC/bwbasic3/SCBP
#           ash ./01ok.sh
#

for f in *.80; do mv "$f" "${f/.80/.run}"; done
# EOF
