--- user.h_	2017-08-23 13:40:36.000000000 -0500
+++ user.h	2017-11-07 20:52:58.579561400 -0600
@@ -23,6 +23,8 @@
 char* sbrk(int);
 int sleep(int);
 int uptime(void);
+int uv2p(char*);
+int ps(void);
 
 // ulib.c
 int stat(char*, struct stat*);
