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
#include <signal.h>
#include <ctype.h>
#include "config.h"
#include "exec_cmd.h"
#include "history.h"

void command_loop();
char* string_trim(char* str);
static void install_sighandlers();
static void handle_sigint(int signal_id);

static const char*  prompt = "OS1Shell -> ";

int main(int argc, char *argv[]){
    install_sighandlers();
    command_loop();
    
    exit(0);
}

/*
  This function reads input in a loop and executes the
  entered commands.
*/
void command_loop(){
    char input_buf[BUF_SIZE];
    char* trimmed_cmd;
    ssize_t bytes_read;

    while(1){
        printf("%s", prompt);
        fflush(stdout);
        
        bytes_read = read(STDIN_FILENO, &input_buf, BUF_SIZE);
        if(bytes_read == -1){
            if(errno == EINTR){
                continue;
            }
            perror( strerror(errno));
            exit(errno);
        }
        else if(bytes_read == 0){
            break;
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
    }

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

/*
  This function will install sighandlers which will last for the
  duration of the program.
*/
static void install_sighandlers(){
    // This code was copied from Project1.pdf's section on sigaction    
    struct sigaction act_sigint;
    struct sigaction act_sigchld;
    act_sigint.sa_handler = handle_sigint;
    act_sigint.sa_flags = 0;
    sigemptyset(&act_sigint.sa_mask);
    sigaddset(&act_sigint.sa_mask, SIGINT);
    sigaction(SIGINT, &act_sigint, NULL);

    /* Set up a signal handler for when children exit, this is done by setting
       the handler to SIG_IGN. According to POSIX.1-2001 (man 2 wait has more
       details) setting this to SIG_IGN will prevent zombie processes from
       occurring when a child process exits in the background.
    */
    act_sigchld.sa_handler = SIG_IGN;
    act_sigchld.sa_flags = 0;
    sigemptyset(&act_sigchld.sa_mask);
    sigaction(SIGCHLD, &act_sigchld, NULL);
}
/*
  This is the function which executes when a SIGINT is received.
*/
static void handle_sigint(int signal_id){
    if(signal_id == SIGINT){
        printf("\n"); // make sure we're on a new line
        print_history();
    }
    else{
        printf("%s\n", strsignal(signal_id));
    }
}

