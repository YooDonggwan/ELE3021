#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10

typedef struct pcb{
    struct pcb *next; // pointer next struct
    char save_state[N];
    int pro_ID;
    int arrive_time;
    int burst_time;
    int execute_time;
    int wait_time;
}PCB

int 
