#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#define DELIM "\t \n"

int main() {
    char *args[10];
    char buf[256];
    char *cmdPipe1[10];
    char *cmdPipe2[10]; 
    char *s;
    char *save;
    int argn, pid, status;
    int i, fd;
    int type;
    int pipefd[2];
    int k=0;
    pid_t pid1, pid2;

    while(1) {
        argn = 0;
        printf("Myshell Input: ");


        fgets(buf,sizeof(buf) , stdin);
        s =  strtok_r(buf, DELIM, &save);

        while(s) {
            args[argn++] = s;
            s = strtok_r(NULL, DELIM, &save);
        }

        args[argn] = NULL;
        if (!strcmp(args[0], "quit"))
             break;

        for(i=0; args[i] != NULL; i++){
            if(!strcmp(args[i], ">")) {
                type=1;
                break;
            }else if(!strcmp(args[i], "<")){
               type=2;
               break;
            }else  if(!strcmp(args[i], "|")){
                type =3;
                break;
            }
            
            else{
                type = 4;
            }
        }
        
        
       
    

        
        pid = fork();

        if (pid > 0){
                pid = wait(&status);
        } else {
                if(type == 1){
                     fd = open(args[i+1], O_RDWR | O_CREAT, 0644);
                     dup2(fd, 1);
                     args[i] = NULL;
                     args[i+1] = NULL;
                     for(i = i; args[i] != NULL; i++){
                     args[i] = args[i+2];
                     }
                     args[i] = NULL;
                     execvp(args[0], args);

                }else if(type ==2){
                     fd = open(args[i+1], O_RDWR | O_CREAT, 0644);
                     dup2(fd, 0);
                     args[i] = NULL;
                     args[i+1] = NULL;
                     for(i = i; args[i] != NULL; i++){
                     args[i] = args[i+2];
                     }
                     args[i] = NULL;
                     execvp(args[0], args);
                }else if(type ==3) {
                     for (i=0; args[i] != NULL; i++){
                         if(!strcmp(args[i], "|")){
                             cmdPipe1[i] = NULL;
                             break;
                         }
                         cmdPipe1[i] = args[i];
                     }
                     for(i= i + 1; args[i] != NULL; i++){
                         cmdPipe2[k] = args[i];
                         k++;
                     }
                     if(fork() == 0){
                          pipe(pipefd);
                          if(fork() > 0){
                             dup2(pipefd[1], STDOUT_FILENO);
                             close(pipefd[0]);
                             execvp(cmdPipe1[0], cmdPipe1);
                            
                          }else{
                               dup2(pipefd[0], STDIN_FILENO);
                               close(pipefd[1]);
                               execvp(cmdPipe2[0], cmdPipe2);
                          }
                          
                     }    
                }
                   
                else {
                     execvp(args[0], args);
                }
        }
    } 
 } //메인 끝 괄호 
