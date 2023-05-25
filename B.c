#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int divideString(char *str, char *commands[]) {
    int len = strlen(str);
    int i, j, k;
    int insideQuotes = 0;
    int commandCount = 0;

    commands[commandCount++] = str;

    for (i = 0, j = 0; i < len; i++) {
        if (str[i] == '"') {
            insideQuotes = !insideQuotes;
        }

        if (!insideQuotes && str[i] == '|') {
            str[i] = '\0';
            commands[commandCount++] = &str[i + 2];
        }
    }
    return commandCount;
}



void executeCommands(char* command) {
    char* commands[256];
    int commandCount = divideString(command, commands);


    int fds[2]; // file descriptors
    pid_t pid;

    for(int j = 0; j < commandCount; j++) {
        pipe(fds);

        if((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        } else if(pid == 0) {
            if(j < commandCount - 1) {
                if(dup2(fds[1], STDOUT_FILENO) < 0) exit(1); // opening output
            }
            close(fds[0]);
            system(commands[j]);
            exit(0);
        } else {
            if(j < commandCount - 1) {
            if(dup2(fds[0], STDIN_FILENO) < 0) exit(1); // opening output
                close(fds[1]);
            }
            wait(NULL);
        }
    }
    kill(getppid(), SIGUSR2);
}

int main() {
    char command[1024];
    printf("Enter command: ");
    fgets(command, sizeof(command), stdin);

    // Sending signal to A process
    kill(getppid(), SIGUSR1);

    // command execution
    executeCommands(command);

    return 0;
}
