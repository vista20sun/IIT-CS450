--- defs_.h	2017-10-27 14:09:40.000000000 -0500
+++ defs.h	2017-11-29 13:20:13.588705200 -0600
@@ -122,6 +122,7 @@
 void            yield(void);
 int             procState(void);
 uint            uv2p_(char*);
+int             large_seek(struct file*,uint);
 
 // swtch.S
 void            swtch(struct context**, struct context*);
