/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "mman.h"
#undef smalloc

#ifdef SMALLOC_WRAP_MALLOC
void *(*real_malloc)(size_t) = NULL;
void *(*real_realloc)(size_t) = NULL;
void (*real_free)(void *) = NULL;

__attribute__ ((constructor))
static void initialize_handles(void) {
# if defined(__APPLE__)
    void *handle = dlopen("libc.dylib", RTLD_NOW);
    real_malloc  = (__typeof__(real_malloc))  dlsym(handle, "malloc");
    real_realloc = (__typeof__(real_realloc)) dlsym(handle, "realloc");
    real_free    = (__typeof__(real_free))    dlsym(handle, "free");
# elif defined(__unix__)
    real_malloc  = (__typeof__(real_malloc))  dlsym(RTLD_NEXT, "malloc");
    real_realloc = (__typeof__(real_realloc)) dlsym(RTLD_NEXT, "realloc");
    real_free    = (__typeof__(real_free))    dlsym(RTLD_NEXT, "free");
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

__attribute__ ((malloc))
void *calloc(size_t size, size_t nmemb) {
    void *ptr = smalloc(size, nmemb, UNIQUE, 0);
    memset(ptr, 0, size * nmemb);
}

void free(void *ptr) {
    sfree(ptr);
}

void *realloc(void *ptr, size_t size) {
    return real_realloc(ptr ? get_meta(ptr) : NULL, size);
}
#endif /* !SMALLOC_WRAP_MALLOC */
