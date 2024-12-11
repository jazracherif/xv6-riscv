

/* the main processor will feed number 2 to 280 to the second processor
 a recursive algoirhtm where a child will create a new function which starts 
 at p and passes all non-mulitple of p to the nexst process, which is created with first
 non-dropped prime and does the same thing, until reaching the max number
*/ 
#include "kernel/types.h"
#include "user/user.h"

#define _pipe int*
#define READ 0
#define WRITE 1

#define NULL 0

void next_process(_pipe left_pipe, int p)  __attribute__((noreturn));

int initialized(int pipe[2]){
    if (pipe == NULL || (pipe[0]==0 || pipe[1]==0)){
        return 0;
    }
    return 1;
}

/*
    create a new process that start with prime p
    and reads character from prev_pip, discarding the mulitiple of p
    prints only p, and passes along the nexts prime to be to a new process
    it will create
    left_pipe: pipe provded by the parent, over which new number are read
    p: the starting prime number against which to check for multiples
 */ 

void next_process(_pipe left_pipe, int p){
    printf("prime %d\n", p);

    int right_pipe[2];
    int value;

    while (1) {
        read(left_pipe[READ], &value, sizeof(int));
        if (value == -1) { // left pipe close, handle exit conditions
            if (!initialized(right_pipe)){
                // no children processes left, just exit
                exit(0);                
            }
            int end = -1;
            write(right_pipe[WRITE], &end, sizeof(int));
            wait((int*)0);
            exit(0);
        }
        if (value % p == 0){
            // discard multiples of the processe's assigned prime
            continue;
        }
        if (!initialized(right_pipe)){
            pipe(right_pipe);
            int pid = fork();
            if (pid == 0){ // child
                next_process(right_pipe, value);
            }
        }
        write(right_pipe[WRITE], &value, sizeof(int));    
    }
}

int main(int argc, char *argv[]) {    
    if (argc > 2){
        printf("Usage: primes <n> \n");
        exit(1);
    }
    int max_num;
    if (argc == 1){
        printf("max number is default 10\n");
        max_num = 10;
    } else {
        max_num = atoi(argv[1]);
    }

    int right_pipe[2];
    pipe(right_pipe);

    int pid = fork();
    if(pid == 0){ // child;
        int pid = fork();
        if(pid==0){ // child
            next_process(right_pipe, 2);
        }
        for (int i = 3; i <= max_num; i++){
            // short sleep helps to avoid jumbled prints
            sleep(1);
            write(right_pipe[WRITE], &i, sizeof(int));
        }
        // User specific value to end the connection with the child, close doesn't seem to work
        int end = -1;
        write(right_pipe[WRITE], &end, sizeof(int));
        wait((int*)0);

        exit(0);
    } else if (pid > 0){ // parent
        // wait for all children to be done.
        wait((int*)0);
    } else {
        // error
        printf("Error forking child from process %d\n", getpid());
        exit(1);
    }

    exit(0);
}

