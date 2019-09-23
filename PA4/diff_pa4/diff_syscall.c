--- syscall_.c	2017-10-27 14:26:52.000000000 -0500
+++ syscall.c	2017-11-25 17:39:53.673000000 -0600
@@ -105,6 +105,7 @@
 extern int sys_uptime(void);
 extern int sys_ps(void);
 extern int sys_uv2p(void);
+extern int sys_lseek(void);
 
 static int (*syscalls[])(void) = {
 [SYS_fork]    sys_fork,
@@ -130,6 +131,8 @@
 [SYS_close]   sys_close,
 [SYS_ps]      sys_ps,
 [SYS_uv2p]    sys_uv2p,
+[SYS_lseek]   sys_lseek,
+
 };
 
 void
