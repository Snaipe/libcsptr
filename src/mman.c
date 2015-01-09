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

#include <errno.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <string.h>
#include <assert.h>

#include "mman.h"
#undef smalloc

#define INLINE __attribute__ ((always_inline)) inline

s_allocator smalloc_allocator = {malloc, free};

typedef struct {
    enum pointer_kind kind;
    _Atomic size_t ref_count;
    void * ptr;
    f_destructor dtor;
} s_meta;

INLINE size_t align(size_t s) {
    return (s + (sizeof (void *) - 1)) & ~(sizeof (void *) - 1);
}

static s_meta *get_meta(void *ptr) {
    size_t *size = (size_t *) ptr - 1;
    return (s_meta *) ((char *) size - *size - sizeof (s_meta));
}

void *get_smart_ptr_meta(void *ptr) {
    assert((size_t) ptr == align((size_t) ptr));

    size_t *metasize = (size_t *) ptr - 1;
    if (*metasize == 0)
        return NULL;
    return (char *) metasize - *metasize;
}

void *sref(void *ptr) {
    s_meta *meta = get_meta(ptr);
    assert(meta->kind == SHARED);
    meta->ref_count++;
    return ptr;
}

static void *alloc_entry(size_t size, size_t metasize) {
    return smalloc_allocator.alloc(sizeof (s_meta) + size + metasize + sizeof (void *));
}

static void dealloc_entry(s_meta *meta, void *ptr) {
    if (meta->dtor)
        meta->dtor(ptr, get_smart_ptr_meta(ptr));

    smalloc_allocator.dealloc(meta);
}

__attribute__((malloc))
static void *smalloc_impl(size_t size, int kind, f_destructor dtor, void *meta, size_t metasize) {
    // align the sizes to the size of a word
    metasize = align(metasize);
    size = align(size);

    s_meta *ptr = alloc_entry(size, metasize);

    char *shifted = (char *) (ptr + 1);
    if (metasize && meta)
        memcpy(shifted, meta, metasize);

    size_t *sz = (size_t *) (shifted + metasize);
    *sz = metasize;

    *ptr = (s_meta) {
        .kind = kind,
        .ref_count = ATOMIC_VAR_INIT(1),
        .ptr = sz + 1,
        .dtor = dtor
    };

    return ptr->ptr;
}

__attribute__((malloc))
void *smalloc(size_t size, int kind, int count, ...) {
    va_list args;

    if (count == 2) {
        errno = EINVAL;
        return NULL;
    }
    if (count > 3)
        count = 3;

    va_start(args, count);

    struct {
        f_destructor dtor;
        void *meta_ptr;
        size_t meta_size;
    } values = { NULL, NULL, 0 };
    int params = 0;

    while (params < count) {
        switch (params++) {
            case 0: values.dtor      = va_arg(args, f_destructor); break;
            case 1: values.meta_ptr  = va_arg(args, void *);       break;
            case 2: values.meta_size = va_arg(args, size_t);       break;
        }
    }

    va_end(args);
    return smalloc_impl(size, kind, values.dtor, values.meta_ptr, values.meta_size);
}

void sfree(void *ptr) {
    if (!ptr) return;

    assert((size_t) ptr == align((size_t) ptr));
    s_meta *meta = get_meta(ptr);
    assert(meta->ptr == ptr);

    if (meta->kind == SHARED && --meta->ref_count)
        return;

    dealloc_entry(meta, ptr);
}
