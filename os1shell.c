/*
  Filename: os1shell.c
  Author: Christopher Sasarak <cms5347@rit.edu>

  This file includes the main method as well as code for the
  command loop which reads commands and then sends them to be executed.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include "exec_cmd.h"
#include "history.h"

void command_loop();
char* string_trim(char* str);

int main(int argc, char *argv[]){
    command_loop();
    
    exit(0);
}

/*
  This function reads input in a loop and executes the
  entered commands.
*/
void command_loop(){
    const char*  prompt = "OS1Shell -> ";
    char input_buf[BUF_SIZE];
    char* trimmed_cmd;
    ssize_t bytes_read;

    do{
        printf("%s", prompt);
        fflush(stdout);
        
        bytes_read = read(STDIN_FILENO, &input_buf, BUF_SIZE);
        if(input_buf[bytes_read] == -1){
            fprintf(stderr, "%s\n", strerror(errno));
            exit(errno);
        }
        // Get rid of newline and end string
        input_buf[bytes_read-1] = '\0';
        trimmed_cmd = string_trim(input_buf);
        // Check if this is the history command
        if(strcmp("history", trimmed_cmd) == 0){
            print_history();
            continue;
        }
        else if(*trimmed_cmd == '\0'){
            continue;
        }
        
        add_cmd(trimmed_cmd);
        exec_cmd(trimmed_cmd);
    }while(bytes_read != 0);

    printf("\n");
}

/*
  Trim  whitespace from the beginning and end of a string
*/ 
char* string_trim(char* str)
{
    // Remove initial whitespace
    for(; isspace(*str); str++){}
    
    // Remove ending whitespace
    char *end = str + strlen(str) - 1;
    for(; end > str && isspace(*end); end--){}

    *(end + 1) = '\0';

    return str;
}

