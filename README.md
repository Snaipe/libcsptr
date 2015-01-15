C Smart Pointers
================

## What this is

This project is a tentative attempt to bring smart pointer constructs
to the (GNU) C programming language.

* `unique_ptr`, `shared_ptr` macros, and `smart` type attribute
* Destructor support for cleanup
* Custom variable metadata on allocation

## Installing

### Prerequisites

* To compile the library, GCC 4.9+ is needed.
* To compile any project with the library, Clang or GCC are needed.

### Installation

1. Clone this repository
2. run `./autogen.sh && ./configure --prefix=$HOME/ && make && make install`
   from the project root for a local install, or run
   `./autogen.sh && ./configure && make && sudo make install` for a global
   install.

## Examples

* Simple unique\_ptr:
    simple1.c:
    ```c
    #include <stdio.h>
    #include <csptr/smart_ptr.h>

    int main(void) {
        smart int *some_int = unique_ptr(int);
        *some_int = 1;

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
        smart struct log_file *log = unique_ptr(struct log_file, cleanup_log_file);
        *log = (struct log_file) {
            .fd = open("/dev/null", O_WRONLY | O_APPEND)
        };

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
        smart int *ints = unique_ptr(int[10], print_int);

        // Smart arrays are length-aware
        for (size_t i = 0; i < array_length(ints); ++i) {
            ints[i] = i;
        }

        // Not initializing the array before getting out of scope
        // is undefined behavior: beware !
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
        smart struct log_file *log = shared_ptr(struct log_file, close_log);
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
* Destructor macro helper:
    ```c
    #include <csptr/smart_ptr.h>

    typedef struct {
        // ...
    } A;

    typedef struct {
        // ...
    } B;

    typedef struct {
        A *a;
        int some_int;
        B *b;
    } C;

    DESTRUCTOR(destroy_c, static, a, b) {
        printf("some_int = %d at destruction.\n", ptr->some_int); // why not ?
    }

    int main(void) {
        smart C *c = unique_ptr(C, destroy_c);
        *c = (C) {
            .a = unique_ptr(A),
            .b = unique_ptr(B),
            .some_int = 42
        };
        return 0; // c->a, c->b, and c are freed
    }
    ```

## FAQ

**Q. Why didn't you use C++ you moron ?**  
A. Because when I first started this, I was working on a C project.
   Also, because it's fun.

**Q. Can I use this on a serious project ?**  
A. Yes, but the project has yet to fully mature, beware of bugs!

**Q. How did you make this ?**  
A. Here's a [link to my blog post](http://snaipe.me/c/c-smart-pointers/) on the matter.
