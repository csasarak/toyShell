#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "exec_cmd.h"

#define ARG_START_SIZE 5
#define ARG_INC_SIZE 5

char** parse_cmd(char *cmd);

/* Execute the command string by first tokenizing
   it and then forking 
*/   
void exec_cmd(char *cmd){
    // Need to find out if there is an & at the end for bg processes
    char** args = parse_cmd(cmd);

    // Need to execute the program and then exit
    pid_t pid = fork();
        
    if(pid == -1){
        fprintf(stderr, "%s\n", strerror(errno));
        exit(errno);
    }
    if(pid != 0){
        wait(NULL);
    }
    else{
        execvp(args[0], args);
        // If execvp returned there was an error
        fprintf(stderr, "%s\n", strerror(errno));
        
    }
    
    free(args);
}

/* This function will return a list that can be given to
the exec commands as the arg list (with included final null).
The first element of the array can be used as the filename
for the exec commands.
The space the returned pointer points to was allocated with
malloc() and can be freed with free().
*/
static char** parse_cmd(char *cmd){
    // Separate the arguments for the command and store them
    size_t args_size = sizeof(char**) * ARG_START_SIZE;
    char** args = (char**)malloc(args_size);

    int loop = 0;
    args[loop] = strtok(cmd, " ");
    do{
        loop++;
        // Add space for more arguments if needed
        if(sizeof(char**) * loop == args_size){
            args_size += sizeof(char**) * ARG_INC_SIZE;
            args = (char**)realloc((void*)args, args_size);
        }
        
        args[loop] = strtok(NULL, " ");
        
    }while(args[loop] != NULL);

    return args;
}

