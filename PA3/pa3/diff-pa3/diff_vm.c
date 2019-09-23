--- vm.c_	2017-08-23 00:40:36.000000000 -0500
+++ vm.c	2017-11-07 20:29:32.456468700 -0600
@@ -356,6 +356,25 @@
   return (char*)P2V(PTE_ADDR(*pte));
 }
 
+
+
+uint uva2pa(pde_t *pgdir,char *uva)
+{
+  if((uint)uva>0x3FFFFF)
+    return -3;
+  int offset = 0xfff&((uint)uva);
+  uva = (char*)PGROUNDDOWN((uint)uva);
+  pte_t *pte;
+  pte = walkpgdir(pgdir, uva, 0);
+  if(pte==0)
+    return -2;
+  if((*pte & PTE_P) == 0)
+    return -1;
+  if((*pte & PTE_U) == 0)
+    return -1;
+  return offset + PTE_ADDR(*pte);
+}
+
 // Copy len bytes from p to user address va in page table pgdir.
 // Most useful when pgdir is not the current page table.
 // uva2ka ensures this only works for PTE_U pages.
