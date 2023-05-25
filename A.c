#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>

pid_t child_pid; // process id 
bool ok = false;
int delay = 5; // delay in seconds

void killOnTimeHandler() {
    sleep(delay);
   
    if(ok) return;

    printf("Program B closed on timout\n");
    kill(child_pid, SIGKILL); // sending signal for completing
    
}
void killOnSucessHandler() {
    ok = true;
    kill(child_pid, SIGKILL);
    printf("Program B closed sucessfully\n");
}


int main() {
    signal(SIGUSR1, killOnTimeHandler);
    signal(SIGUSR2, killOnSucessHandler);
    child_pid = fork(); // creating a new process

    if(child_pid == 0) {
        // start prog B
        execl("./B", "B", (char*) NULL);
    } else {
        // waiting for the completion of the subprog
        wait(NULL);
    }

    return 0;
}
