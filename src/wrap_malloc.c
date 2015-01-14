#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "mman.h"
#undef smalloc

#ifdef SMALLOC_WRAP_MALLOC
void *(*real_malloc)(size_t) = NULL;
void (*real_free)(void *) = NULL;

__attribute__ ((constructor))
static void initialize_handles(void) {
# if defined(__APPLE__)
    void *handle = dlopen("libc.dylib", RTLD_NOW);
    real_malloc = (__typeof__(real_malloc)) dlsym(handle, "malloc");
    real_free   = (__typeof__(real_free))   dlsym(handle, "free");
# elif defined(__unix__)
    real_malloc = (__typeof__(real_malloc)) dlsym(RTLD_NEXT, "malloc");
    real_free   = (__typeof__(real_free))   dlsym(RTLD_NEXT, "free");
# else
#  error malloc wrapping not supported on non-*nix systems.
# endif
    if (real_malloc == NULL || real_free == NULL) {
        puts("Failed to locate original malloc or free; aborting.\n");
        exit(-1);
    }

    smalloc_allocator = (s_allocator) {real_malloc, real_free};
}

__attribute__ ((malloc))
void *malloc(size_t size) {
    return smalloc(size, 0, UNIQUE, 0);
}

void free(void *ptr) {
    sfree(ptr);
}
#endif /* !SMALLOC_WRAP_MALLOC */
