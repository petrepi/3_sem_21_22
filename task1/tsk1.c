#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
char** parse(const char* s_0, int flag) {

    char** r = malloc(sizeof(char*)*100);
    char *rest = NULL;
    char *token; 
    char* s = strdup(s_0);
    int i = 0;
    const char* delim;
    if (flag) {
        delim = " ,\n";

    }
    else {
        delim = "|\n";
    }
    for (token = strtok_r(s, delim, &rest); token != NULL; token = strtok_r(NULL, delim, &rest)) {   
        r[i++] = token;
        if (i % 100 == 0)
            r = realloc(r, sizeof(char*)*100);
    }
    return r;
}


static void run_cmd(char **cmd_parsed)
{
    int i=0;
    int d;
    int p[2];
    while(cmd_parsed[i]!=NULL)
    {
        char *cmd=cmd_parsed[i];
        pipe(p);
        int status;
        const pid_t pid = fork();
        if (pid < 0) {
            printf("fork failed!\n");
            exit(1);
        }
        if (pid) {
            waitpid(pid, &status,0);
            close(p[1]);
            d = p[0];
            printf("Ret code: %d\n", WEXITSTATUS(status));
            i++;
            
        }
        else {
            char **args=parse(cmd,1); 
            dup2(d, 0);
            if(cmd_parsed[i+1] != NULL)
                dup2(p[1],1);
            close(p[0]);
            execvp(args[0], args);
            printf("exec* failed\n");
        }

    }
    return ;


}

int main()
{
  while(1) {
    char *cmd;
    fgets(cmd, 255, stdin);
    char **cmd_parsed=parse(cmd,0);
    int i=0;
    run_cmd(cmd_parsed);
  }
  return 0;
 }

