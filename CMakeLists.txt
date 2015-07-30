cmake_minimum_required(VERSION 2.8)

project(csptr C)

# Project setup & environment variables

enable_testing()

set(PROJECT_VERSION "2.0.4")
set(MODULE_DIR "${CMAKE_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g -std=gnu99 -Wno-unused-result -Wno-missing-field-initializers -fplan9-extensions")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no-undefined")

# Setup coveralls

option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

if (COVERALLS)
  include(Coveralls)
  coveralls_turn_on_coverage()
endif()

# Find dependencies

find_package(Check)
if (CHECK_FOUND)
  add_subdirectory(check)
endif()

# Setup options

option(SENTINEL "Use a sentinel for the variadic function arguments" ON)
option(FIXED_ALLOCATOR "Define if malloc should always be used" OFF)

# Generate the configure file

configure_file(
  "${CMAKE_SOURCE_DIR}/src/config.h.in"
  "${CMAKE_SOURCE_DIR}/include/csptr/config.h"
)

# List sources and headers

set(SOURCE_FILES
    src/mman.c
    src/mman.h
)

set(INTERFACE_FILES
  include/csptr/config.h
  include/csptr/smalloc.h
  include/csptr/array.h
  include/csptr/smart_ptr.h
  include/csptr/config.h
)

include_directories(include/csptr src)
add_library(csptr SHARED ${SOURCE_FILES} ${INTERFACE_FILES})

if (COVERALLS)
  coveralls_setup("${SOURCE_FILES}" ${COVERALLS_UPLOAD})
endif()

install(FILES ${INTERFACE_FILES} DESTINATION include/csptr)
install(TARGETS csptr
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake"
)
