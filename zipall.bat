del bwbinc.zip
del bwbsrc.zip
del bwbjcl.zip
del all.zip
del bwbasic3.jcl
del alljcl.jcl
del alljcl.txt
zip -0 -X -ll -j bwbsrc.zip *.c
zip -0 -X -ll -j bwbinc.zip *.h
zip -0 -X -ll -j bwbjcl.zip *.jcl README
zip -0 -X all *.zip
