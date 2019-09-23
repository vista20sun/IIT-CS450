--- syscall.c_	2017-10-03 07:50:24.000000000 -0500
+++ syscall.c	2017-10-27 15:26:52.783602000 -0500
@@ -104,6 +104,7 @@
 extern int sys_write(void);
 extern int sys_uptime(void);
 extern int sys_ps(void);
+extern int sys_uv2p(void);
 
 static int (*syscalls[])(void) = {
 [SYS_fork]    sys_fork,
@@ -128,6 +129,7 @@
 [SYS_mkdir]   sys_mkdir,
 [SYS_close]   sys_close,
 [SYS_ps]      sys_ps,
+[SYS_uv2p]    sys_uv2p,
 };
 
 void
