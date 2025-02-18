/*
* Name: Jenny Wright, Tia Ransom and Ashton Harrell
* Date: 2/13/25
* Description: basic shell implementation with I/O redirection 
*/

// headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

struct ShellCommand{
    char commandname[50];
    char *args[10];
    int argcount;
};

// Functions to implement

// WORKS (for now )
char* CommandPrompt(){ // display working directory and take user input
    char* input = malloc(100); // allocate memory to prevent segmentation fault 
    char* cwd = getcwd(NULL, 0);
    printf("%s$ ", cwd); // print pwd
    scanf("%s", input); // take user input
    return input;
}

struct ShellCommand ParseCommandLine(char* input);

void ExecuteCommand(struct ShellCommand command);

// MAIN
int main(){
    char* input;
    struct ShellCommand command;
    // repeatedly prompt the user for input 
    for (;;){
        input = CommandPrompt();
        break;
        // parse the command line 
        //command = ParseCommandLine(input);
        // execute the command 
        //ExecuteCommand(command);
    }
    free(input);
    exit(0);
} 