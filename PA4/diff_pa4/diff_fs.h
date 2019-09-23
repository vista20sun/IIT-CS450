--- fs_.h	2017-08-23 12:40:36.000000000 -0500
+++ fs.h	2017-11-25 15:30:01.407855000 -0600
@@ -4,6 +4,7 @@
 
 #define ROOTINO 1  // root i-number
 #define BSIZE 512  // block size
+#define EXTMAXBLKSIZE 50
 
 // Disk layout:
 // [ boot block | super block | log | inode blocks |
@@ -24,6 +25,11 @@
 #define NDIRECT 12
 #define NINDIRECT (BSIZE / sizeof(uint))
 #define MAXFILE (NDIRECT + NINDIRECT)
+#define MAXEFILE (NDIRECT * EXTMAXBLKSIZE)
+
+#define ELENGTH(addr) (addr & 0xFF)               //length (blocks) of Extent file 
+#define EADDRESS(addr) ((addr>>8)&0xFFFFFF)       //address of Extent files' blocks
+#define TOEADDR(addr, length) ((addr<<8)|(length&0xFF)) 
 
 // On-disk inode structure
 struct dinode {
