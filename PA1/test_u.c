
#include "types.h"
#include "user.h"
#include "fcntl.h"
 
 int main(int argc, char **argv) 
 {
    int secs,i;
     if (argc != 3) {
     printf(2, "Usage: %s <n>\n", argv[0]);
     exit();
     }
     secs = atoi(argv[1]);
 
     for (i=0; i < secs; i++)
     {
        sleep(100);
        printf(2,"%s\n",argv[2]);
     }
     
    exit();
 
 }