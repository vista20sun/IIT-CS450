--- user_.h	2017-11-03 15:44:38.000000000 -0500
+++ user.h	2017-11-25 17:32:09.974956000 -0600
@@ -25,6 +25,7 @@
 int uptime(void);
 int uv2p(char*);
 int ps(void);
+int lseek(int,int);
 
 // ulib.c
 int stat(char*, struct stat*);
