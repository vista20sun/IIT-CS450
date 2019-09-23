--- sysproc.c_	2017-09-26 22:37:34.000000000 -0500
+++ sysproc.c	2017-10-27 15:26:51.138000000 -0500
@@ -56,6 +56,15 @@
   return addr;
 }
 
+uint sys_uv2p(void)
+{
+  int addr;
+
+  if(argint(0, &addr) < 0)
+    return -1;
+  return uv2p_((char*)addr);
+}
+
 int
 sys_sleep(void)
 {
