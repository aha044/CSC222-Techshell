/*
* Name: Jenny Wright, Tia Ransom, and Ashton Harrell
* Date: 2/13/25
* Description: basic shell implementation with I/O redirection 
*/

// headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

// a shellcommand has 10 possible arguments and a counter for those arguments
struct ShellCommand{
    char* args[10];
    int argcount;
};

// a function to continually prompt the user for input 
char* CommandPrompt(){ // display working directory and take user input
    char* input = malloc(100); // allocate memory to prevent segmentation fault 
    char* cwd = getcwd(NULL, 0);
    printf("%s$ ", cwd); // print pwd
    fgets(input, 100, stdin); // take user input
    return input;
}

// a function that parses user input and outputs a ShellCommand
struct ShellCommand ParseCommandLine(char* input) {
    // while reading user input, add individual args to args array 
    // use counter to navigate args array
    // use strtok() to split the input string by whitespace
    struct ShellCommand command;
    char *args[10];
    // first argument
    char* token = strtok(input, " ");
    // counter for where to put next arg
    int length = 0;
    // append tokens to *args until NULL pointer
    while (token != NULL && length < 10){
        // next argument added to args
        args[length] = token;
        // grab the next argument
        token = strtok(NULL, " ");
        // increment to go to next available index
        length++;
    }
    // assign each user argument to our structs arguments
    for (int k = 0; k < 10; k++){
        command.args[k] = args[k];
    }
    command.argcount = length;
    return command;
}

// a function to handle execution of a command
void ExecuteCommand(struct ShellCommand command) {
    
    size_t len = strlen(command.args[command.argcount - 1]);
    // terminate each user line's newline char with null char
    if (command.args[command.argcount - 1][len - 1] == '\n') {
        command.args[command.argcount - 1][len - 1] = '\0';
    }

    // if nothing was typed in
    if (command.argcount == 0) {
        return;
    }

    // if exit was the first thing typed in the terminal
    if (strcmp(command.args[0], "exit") == 0) {
       exit(0);
    }

    // if cd is args[0]
    // check that we are cd'ing and we are only doing 2 args
    if (strcmp(command.args[0], "cd") == 0 && command.argcount == 2){
        // error handling for cd
        // trying to get to root directory 
        if (strcmp(command.args[1], "/") == 0){
            printf("Error: permission denied\n");
        }
        // failure to 
        else if (chdir(command.args[1]) != 0){
            printf("%s not found.\n", command.args[1]);
            printf("Usage: cd <directory>\n");
        }
        // return because we cannot cd in a forked process
        return;
    }

    else{
        pid_t PID = fork();
        // child process
        if (PID == 0) {
            command.args[command.argcount] = NULL; //ensures that the last argument is NULL
            execvp(command.args[0], command.args);
            perror("command not found");
            exit(1);
        } else { // parent process
            int exitstatus;
            waitpid(PID, &exitstatus, 0); //wait for the child process to complete
        }
    }
}

// MAIN
int main(){
    char* input;
    // forever loop
    while (1) {
        // take input
        input = CommandPrompt();
        // parse input
        struct ShellCommand command = ParseCommandLine(input);
        // execute input
        ExecuteCommand(command);
    }
    // free the allocated space for input 
    free(input);
    return 0;
}