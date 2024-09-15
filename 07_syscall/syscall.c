#include <stdio.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define __NR_helloworld 435

int main(int argc, char **argv){
    printf("syscall test\n");
    syscall(__NR_helloworld);

    return 0;
}
