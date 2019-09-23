--- proc.c_	2017-10-03 08:09:56.000000000 -0500
+++ proc.c	2017-10-27 15:16:45.377090000 -0500
@@ -560,3 +560,8 @@
   release(&ptable.lock);
   return 0;
 }
+
+uint uv2p_(char *uva){
+  struct proc *curproc = myproc();
+  return uva2pa(curproc->pgdir, uva);
+}
\ No newline at end of file
