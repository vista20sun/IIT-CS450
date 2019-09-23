
 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <sys/types.h>
 #include <sys/wait.h>
 #include <signal.h>
 
 int main(int argc, char **argv) 
 {
    int secs,i;
     if (argc != 3) {
     fprintf(stderr, "Usage: %s <n>\n", argv[0]);
     return 0;
     }
     secs = atoi(argv[1]);
 
     for (i=0; i < secs; i++)
     {
        sleep(1);
        printf("%s\n",argv[2]);
     }
     
 
    return 0;
 
 }