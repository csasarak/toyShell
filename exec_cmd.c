/*
  Filename: exec_cmd.c
  Author: Christopher Sasarak <cms5347@rit.edu>

  This file contains code for parsing and executing a command that
  has been passed to it.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>
#include "exec_cmd.h"
#include "history.h"

#define ARGS_START_SIZE 5
#define ARGS_INC_SIZE 5

static char** parse_cmd(char *cmd);

/* Execute the command string by first tokenizing
   it and then forking 
*/   
void exec_cmd(char *cmd){
    // Check if we need to wait for a command to finish
    int str_end = strlen(cmd) - 1, background = 0;
    if(cmd[str_end] == '&'){
        background = 1;
        cmd[str_end] = '\0';
    }
    char** args = parse_cmd(cmd);
        
    // Need to execute the program and then exit
    pid_t pid = fork();
        
    if(pid == -1){
        perror( strerror(errno));
        exit(errno);
    }
    
    if(pid && !background){
        wait(NULL);
    }
    else if(!pid){
        execvp(args[0], args);
        // If execvp returned there was an error
        perror( strerror(errno));
    }

    // Args aren't needed anymore so free the space used for pointers
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
    size_t args_size = sizeof(char**) * ARGS_START_SIZE;
    char** args = (char**)malloc(args_size);

    int loop = 0;
    args[loop] = strtok(cmd, " ");
    do{
        loop++;
        // Add space for more arguments if needed
        if(sizeof(char**) * loop == args_size){
            args_size += sizeof(char**) * ARGS_INC_SIZE;
            args = (char**)realloc((void*)args, args_size);
        }
        
        args[loop] = strtok(NULL, " ");
        
    }while(args[loop] != NULL);

    return args;
}

