/*This is the CS3620 Homework1 Task1: a LINUX SHELL by system calls.
hawkID: jzhang306*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#define SHELL_TOKEN_DELIM " \t\r\n"//Parse delim
#define MAX_LEN 1024


void built_in_cd(char * dir){//Implement a built in cd command by chdir function.
    if(dir == NULL)printf("Expect a directory after the cd command");
    else if(chdir(dir)!= 0){
        perror("Failed!");
    }
}
void built_in_help(){
    printf("This is the help command:\n");
    printf("This shell supports all commands in /bin/\n");
    printf("cd <directory>: should followed by a directory\n");
    printf("exit: an exit() call to quit the shell.\n");
    printf("help: list the built in commands supported by this shell.\n");
}
void execute_command(char *argv[]){
    char *command = argv[0], *argument = argv[1];//get the command and argument from the array.

    // Check for built-in commands
    if (strcmp(command, "exit") == 0) {
        exit(0); 
    } else if (strcmp(command, "cd") == 0) {
        built_in_cd(argument);
        return; // Stop further processing, have to do this for all built-in functions or it will proceed to the exec() calls.
    } else if (strcmp(command, "help") == 0) {
        built_in_help();
        return; 
    } else {// Handle external commands in /bin/ folder.
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return;
        }
        if (pid == 0) {
            // Child process
            if (execvp(argv[0], argv) == -1) {
                perror("execvp");
            }
            exit(EXIT_FAILURE); // Ensure child exits if exec fails
        } else {// Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
}


int main(){
    char line[MAX_LEN];
    char *argv[100]; //Array to hold command arguments.
    char *token;
    while(1){//always running the shell
        printf("[SHELL:type help for commands!]>>> ");
        fflush(stdout);//Make sure the prompt is displayed.

        if(!fgets(line, MAX_LEN, stdin))break;
        //Split the line into tokens by the token delim
        int argc = 0;//counter
        token = strtok(line, SHELL_TOKEN_DELIM);
        while(token != NULL){
            argv[argc++] = token;
            token = strtok(NULL, SHELL_TOKEN_DELIM);
        }
        argv[argc] = NULL;
             
        if (argv[0] == NULL)break; //Empty command.
        execute_command(argv);
    }
}