--- defs.h_	2017-09-26 22:52:08.000000000 -0500
+++ defs.h	2017-10-27 15:09:41.801600000 -0500
@@ -121,6 +121,7 @@
 void            wakeup(void*);
 void            yield(void);
 int             procState(void);
+uint            uv2p_(char*);
 
 // swtch.S
 void            swtch(struct context**, struct context*);
@@ -186,6 +187,7 @@
 void            switchkvm(void);
 int             copyout(pde_t*, uint, void*, uint);
 void            clearpteu(pde_t *pgdir, char *uva);
+uint            uva2pa(pde_t*,char*);
 
 // number of elements in fixed-size array
 #define NELEM(x) (sizeof(x)/sizeof((x)[0]))
