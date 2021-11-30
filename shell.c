#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void loop() {
    char* user = getenv("USER");
    char cwd[128];
    char cmd[1024];

    getcwd(cwd, sizeof(cwd));  
    printf("%s@%s$ ", user, cwd);

    char *args[8];
    char **next = args;
    while (fgets(cmd, sizeof(cmd), stdin) != NULL) {
        if (strcmp(cmd, "exit\n") == 0) 
            exit(EXIT_SUCCESS);

        char *temp = strtok(cmd, " \n");
        while (temp != NULL) {
            *next++ = temp;
            temp = strtok(NULL, " \n");
        }
        *next = NULL;

        pid_t pid = fork();

        if (pid == -1) {
            printf("\nFailed forking child..");
            return;
        } else if(pid == 0) {
            if (execvp(args[0], args) < 0) {
                printf("\nCould not execute command..");
            }
            exit(0);  
        }
        else {
            wait(NULL);
            loop();
        }
    }
}


int main() {
    loop();
    return 0;
}