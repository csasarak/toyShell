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
#include "config.h"
#include "exec_cmd.h"
#include "hist.h"

void command_loop();

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
        add_cmd(input_buf);
        exec_cmd(input_buf);
    }while(bytes_read != 0);

    printf("\n");
}

