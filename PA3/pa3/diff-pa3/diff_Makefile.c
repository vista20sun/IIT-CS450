--- Makefile_	2017-09-26 22:46:24.000000000 -0500
+++ Makefile	2017-10-27 15:24:24.056366000 -0500
@@ -177,6 +177,7 @@
 	_sh_xv\
 	_test\
 	_ps\
+	_testup\
 	
 fs.img: mkfs README $(UPROGS)
 	./mkfs fs.img README $(UPROGS)
