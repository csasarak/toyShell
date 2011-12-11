/*
  Filename: os1shell.c
  Author: Christopher Sasarak <cms5347@rit.edu>
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "shell_consts.h"

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
    int read_code;

    do{
        printf("%s", prompt);
        fflush(stdout);
        
        read_code = read(STDIN_FILENO, &input_buf, BUF_SIZE);
    }while(read_code != 0);

    printf("\n");
}
