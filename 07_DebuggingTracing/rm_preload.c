#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <string.h>

typedef int (*unlink_syscall_t)(int, const char*, int);

int unlinkat(int fd, const char* name, int flag) {
    if (strstr(name, "FIX")) {
        errno = EPERM;
        return -1;
    }
    return ((unlink_syscall_t)dlsym(RTLD_NEXT, "unlinkat"))(fd, name, flag);
}
