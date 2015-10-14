# Compilation & installation instructions

## CMake options

### Developer options

These options are useful for developers that want to customize libcsptr.

* `-DSENTINEL=<ON|OFF>`: Use a sentinel for the variadic function parameters.  
    This must be left on for maximum compatibility. However, in case you target
    embedded systems *and* your C compiler supports empty compound literals as
    a nonstandard extension, you may want to turn it off.  
    This option is **ON** by default.
* `-DFIXED_ALLOCATOR=<ON|OFF>`: If turned on, directly use malloc/free as the
    memory allocator, and ignore any allocator provided by `smalloc_allocator`.  
    This option is **OFF** by default.

### Maintainer options

These options target maintainers of libcsptr, and are not otherwise useful.

* `-DLIBCSPTR_TESTS=<ON|OFF>`: Compiles tests alongside libcsptr.  
    This option is **ON** by default.
* `-DCOVERALLS=<ON|OFF>`: Turns on coverage for libcsptr.  
    This option is **OFF** by default.
* `-DCOVERALLS_UPLOAD=<ON|OFF>`: Uploads coverage for libcsptr to coveralls.  
    The option is only relevant if `-DCOVERALLS=ON` is specified.  
    This option is **ON** by default.

