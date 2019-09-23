--- stat_.h	2017-08-23 12:40:36.000000000 -0500
+++ stat.h	2017-11-25 16:34:21.480019000 -0600
@@ -1,6 +1,8 @@
 #define T_DIR  1   // Directory
 #define T_FILE 2   // File
 #define T_DEV  3   // Device
+#define E_FILE 4   // Extent File
+
 
 struct stat {
   short type;  // Type of file
@@ -8,4 +10,5 @@
   uint ino;    // Inode number
   short nlink; // Number of links to file
   uint size;   // Size of file in bytes
+  uint addrs[20];
 };
