C Smart Pointers
================

[![Build Status](https://travis-ci.org/Snaipe/libcsptr.svg?branch=master)](https://travis-ci.org/Snaipe/libcsptr) 
[![Coverage Status](https://coveralls.io/repos/Snaipe/libcsptr/badge.svg?branch=master)](https://coveralls.io/r/Snaipe/libcsptr?branch=master) 
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](https://github.com/Snaipe/libcsptr/blob/master/LICENSE) 
[![Version](https://img.shields.io/github/tag/Snaipe/libcsptr.svg?label=version&style=flat)](https://github.com/Snaipe/libcsptr/releases)

## What this is

This project is an attempt to bring smart pointer constructs
to the (GNU) C programming language.

### Features

* `unique_ptr`, `shared_ptr` macros, and `smart` type attribute
* Destructor support for cleanup
* Custom variable metadata on allocation
* Cross-platform: tested under linux 3.18.6-1, Mac OS X Yosemite 10.10, and Windows 7 (with MinGW and the Cygwin port of GCC)

## Installing

### With a package manager

* Mac OS X: `brew install snaipe/soft/libcsptr`
* [AUR](https://aur.archlinux.org/packages/libcsptr-git/): `yaourt -S libcsptr`
* Ubuntu:

    ```bash
    $ sudo add-apt-repository ppa:snaipewastaken/ppa
    $ sudo apt-get update
    $ sudo apt-get install libcsptr-dev
    ```

### Building from source
#### Prerequisites

To compile the library, GCC 4.6+ is needed.

#### Installation

1. Clone this repository
2. run `mkdir build && cd $_ && cmake -DCMAKE_INSTALL_PREFIX=$HOME .. && make && make install`  
   from the project root for a local install, or run  
   `mkdir build && cd $_ && cmake -DCMAKE_INSTALL_PREFIX=/usr .. && make && sudo make install` for a global install.

## Examples

* Simple unique\_ptr:
    simple1.c:
    ```c
    #include <stdio.h>
    #include <csptr/smart_ptr.h>

    int main(void) {
        // some_int is an unique_ptr to an int with a value of 1.
        smart int *some_int = unique_ptr(int, 1);

        printf("%p = %d\n", some_int, *some_int);

        // some_int is destroyed here
        return 0;
    }
    ```
    Shell session:
    ```bash
    $ gcc -std=c99 -o simple1 simple1.c -lcsptr
    $ valgrind ./simple1
    ==3407== Memcheck, a memory error detector
    ==3407== Copyright (C) 2002-2013, and GNU GPL\'d, by Julian Seward et al.
    ==3407== Using Valgrind-3.10.0 and LibVEX; rerun with -h for copyright info
    ==3407== Command: ./test1
    ==3407==
    0x53db068 = 1
    ==3407==
    ==3407== HEAP SUMMARY:
    ==3407==     in use at exit: 0 bytes in 0 blocks
    ==3407==   total heap usage: 1 allocs, 1 frees, 48 bytes allocated
    ==3407==
    ==3407== All heap blocks were freed -- no leaks are possible
    ==3407==
    ==3407== For counts of detected and suppressed errors, rerun with: -v
    ==3407== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
    ```
* Simple unique\_ptr with destructor:
    ```c
    #include <unistd.h>
    #include <fcntl.h>
    #include <csptr/smart_ptr.h>

    struct log_file {
        int fd;
        // ...
    };

    void cleanup_log_file(void *ptr, void *meta) {
        (void) meta;
        close(((struct log_file *) ptr)->fd);
    }

    int main(void) {
        smart struct log_file *log = unique_ptr(struct log_file, {
                .fd = open("/dev/null", O_WRONLY | O_APPEND),
                // ...
            }, cleanup_log_file);

        write(log->fd, "Hello", 5);

        // cleanup_log_file is called, then log is freed
        return 0;
    }
    ```
* Allocating a smart array and printing its contents before destruction:
    ```c
    #include <stdio.h>
    #include <csptr/smart_ptr.h>
    #include <csptr/array.h>

    void print_int(void *ptr, void *meta) {
        (void) meta;
        // ptr points to the current element
        // meta points to the array metadata (global to the array), if any.
        printf("%d\n", *(int*) ptr);
    }

    int main(void) {
        // Destructors for array types are run on every element of the
        // array before destruction.
        smart int *ints = unique_ptr(int[5], {5, 4, 3, 2, 1}, print_int);
        // ints == {5, 4, 3, 2, 1}

        // Smart arrays are length-aware
        for (size_t i = 0; i < array_length(ints); ++i) {
            ints[i] = i + 1;
        }
        // ints == {1, 2, 3, 4, 5}

        return 0;
    }
    ```

## More examples

* Using a different memory allocator (although most will replace malloc/free):
    ```c
    #include <csptr/smart_ptr.h>

    void *some_allocator(size_t);
    void some_deallocator(void *);

    int main(void) {
        smalloc_allocator = (s_allocator) {some_allocator, some_deallocator};
        // ...
        return 0;
    }
    ```

* Automatic cleanup on error cases:
    ```c
    #include <unistd.h>
    #include <fcntl.h>
    #include <csptr/smart_ptr.h>

    struct log_file {
        int fd;
        // ...
    };

    static void close_log(void *ptr, void *meta) {
        (void) meta;
        struct log_file *log = ptr;
        if (log->fd != -1)
            close(log->fd);
    }

    struct log_file *open_log(const char *path) {
        smart struct log_file *log = shared_ptr(struct log_file, {0}, close_log);
        if (!log) // failure to allocate
            return NULL; // nothing happens, destructor is not called

        log->fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (log->fd == -1) // failure to open
            return NULL; // log gets destroyed, file descriptor is not closed since fd == -1.

        return sref(log); // a new reference on log is returned, it does not get destoyed
    }

    int main(void) {
        smart struct log_file *log = open_log("/dev/null");
        // ...
        return 0; // file descriptor is closed, log is freed
    }
    ```
* Using named parameters:
    ```c
    #include <csptr/smart_ptr.h>

    void nothing(void *ptr, void *meta) {}

    int main(void) {
        struct { int a; } m = { 1 };

        smart int *i = unique_ptr(int,
                .dtor = nothing,
                .value = 42,
                .meta = { &m, sizeof (m) }
            );

        return 0;
    }
    ```

## FAQ

**Q. Why didn't you use C++ you moron ?**  
A. Because when I first started this, I was working on a C project.
   Also, because it's fun.

**Q. Can I use this on a serious project ?**  
A. Yes, but as this project has not been widely used, there might be
   some bugs. Beware!

**Q. How did you make this ?**  
A. Here's a [link to my blog post](http://snaipe.me/c/c-smart-pointers/) on the matter. In short words, with `__attribute__ ((cleanup(func_to_free_stack)))`.
