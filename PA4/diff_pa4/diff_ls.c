--- ls_.c	2017-08-23 12:40:36.000000000 -0500
+++ ls.c	2017-11-29 13:38:24.814219200 -0600
@@ -36,7 +36,7 @@
   }
 
   if(fstat(fd, &st) < 0){
-    printf(2, "ls: cannot stat %s\n", path);
+    printf(2, "ls: cannot stat - %s\n", path);
     close(fd);
     return;
   }
@@ -46,6 +46,23 @@
     printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
     break;
 
+  case E_FILE:
+    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
+    if(st.addrs == 0){
+      printf(1,"  ----empty file\n");
+      break;
+    }
+    int i,size = 0;
+    for(i=0;i<= NDIRECT;i++){
+      if(st.addrs[i]==0)
+        break;
+      int blksize = ELENGTH(st.addrs[i])*512>(st.size-size)?(st.size-size):ELENGTH(st.addrs[i])*512;
+      size += blksize;
+      printf(1,"  ----blocks @ entry #%d, length:%d,size %d,start at:0x%x\n",i,ELENGTH(st.addrs[i]),blksize,EADDRESS(st.addrs[i]));
+    }
+    printf(1,"total %d blocks\n",1);
+    free(st.addrs);
+	break;
   case T_DIR:
     if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
       printf(1, "ls: path too long\n");
