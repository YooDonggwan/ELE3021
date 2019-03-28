#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>

#define MAX_SIZE 100

int main(int argc, char *argv[])
{
    // Declare variable
    char command[MAX_SIZE]; // input command
    char *cmd_tmp; // arbitrary space of saving command
    char *buf_tmp;
    char **argvs; // pointer array of seperating command to save
    char **token; // token
    pid_t pid[100];
    int status;
    int cmd_cnt = 0; // command count
    int i = 0;
    FILE *fp;


    while(1)
    {

        if(argc == 1){
            printf("prompt>");
            //input command. if input is CtrlD, quit
            if(fgets(command,MAX_SIZE,stdin) == NULL){
                printf("\n");
                exit(0);
            }

            command[strlen(command)-1] = '\0'; // delete enter space   

            cmd_tmp = (char*)malloc(strlen(command)*100); // allocate size of cmd tmp
            strcpy(cmd_tmp, command); // copy command to cmd_tmp
            if(!strcmp(cmd_tmp,"quit")){
                exit(0);
            }

            token = (char**)malloc(strlen(cmd_tmp)*100);
            cmd_cnt=0;
            token[cmd_cnt]=strtok(cmd_tmp,";");

            while(token[cmd_cnt] != NULL) // count the amount of command, seperate ';' 
            {
                cmd_cnt++;
                token[cmd_cnt]=strtok(NULL,";");
            }
            for(i=0; i<cmd_cnt; i++) // execute command as cmd amount
            {
                int temp = 0;
                if(token[i]!=NULL){

                    pid[i] = fork(); // make child process 

                    // Save token to buf, buf_tmp
                    buf_tmp = (char*)malloc(strlen(token[i])+1);
                    argvs = (char**)malloc(strlen(token[i]));
                    strcpy(buf_tmp,token[i]);
                    argvs[temp]= strtok(buf_tmp," ");
                    while(argvs[temp]!=NULL){

                        temp++;
                        argvs[temp]=strtok(NULL," ");
                    }

                    if(pid[i]==0){ // execute command
                        if(execvp(argvs[0],argvs) == -1){
                            printf("Command is not available. Please type correct command\n");
                        }

                        if(strcmp(argvs[0],"quit"))
                            exit(0);
                    }
                    else if(pid[i]<0){
                        printf("Failed create childprocess\n");
                    }

                }

            }
            // to arrange parallel process
            for(int p = 0; p<cmd_cnt;p++){
                if(pid[p]>0){
                    pid_t exitchildpid = wait(&status);

                    waitpid(pid[p],&status,0);
                    if(WIFEXITED(status) == 0){
                        printf("childprocess %d is exited exceptionally\n",exitchildpid);
                    }
                }
            }
        }
        else if(argc == 2){
                fp = fopen(argv[1],"r");
                while(!feof(fp)){
                    if(fgets(command,MAX_SIZE,fp) == NULL)
                        break;

                    printf("%s",command);
                    command[strlen(command)-1] = '\0'; // delete enter space   

                    cmd_tmp = (char*)malloc(strlen(command)*100); // allocate size of cmd tmp
                    strcpy(cmd_tmp, command); // copy command to cmd_tmp
                    if(!strcmp(cmd_tmp,"quit")){
                        exit(0);
                    }
                    token = (char**)malloc(strlen(cmd_tmp)*100);
                    cmd_cnt=0;
                    token[cmd_cnt]=strtok(cmd_tmp,";");

                    while(token[cmd_cnt] != NULL) // count the amount of command, seperate ';' 
                    {
                        cmd_cnt++;
                        token[cmd_cnt]=strtok(NULL,";");
                    }
                    for(i=0; i<cmd_cnt; i++) // execute command as cmd amount
                    {
                        int temp2 = 0;
                        if(token[i]!=NULL){

                            pid[i] = fork(); // make child process 

                            // Save token to buf_tmp
                            buf_tmp = (char*)malloc(strlen(token[i])+1);
                            argvs = (char**)malloc(strlen(token[i]));
                            strcpy(buf_tmp,token[i]);
                            argvs[temp2]= strtok(buf_tmp," ");
                            while(argvs[temp2]!=NULL){
                                temp2++;
                                argvs[temp2] = strtok(NULL," ");
                            }

                            if(pid[i]==0){ // execute command
                                if(execvp(argvs[0],argvs) == -1){
                                    printf("Command is not available. Please type correct command\n");
                                }
                                if(strcmp(argvs[0],"quit"))
                                    exit(0);
                            }
                            else if(pid[i]<0){
                                printf("Failed create childprocess\n");
                            }
                        }
                    }
                    // to arrange parallel process
                    for(int p = 0; p<cmd_cnt;p++){
                        if(pid[p]>0){
                            pid_t exitchildpid = wait(&status);

                            waitpid(pid[p],&status,0);
                            if(WIFEXITED(status) == 0){
                                printf("childprocess %d is exited exceptionally",exitchildpid);
                            }
                        }
                    }
                }
                fclose(fp);
                exit(0);
        }
    }
    return 0;
}
