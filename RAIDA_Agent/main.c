// C program to illustrate use of fork() & exec() system call for process creation

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int val =  200;

int main(){
    pid_t pid, x;
    int ret = 1;
    int status;

    int test = 155;
    char* test_str;
    char val_str[20];
    sprintf(test_str, "%d", test);
    sprintf(val_str, "%d", val);

    printf("pid_0: %u\n", pid);
    printf("main process, pid = %u\n",getpid());
    printf("parent of main process, pid = %u\n",getppid());
    
    pid = fork();
    printf("pid_1: %u\n", pid);

    if (pid == -1){

        // pid == -1 means error occured
        printf("can't fork, error occured\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0){

        printf("CHILD PROCESS\n");
        printf("pid_2: %u\n", pid);
        // pid == 0 means child process created
        printf("child process, pid = %u\n",getpid());
        printf("parent of child process, pid = %u\n",getppid());

        char *args[]={"./demo", test_str, val_str, NULL};
        execvp(args[0],args);
        
        printf("I am main.c\n");
        printf("main: val = %d   test = %d\n", val, test);
        printf("Ending-----");
    
        // the execv() only return if error occured. The return value is -1
        exit(0);
    }
    else{
        printf("PARENT PROCESS\n");
        printf("pid_3: %u\n", pid);
        printf("Parent Of parent process, pid = %u\n",getppid());
        printf("parent process, pid = %u\n",getpid());

            // the parent process calls waitpid() on the child 
            //waitpid() system call suspends execution of calling process until a child specified by pid argument has changed state
            printf("pid_4: %u\n", pid);
            //if (WIFEXITED(status) && !WEXITSTATUS(status))
            //    printf("program execution successful\n");
            
            if ((x = waitpid(pid, &status, 0)) > 0) {
                printf("pid_5: %u\n", pid);
                printf("x = %u\n", x);
                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    printf("wifexited: %d  wexitstatus: %d\n",WIFEXITED(status),WEXITSTATUS(status) );
                    printf("program execution successful\n"); }
                
                else if (WIFEXITED(status) && WEXITSTATUS(status)) {
                    if (WEXITSTATUS(status) == 127) {
                        printf("execv failed\n");
                    }
                    else
                        printf("program terminated normally,"
                        " but returned a non-zero status\n");				
                }
                else {
                    printf("program didn't terminate normally\n");	}		
            }
            else {
            printf("waitpid() failed\n");
            }
        exit(0);
    }
    return 0;
}
