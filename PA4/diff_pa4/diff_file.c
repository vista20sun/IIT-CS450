--- file_.c	2017-08-23 12:40:36.000000000 -0500
+++ file.c	2017-11-25 17:36:17.024610000 -0600
@@ -112,6 +112,8 @@
   panic("fileread");
 }
 
+
+
 //PAGEBREAK!
 // Write to file f.
 int
@@ -130,7 +132,7 @@
     // and 2 blocks of slop for non-aligned writes.
     // this really belongs lower down, since writei()
     // might be writing a device like the console.
-    int max = ((LOGSIZE-1-1-2) / 2) * 512;
+    int max = ((LOGSIZE-1-1-2) / 2) * 512;//6656
     int i = 0;
     while(i < n){
       int n1 = n - i;
