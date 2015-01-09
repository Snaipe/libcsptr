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

#ifndef CSPTR_MMAN_H_
# define CSPTR_MMAN_H_

# include <stddef.h>
# include <malloc.h>
# include "vararg.h"

enum pointer_kind {
    UNIQUE,
    SHARED
};

typedef void (*f_destructor)(void *, void *);

typedef struct {
    void *(*alloc)(size_t);
    void (*dealloc)(void *);
} s_allocator;

extern s_allocator smalloc_allocator;

void *get_smart_ptr_meta(void *ptr);
void *sref(void *ptr);
__attribute__((malloc))
void *smalloc(size_t size, int kind, int argcount, ...);
void sfree(void *ptr);

# define smalloc(Size, Kind, Args...) smalloc(Size, Kind, ARG_LENGTH(Args), ## Args)

#endif /* !CSPTR_MMAN_H_ */
