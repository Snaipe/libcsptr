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

#include "config.h"
#include "mman.h"
#include "array.h"
#include "wrap_malloc.h"
#undef smalloc

#define INLINE __attribute__ ((always_inline)) inline

s_allocator smalloc_allocator = {malloc, free};

typedef struct {
    enum pointer_kind kind;
    f_destructor dtor;
#ifndef NDEBUG
    void *ptr;
#endif /* !NDEBUG */
} s_meta;

typedef struct {
    s_meta;
    _Atomic size_t ref_count;
} s_meta_shared;

INLINE static size_t align(size_t s) {
    return (s + (sizeof (void *) - 1)) & ~(sizeof (void *) - 1);
}

__attribute__ ((pure))
INLINE static s_meta *get_meta(void *ptr) {
    size_t *size = (size_t *) ptr - 1;
    return (s_meta *) ((char *) size - *size);
}

__attribute__ ((pure))
void *get_smart_ptr_meta(void *ptr) {
    assert((size_t) ptr == align((size_t) ptr));

    s_meta *meta = get_meta(ptr);
    assert(meta->ptr == ptr);

    size_t head_size = meta->kind & SHARED ? sizeof (s_meta_shared) : sizeof (s_meta);
    size_t *metasize = (size_t *) ptr - 1;
    if (*metasize == head_size)
        return NULL;

    return (char *) meta + head_size;
}

void *sref(void *ptr) {
    s_meta *meta = get_meta(ptr);
    assert(meta->ptr == ptr);
    assert(meta->kind == SHARED);
    ((s_meta_shared *) meta)->ref_count++;
    return ptr;
}

__attribute__ ((malloc))
INLINE static void *alloc_entry(size_t head, size_t size, size_t metasize) {
    const size_t totalsize = head + size + metasize + sizeof (size_t);
#ifdef SMALLOC_FIXED_ALLOCATOR
# ifdef SMALLOC_WRAP_MALLOC
    return real_malloc(totalsize);
# else /* !SMALLOC_WRAP_MALLOC */
    return malloc(totalsize);
# endif /* !SMALLOC_WRAP_MALLOC */
#else /* !SMALLOC_FIXED_ALLOCATOR */
    return smalloc_allocator.alloc(totalsize);
#endif /* !SMALLOC_FIXED_ALLOCATOR */
}

INLINE static void dealloc_entry(s_meta *meta, void *ptr) {
    if (meta->dtor) {
        void *user_meta = get_smart_ptr_meta(ptr);
        if (meta->kind & ARRAY) {
            s_meta_array *arr_meta = (void *) (meta + 1);
            for (size_t i = 0; i < arr_meta->nmemb; ++i)
                meta->dtor(ptr + arr_meta->size * i, user_meta);
        } else
            meta->dtor(ptr, user_meta);
    }

#ifdef SMALLOC_FIXED_ALLOCATOR
# ifdef SMALLOC_WRAP_MALLOC
    real_free(meta);
# else /* !SMALLOC_WRAP_MALLOC */
    free(meta);
# endif /* !SMALLOC_WRAP_MALLOC */
#else /* !SMALLOC_FIXED_ALLOCATOR */
    smalloc_allocator.dealloc(meta);
#endif /* !SMALLOC_FIXED_ALLOCATOR */
}

__attribute__ ((malloc))
static void *smalloc_impl(size_t size, int kind, f_destructor dtor, void *meta, size_t metasize) {
    if (!size)
        return NULL;

    // align the sizes to the size of a word
    size_t aligned_metasize = align(metasize);
    size = align(size);

    size_t head_size = kind & SHARED ? sizeof (s_meta_shared) : sizeof (s_meta);
    s_meta_shared *ptr = alloc_entry(head_size, size, aligned_metasize);
    if (ptr == NULL)
        return NULL;

    char *shifted = (char *) ptr + head_size;
    if (metasize && meta)
        memcpy(shifted, meta, metasize);

    size_t *sz = (size_t *) (shifted + aligned_metasize);
    *sz = head_size + aligned_metasize;

    *(s_meta*) ptr = (s_meta) {
        .kind = kind,
        .dtor = dtor,
#ifndef NDEBUG
        .ptr = sz + 1
#endif
    };

    if (kind & SHARED)
        ptr->ref_count = ATOMIC_VAR_INIT(1);

    return sz + 1;
}

__attribute__ ((malloc))
INLINE static void *smalloc_array(size_t size, size_t nmemb, int kind, f_destructor dtor, void *meta, size_t metasize) {
    char new_meta[align(metasize + sizeof(s_meta_array))];
    s_meta_array *arr_meta = (void *) new_meta;
    *arr_meta = (s_meta_array) {
        .size = size,
        .nmemb = nmemb
    };
    memcpy(arr_meta + 1, meta, metasize);
    return smalloc_impl(nmemb * size, kind | ARRAY, dtor, &new_meta, sizeof (new_meta));
}

__attribute__ ((malloc))
void *smalloc(size_t size, size_t nmemb, int kind, int count, ...) {
    va_list args;

    int params = 0;
    if (count == 2) {
        ++count;
        ++params;
    }
    if (count > 3)
        count = 3;

    va_start(args, count);

    struct {
        f_destructor dtor;
        void *meta_ptr;
        size_t meta_size;
    } values = { NULL, NULL, 0 };

    while (params < count) {
        switch (params++) {
            case 0: values.dtor      = va_arg(args, f_destructor); break;
            case 1: values.meta_ptr  = va_arg(args, void *);       break;
            case 2: values.meta_size = va_arg(args, size_t);       break;
        }
    }

    va_end(args);
    if (nmemb == 0)
        return smalloc_impl(size, kind, values.dtor, values.meta_ptr, values.meta_size);
    else
        return smalloc_array(size, nmemb, kind, values.dtor, values.meta_ptr, values.meta_size);
}

void sfree(void *ptr) {
    if (!ptr) return;

    assert((size_t) ptr == align((size_t) ptr));
    s_meta *meta = get_meta(ptr);
    assert(meta->ptr == ptr);

    if (meta->kind == SHARED && --((s_meta_shared *) meta)->ref_count)
        return;

    dealloc_entry(meta, ptr);
}
