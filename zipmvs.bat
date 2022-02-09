del bwbinc.zip
del bwbsrc.zip
del bwbjcl.zip
del all.zip
del alljcl.jcl
zip -9 -X -ll -j bwbsrc.zip *.c
zip -9 -X -ll -j bwbinc.zip *.h
zip -9 -X -ll -j bwbjcl.zip *.jcl README COPYING bwbasic.doc
zip -9 -X all *.zip
