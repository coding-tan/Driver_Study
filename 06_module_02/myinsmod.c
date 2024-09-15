#include <stdio.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define init_module(mod, len, opts) syscall(__NR_init_module, mod, len, opts)

int main(int argc, char **argv){
    int fd;
    int ret;
    struct stat statbuf;
    size_t image_size;
    char *image;
    
    fd = open(argv[1], O_RDONLY | O_CLOEXEC);

    if(fd < 0){
        printf("open error \n");
        return -1;
    }
    fstat(fd, &statbuf);

    image_size = statbuf.st_size;
    image = malloc(image_size);
    read(fd, image, image_size);

    ret = init_module(image, image_size, "");
    if(ret < 0){
        printf("error\n");
    }
    else{
        printf("ok\n");
    }
    free(image);
    return ret;
}
