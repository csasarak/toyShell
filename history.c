/*
  Filename: history.c
  Author: Christopher Sasarak <cms5347@rit.edu>

  This file includes code for storing, maintaining, and retrieving
  the shell's history.
*/
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "config.h"

static char** history = NULL;
static int oldest_cmd = 0; // The oldest command in history

/* Add a new command string to the history, deleting
   one in the process if that would make more than HIST_SIZE
   commands in history. Return 0 on success, set errno and return -1 otherwise
*/
int add_cmd(char *cmd){
    static int last_cmd = 0; // The most recently added command
    
    if(history != NULL){
        // Check if we need to wrap around
        if(last_cmd == HIST_SIZE - 1)
            last_cmd = 0;
        else
            last_cmd++;

        if(history[last_cmd] != NULL){
            free(history[last_cmd]);
            oldest_cmd = (oldest_cmd < HIST_SIZE - 1) ? oldest_cmd + 1 : 0;
        }
    }
    else{ // Allocate memory for the history if it isn't there
        history = (char**)calloc(HIST_SIZE, sizeof(char*));
        oldest_cmd = last_cmd;
    }
    
    history[last_cmd] = (char*)malloc(strlen(cmd));

    // Check for memory allocation errors
    if(history[last_cmd] == NULL){
        return -1;
    }
    
    strcpy(history[last_cmd], cmd);
    return 0;
}

/* This function can be used to iterate through the history from the oldest to
   the most recent command It returns a pointer to the next string in the
   history, or NULL if it has reached the end or there is no history.  Passing
   0 in as the parameter will get the next value, while anything else will
   cause iteration to restart from the beginning and return the first history
   item.
*/

char* next_cmd(int reset){
    static int curr_cmd = 0;
    // Return if there isn't a history yet
    if(history == NULL)
        return NULL;
    
    // Reset the iteration if necessary
    if(reset){
        curr_cmd = oldest_cmd;
        return history[curr_cmd];
    }

    curr_cmd++;

    if(curr_cmd == HIST_SIZE){
        curr_cmd = 0;
    }
    if(curr_cmd == oldest_cmd){
        return (char*) NULL;
    }
    return history[curr_cmd];
}

void print_history(){
    // Reset the iteration and get the first history element
    char* cmd = next_cmd(1);
    // Return if there isn't a history yet
    if(cmd == NULL)
        return;
    
    printf("%s\n", cmd);
    // Print the rest
    while((cmd = next_cmd(0)) != NULL){
        printf("%s\n", cmd);
    }
}

