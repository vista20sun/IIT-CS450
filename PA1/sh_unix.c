#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// Simplifed xv6 shell.

#define MAXARGS 10
#define MAXPARA 10    //define the max length of parallel cmds
#define MAXSEQU 10    //define the max length of sequence cmds

// All commands have at least a type. Have looked at the type, the code
// typically casts the *cmd to some specific cmd type.

/* deleted three function
  struct execcmd
  struct redircmd
  struct pipecmd
*/
int mc=0;
struct execcmd{
  char **argv;
  int argc;
};

struct para_cmd{
  struct execcmd** cmds;
  int argc;
};

struct sequ_cmd{
  struct para_cmd** p_cmds;
  int argc;
};

struct sequ_cmd * parse_sequcmd(char *s);
int fork1(void);  // Fork but exits on failure.
//struct cmd *parsecmd(char*); deleted

// Execute cmd.  Never returns.
void runcmd(struct sequ_cmd *scmd)
{
  if (scmd==NULL)
    return;
  int i,j;
  for(int i=0;i<scmd->argc;i++)
  {
    struct para_cmd *pcmd=scmd->p_cmds[i];
    for(j=0;j<pcmd->argc;j++)
    {
      struct execcmd *cmds=pcmd->cmds[j];
      if(fork1()==0){
        if(execvp(cmds->argv[0],cmds->argv)<0)// or execvp
        {
          printf("%s: Command not found\n",cmds->argv[0]);
          exit(0);
        }
      }
      else
      {
        printf("skip exec\n");
        free(cmds->argv);
        free(cmds);
      }
    }
    for(j=0;j<pcmd->argc;j++)
      wait(NULL);
    free(pcmd->cmds);
    free(pcmd);
  }
  free(scmd->p_cmds);
  free(scmd);
}

int getcmd(char *buf, int nbuf)
{
  
  if (isatty(fileno(stdin)))
    fprintf(stdout, "$ ");
  memset(buf, 0, nbuf);
  fgets(buf, nbuf, stdin);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

int main(void)
{
  static char buf[100];
  int fd, r;

  // Read and run input commands.
  while(getcmd(buf, sizeof(buf)) >= 0){
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
      // Clumsy but will have to do for now.
      // Chdir has no effect on the parent if run in the child.
      buf[strlen(buf)-1] = 0;  // chop \n
      if(chdir(buf+3) < 0)
        fprintf(stderr, "cannot cd %s\n", buf+3);
      continue;
    }
    //if(fork1() == 0) modified, fork in runcmd();
    runcmd(parse_sequcmd(buf));
    //wait(&r); modified, wait in runcmd();
  }
  exit(0);
}

int fork1(void)
{
  int pid;
  
  pid = fork();
  if(pid == -1)
    perror("fork");
  return pid;
}

/* removed functions
struct cmd* execcmd(void)
struct cmd* redircmd(struct cmd *subcmd, char *file, int type);
struct cmd* pipecmd(struct cmd *left, struct cmd *right)
*/

// Parsing

char whitespace[] = " \t\r\n\v";

// int gettoken(char **ps, char *es, char **q, char **eq) deleted

// int peek(char **ps, char *es, char *toks) deleted

/* modified
  struct cmd *parseline(char**, char*);
  struct cmd *parsepipe(char**, char*);
  struct cmd *parseexec(char**, char*);
*/

// make a copy of the characters in the input buffer, starting from s through es.
// null-terminate the copy to make it a string. 

//char *mkcopy(char *s, char *es) deleted

struct execcmd* parseexec(char *s)
{
  char *delim=whitespace;
  struct execcmd *cmd=(struct execcmd*)calloc(1,sizeof(struct execcmd));
  cmd->argv=(char**)calloc(MAXARGS,sizeof(char*));
  int argc=0;
  char **argv=cmd->argv;
  argv[argc++]=strtok(s,delim);
  while((argv[argc++]=strtok(NULL,delim))!=NULL&&argc<MAXARGS);
  cmd->argc=argc-1;
  return cmd;
}

struct para_cmd * parse_paracmd(char *s)
{
  char *delim="&";
  struct para_cmd *pcmd=(struct para_cmd*)calloc(1,sizeof(struct para_cmd));
  pcmd->cmds=(struct execcmd**)calloc(MAXPARA,sizeof(struct execcmd*));
  char ** split=(char**)calloc(MAXPARA,sizeof(char*));
  int argc=0;
  split[argc++]=strtok(s,delim);
  while((split[argc++]=strtok(NULL,delim))!=NULL&&argc<MAXPARA);
  pcmd->argc=argc-1;
  int i;
  for(i=0;i<argc-1;i++)
    pcmd->cmds[i]=parseexec(split[i]);
  free(split);
  return pcmd;
}

struct sequ_cmd * parse_sequcmd(char *s)
{
  int len=strlen(s);
  if(s[len-2]=='&')
  {
    printf("commands can not end with\'&\'\n");
    return NULL;
  }
  char *delim=";";
  struct sequ_cmd *scmd=(struct sequ_cmd*)calloc(1,sizeof(struct sequ_cmd));
  scmd->p_cmds=(struct para_cmd**)calloc(MAXSEQU,sizeof(struct para_cmd*));
  char ** split=(char**)calloc(MAXSEQU,sizeof(char*));
  int argc=0;
  split[argc++]=strtok(s,delim);
  while((split[argc++]=strtok(NULL,delim))!=NULL&&argc<MAXSEQU);
  scmd->argc=argc-1;
  int i;
  for(i=0;i<argc-1;i++)
    scmd->p_cmds[i]=parse_paracmd(split[i]);
  free(split);
  return scmd;
}






/* removed functions
struct sequ_cmd* parsecmd(char *s)
struct cmd* parseline(char **ps, char *es)
struct cmd* parsepipe(char **ps, char *es)
struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es)
struct cmd* parseexec(char **ps, char *es)
*/