#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 1048576

int buf_write(int sock, char *buf, int size) {
    int offset = 0;
    int ret;

    while (offset < size) {
        ret = write(sock, buf+offset, size-offset);
        if (ret < 0) {
            fprintf(stderr, "write error\n");
            exit(1);
        }
        if (!ret) {
            fprintf(stderr, "write eof\n");
            exit(1);
        }
        offset += ret;
    }
    return size;
}

int main(int argc, char **argv) {
    int fd;
    ssize_t size;
    char buf[BUF_SIZE];
    int i;

    if (argc <= 1) {
        fd = 0;
        while ((size = read(fd, buf, BUF_SIZE)) > 0)
            buf_write(1, buf, size);
    } else {
        for (i = 1; i < argc; i++) {
            fd = open(argv[i], O_RDONLY);
            if (fd >= 0) {
                while ((size = read(fd, buf, BUF_SIZE)) > 0)
                    buf_write(1, buf, size);
                close(fd);
            }
        }
    }

    return(0);
}
