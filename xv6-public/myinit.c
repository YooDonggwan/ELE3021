#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char *argv[] = { "sh", 0};

int main(){
    int pid, wpid;

    if(open("console", O_RDWR) < 0){
        mknod("console", 1, 1);
        open("console", O_RDWR);
    }

    dup(0);
    dup(0);


    for(;;){
        printf(1, "Hello, xv6!\n5\n4\n3\n2\n1\nstart!!\n");
        pid = fork();

        if(pid < 0){
            printf(1, "init: fork failed\n");
            exit();
        }
        if(pid == 0){
            exec("sh",argv);
            printf(1, "init: exec sh failde\n");
            exit();
        }
        while((wpid=wait()) >= 0 && wpid != pid)
            printf(1, "zombie!\n");
    }
}

