#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

enum
{
    BUF_SIZE = 4096
};

int remove_safe(const char *filename, const char *spec)
{
   if(remove(filename)) {
        fprintf(stderr, "Error removing %s file: [%d] %s\n", spec, errno, strerror(errno));
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if(argc != 3) {
        fprintf(stderr, "Error: invalid arguments.\n");
        return 1;
    }

    FILE *src_file = fopen(argv[1], "rb");
    if(!src_file) {
        fprintf(stderr, "Error opening source file: [%d] %s\n", errno, strerror(errno));
        return 2;
    }

    FILE *dst_file = fopen(argv[2], "wb");
    if(!src_file) {
        fprintf(stderr, "Error opening destination file: [%d] %s\n", errno, strerror(errno));
        return 2;
    }

    size_t bytes_read;
    char buf[BUF_SIZE];
    while((bytes_read = fread(buf, 1, BUF_SIZE, src_file))) {
        if(ferror(src_file)) {
            fprintf(stderr, "Error reading source file: [%d] %s\n", errno, strerror(errno));
            remove_safe(argv[2], "destination");
            return 3;
        }

        if(!fwrite(buf, bytes_read, 1, dst_file)) {
            fprintf(stderr, "Error writing to destination file: [%d] %s\n", errno, strerror(errno));
            remove_safe(argv[2], "destination");
            return 4;
        }
    }

    if(fclose(dst_file)) {
        fprintf(stderr, "Error closing destination file: [%d] %s\n", errno, strerror(errno));
        remove_safe(argv[2], "destination");
        return 5;
    }
    printf("aaaaaa\n");
    if(fclose(src_file)) {
        fprintf(stderr, "Error closing source file: [%d] %s\n", errno, strerror(errno));
        return 6;
    }
    printf("aaa\n");

    if(remove_safe(argv[1], "source")) {
        return 7;
    }

    return 0;
} 
