C Smart Pointers
================

## What this is

This project is a tentative to bring smart pointer constructs
to the C programming language.

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
        smart int *some_int = unique_ptr(sizeof (int));
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
        close(((struct log_file *) log)->fd);
    }

    int main(void) {
        smart struct log_file *log = unique_ptr(sizeof (struct log_file), cleanup_log_file);
        *log = (struct log_file) {
            .fd = open("/dev/null", O_WRONLY | O_APPEND)
        };

        write(log->fd, "Hello", 5);

        // cleanup_log_file is called, then log is freed
        return 0;
    }
    ```
* Simple metadata usage to implement length-aware arrays:
    ```c
    #include <csptr/smart_ptr.h>

    void *new_array(size_t size, size_t len) {
        return unique_ptr(size * len, NULL, &len, sizeof (len));
    }

    size_t length(void *arr) {
        size_t *size = get_smart_ptr_meta(arr);
        return size ? *size : 0; // size may be NULL if arr is invalid
    }

    int main(void) {
        smart int *arr = new_array(sizeof (int), 4);
        for (size_t i = 0; i < length(arr); ++i)
            arr[i] = i;

        // ...
        // arr is destroyed
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
        smart struct log_file *log = shared_ptr(sizeof (struct log_file), close_log);
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
        smart C *c = unique_ptr(sizeof (C), destroy_c);
        *c = (C) {
            .a = unique_ptr(sizeof (A)),
            .b = unique_ptr(sizeof (B)),
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
A. Altough I used these on three projects already, I would not.
   It's *probably* fine, but this is still a toy project, and has yet to
   mature -- beware of bugs !
