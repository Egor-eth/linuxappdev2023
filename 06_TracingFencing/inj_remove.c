#include <string.h>
#include <dlfcn.h>

typedef int (*remove_type)(const char *);

int remove(const char *filename) {
    if(strstr(filename, "PROTECT") != NULL) {
        return 0;
    }

    return ((remove_type)(dlsym(RTLD_NEXT, "remove")))(filename);
}
