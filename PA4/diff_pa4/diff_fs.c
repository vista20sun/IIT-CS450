--- fs_.c	2017-08-23 12:40:36.000000000 -0500
+++ fs.c	2017-11-29 13:14:05.089782100 -0600
@@ -54,7 +54,7 @@
 
 // Allocate a zeroed disk block.
 static uint
-balloc(uint dev)
+balloc_(uint dev, int ext)
 {
   int b, bi, m;
   struct buf *bp;
@@ -63,6 +63,11 @@
   for(b = 0; b < sb.size; b += BPB){
     bp = bread(dev, BBLOCK(b, sb));
     for(bi = 0; bi < BPB && b + bi < sb.size; bi++){
+      if(ext&&(b+bi)>(0xFFFFFF))
+      {
+        brelse(bp);
+        panic("balloc: out of blocks");
+      }
       m = 1 << (bi % 8);
       if((bp->data[bi/8] & m) == 0){  // Is block free?
         bp->data[bi/8] |= m;  // Mark block in use.
@@ -77,6 +82,35 @@
   panic("balloc: out of blocks");
 }
 
+static uint balloc (uint dev)
+{
+  return balloc_(dev,0);
+}
+
+static uint Eballoc(uint dev, uint Eaddress) //p_blk should be the returning value of balloc
+{
+  uint p_blk = EADDRESS(Eaddress)+ ELENGTH(Eaddress)-1;
+  int bi=p_blk%BPB,b = p_blk-bi,m;
+  struct buf *bp;
+  bi=(bi+1)%BPB;
+  b=b+(bi==0?1:0);
+
+  if(b>=sb.size||(b+bi)>(0xFFFFFF))
+    return 0;
+  bp = bread(dev, BBLOCK(b, sb));
+  m = 1 << (bi % 8);
+  if((bp->data[bi/8] & m) == 0)
+  {
+    bp->data[bi/8] |= m;
+    log_write(bp);
+    brelse(bp);
+    bzero(dev, b + bi);
+    return b + bi;
+  }
+  brelse(bp);
+  return 0;
+}
+
 // Free a disk block.
 static void
 bfree(int dev, uint b)
@@ -370,12 +404,91 @@
 
 // Return the disk block address of the nth block in inode ip.
 // If there is no such block, bmap allocates one.
+static int Eseek(struct inode *ip, uint bn,uint * off, int * ext)
+{  //no indirect block
+   // ext=0 exist, ext = 1 extent, ext = -1 new,  
+  int pre=0;
+  for (int i=0;i<=NDIRECT;i++)
+  {
+    //cprintf("S#ip->addrs[%d]=%d\n",i,ip->addrs[i]);
+    if(ip->addrs[i]==0)
+    {
+      if(i)
+      {
+        *off= ELENGTH(ip->addrs[i-1]);
+        *ext = 1;
+        //cprintf("0#%d,%d,%d\n",bn,pre,i);
+        return (i-1);
+      }
+      *off=0;
+      *ext = -1;
+      //cprintf("1#%d,%d,%d\n",bn,pre,i);
+      return 0;   // return - index mean last index in use
+    }
+    int length = ELENGTH(ip->addrs[i]); 
+    if( pre+length > bn)
+    {
+      *ext = 0;
+      //cprintf("2#%d,%d,%d\n",bn,pre,i);
+      *off=bn-pre;
+      return i;
+    }
+    pre+=length;
+  }
+  //cprintf("3#%d,%d,%d\n",bn,pre);
+  return 0;
+}
+
+
+static uint Ebmap(struct inode *ip, uint bn)
+{
+  uint off=0,addr;
+  int ext=0, bdx=Eseek(ip,bn,&off,&ext);
+  //cprintf("bn:%d bdx:%d off:%d\n",bn,bdx,off);
+
+  if(ext==1)
+  {   //bn is new;
+    if(off>=EXTMAXBLKSIZE)
+      off=0;
+    else
+    {
+      addr = Eballoc(ip->dev,ip->addrs[bdx]);
+      //cprintf("###---###addr:%d\n",addr);
+      if(addr)
+      {
+        //cprintf("#-----extent\n");
+        ip->addrs[bdx]=TOEADDR(EADDRESS(ip->addrs[bdx]),(1+off));
+        return addr;
+      }
+    }
+    bdx+=1;
+  }
+  if((addr = ip->addrs[bdx]) == 0)
+  {
+    addr = balloc_(ip->dev,1);
+    ip->addrs[bdx]=TOEADDR(addr,1);
+    //cprintf("#-----new\n");
+    //cprintf("E#ip->addrs[%d]=%d\n",bdx,ip->addrs[bdx]);
+    off=0;
+    return addr;
+  }
+  else
+  {
+    //cprintf("#------old\n");
+    addr=EADDRESS(ip->addrs[bdx]);
+  }
+  return addr+off;
+}
+
 static uint
 bmap(struct inode *ip, uint bn)
 {
   uint addr, *a;
   struct buf *bp;
 
+  if(ip->type==E_FILE)
+    return Ebmap(ip,bn);
+
   if(bn < NDIRECT){
     if((addr = ip->addrs[bn]) == 0)
       ip->addrs[bn] = addr = balloc(ip->dev);
@@ -400,17 +513,40 @@
   panic("bmap: out of range");
 }
 
+
 // Truncate inode (discard contents).
 // Only called when the inode has no links
 // to it (no directory entries referring to it)
 // and has no in-memory reference to it (is
 // not an open file or current directory).
+static void Eitrunc(struct inode *ip)
+{
+  int i, j;
+
+  for(i = 0; i <= NDIRECT; i++){
+    if(ip->addrs[i]){
+      int length= ELENGTH(ip->addrs[i]);
+      for(j=0;j<length;j++)
+      {
+        bfree(ip->dev,EADDRESS(ip->addrs[i])+j);
+      }
+      ip->addrs[i] = 0;
+    }
+  }
+  ip->size = 0;
+  iupdate(ip);
+}
 static void
 itrunc(struct inode *ip)
 {
   int i, j;
   struct buf *bp;
   uint *a;
+  if(ip->type==E_FILE)
+  {
+    Eitrunc(ip);
+    return;
+  }
 
   for(i = 0; i < NDIRECT; i++){
     if(ip->addrs[i]){
@@ -435,6 +571,7 @@
   iupdate(ip);
 }
 
+
 // Copy stat information from inode.
 // Caller must hold ip->lock.
 void
@@ -445,6 +582,11 @@
   st->type = ip->type;
   st->nlink = ip->nlink;
   st->size = ip->size;
+  if (ip->type==E_FILE){
+    int i;
+    for(i=0;i<=NDIRECT;i++)
+      st->addrs[i] = ip->addrs[i];
+  }
 }
 
 //PAGEBREAK!
@@ -476,6 +618,15 @@
   return n;
 }
 
+int large_seek(struct file * f,uint off) 
+{
+  struct inode * ip = f->ip;
+  if(ip->type!=E_FILE||off < 0 || off > ip->size) 
+    return -1;
+  f -> off =off;
+  return 0;
+}
+
 // PAGEBREAK!
 // Write data to inode.
 // Caller must hold ip->lock.
@@ -493,7 +644,9 @@
 
   if(off > ip->size || off + n < off)
     return -1;
-  if(off + n > MAXFILE*BSIZE)
+  if (ip->type == E_FILE && n>MAXEFILE*BSIZE)
+    return -1;
+  else if(ip->type != E_FILE&&off + n > MAXFILE*BSIZE)
     return -1;
 
   for(tot=0; tot<n; tot+=m, off+=m, src+=m){
