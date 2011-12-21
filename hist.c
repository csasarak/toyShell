#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "config.h"

// Make thread safe by passing history structs to functions
static char** history = NULL;
static int first_cmd = HIST_SIZE - 1; // The most recent command

/* Add a new command string to the history, deleting
   one in the process if that would make more than HIST_SIZE
   commands in history. Return 0 on success, set errno and return -1 otherwise
*/
int add_cmd(char *cmd){
    //Initialize the memory if it isn't there
    if(history == NULL)
        history = (char**)calloc(HIST_SIZE, sizeof(char*));
    
    // Check if we need to wrap around
    if(first_cmd == 0)
        first_cmd = HIST_SIZE - 1;
    else
        first_cmd--;

    if(history[first_cmd] != NULL){
        // possible TODO: only reallocate if a larger memory chunk is needed
        free(history[first_cmd]);
    }
    history[first_cmd] = (char*)malloc(strlen(cmd));

    // Check for memory allocation errors
    if(history[first_cmd] == NULL){
        return -1;
    }
    
    strcpy(history[first_cmd], cmd);
    return 0;
}

/* This function can be used to iterate through the history.
   It returns a pointer to the next string in the history, or
   NULL if it has reached the end or there is no history.
   Passing NULL in as the parameter will get the next value,
   while anything else will cause iteration to restart from the beginning.
   Not thread safe. 
*/
char* next_cmd(int reset){
    static int curr_cmd = HIST_SIZE - 1;

    // Reset the iteration if necessary
    if(reset != (int)NULL){
        curr_cmd = first_cmd;
        return history[first_cmd];
    }

    curr_cmd++;

    if(curr_cmd > HIST_SIZE - 1){
        curr_cmd = 0;
    }
    else if(curr_cmd == first_cmd){
        return (char*) NULL;
    }
    return history[curr_cmd];
}
