cmake_minimum_required(VERSION 2.8)

project(csptr C)

# Project setup & environment variables

enable_testing()

set(PROJECT_VERSION "2.0.5-2")
set(MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})

if (NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g -std=gnu99 -Wno-unused-result -Wno-missing-field-initializers")
endif ()

if (WIN32 AND NOT MSVC)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no-undefined")
endif()

# Setup coveralls

option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

if (COVERALLS AND NOT LIBCSPTR_DISABLE_COVERALLS)
  include(Coveralls)
  coveralls_turn_on_coverage()
endif()

# Find dependencies

option(LIBCSPTR_TESTS "Turn on tests" ON)

find_package(Check)
if (CHECK_FOUND AND LIBCSPTR_TESTS AND NOT LIBCSPTR_DISABLE_TESTS)
  add_subdirectory(check)
endif()

# Setup options

option(SENTINEL "Use a sentinel for the variadic function arguments" ON)
option(FIXED_ALLOCATOR "Define if malloc should always be used" OFF)

# Generate the configure file

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/csptr/config.h"
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
  include/csptr/common.h
)

set(MAN_FILES doc/csptr.3)

if (NOT WIN32)
  add_definitions("-fPIC")
endif ()

include_directories(include/csptr src)
add_library(csptr STATIC ${SOURCE_FILES} ${INTERFACE_FILES})

if (COVERALLS AND NOT LIBCSPTR_DISABLE_COVERALLS)
  coveralls_setup("${SOURCE_FILES}" ${COVERALLS_UPLOAD})
endif()

install(FILES ${INTERFACE_FILES} DESTINATION include/csptr)
install(FILES ${MAN_FILES} DESTINATION share/man/man3)
install(TARGETS csptr
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake"
)

# Debian package

function(extract_version version major minor patch extra)
  string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)(.*)?" version_valid ${version})
  if(version_valid)
    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)(.*)?" "\\1;\\2;\\3;\\4" VERSION_MATCHES ${version})
    list(GET VERSION_MATCHES 0 version_major)
    set(${major} ${version_major} PARENT_SCOPE)
    list(GET VERSION_MATCHES 1 version_minor)
    set(${minor} ${version_minor} PARENT_SCOPE)
    list(GET VERSION_MATCHES 2 version_patch)
    set(${patch} ${version_patch} PARENT_SCOPE)
    list(GET VERSION_MATCHES 3 version_extra)
    set(${extra} ${version_extra} PARENT_SCOPE)
  else(version_valid)
    message(AUTHOR_WARNING "Bad version ${version}; falling back to 0 (have you made an initial release?)")
    set(${major} "0" PARENT_SCOPE)
    set(${minor} "" PARENT_SCOPE)
    set(${patch} "" PARENT_SCOPE)
    set(${extra} "" PARENT_SCOPE)
  endif(version_valid)
endfunction(extract_version)

if (WIN32)
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_SOURCE_GENERATOR "ZIP")
else ()
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_SOURCE_GENERATOR "TGZ")
endif ()

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Smart pointers library for the C programming language.")
set(CPACK_PACKAGE_FILE_NAME           "lib${CMAKE_PROJECT_NAME}-binary-${PROJECT_VERSION}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME    "lib${CMAKE_PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY   "lib${CMAKE_PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR              "Franklin \"Snaipe\" Mathieu")
set(CPACK_PACKAGE_CONTACT             "Franklin \"Snaipe\" Mathieu <franklinmathieu@gmail.com>")
set(CPACK_PACKAGE_VERSION             "${PROJECT_VERSION}")

if (WIN32)
  # add snapshot specific versioning information
  if (CPACK_DEBIAN_PACKAGE_TYPE STREQUAL "snapshot")
    execute_process(COMMAND date +%Y%m%d%0k%0M%0S%z OUTPUT_VARIABLE SNAPSHOT_DATE_TIME)
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}-snapshot-${SNAPSHOT_DATE_TIME}")
    STRING(REPLACE "\n" "" CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
  endif ()
endif ()

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_HOME_DIRECTORY}/LICENSE")
extract_version(${PROJECT_VERSION}
    CPACK_PACKAGE_VERSION_MAJOR
    CPACK_PACKAGE_VERSION_MINOR
    CPACK_PACKAGE_VERSION_PATCH
    VERSION_EXTRA
)

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

file(GLOB TRASH_FILES "${CMAKE_SOURCE_DIR}/*")
set(KEEP_FILES
  "${CMAKE_SOURCE_DIR}/.cmake"
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_SOURCE_DIR}/doc"
  "${CMAKE_SOURCE_DIR}/CMakeLists.txt"
  "${CMAKE_SOURCE_DIR}/README.md"
  "${CMAKE_SOURCE_DIR}/INSTALL.md"
  "${CMAKE_SOURCE_DIR}/LICENSE"
  "${CMAKE_SOURCE_DIR}/ChangeLog"
  "${CMAKE_SOURCE_DIR}/description.txt"
)
list(REMOVE_ITEM TRASH_FILES ${KEEP_FILES})
# Escape any '.' characters
string(REPLACE "." "\\\\." TRASH_FILES "${TRASH_FILES}")
set(CPACK_SOURCE_IGNORE_FILES "${TRASH_FILES}")

include(CPack)

IF(UNIX)
# build a Debian package for Launchpad
set(CPACK_DEBIAN_PACKAGE_NAME "lib${CMAKE_PROJECT_NAME}")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/Snaipe/lib${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_HOME_DIRECTORY}/description.txt")
set(CPACK_DEBIAN_PACKAGE_LICENSE "expat")
set(CPACK_COMPONENTS_ALL "dev")
set(CPACK_COMPONENT_DEV_DISPLAY_NAME "libcsptr Library development files.")
set(CPACK_COMPONENT_DEV_DESCRIPTION "These are the development files.")
set(CPACK_COMPONENT_DEV_SECTION "libdevel")
set(CPACK_DEBIAN_PACKAGE_BUILD_NUMBER_PREFIX "0ubuntu")
set(CPACK_DEBIAN_PACKAGE_BUILD_NUMBER "1")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Franklin \"Snaipe\" Mathieu <franklinmathieu@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://launchpad.net/~snaipewastaken/+archive/ubuntu/ppa")
set(CPACK_DEBIAN_PACKAGE_SNAPSHOT_HOMEPAGE "https://launchpad.net/~snaipewastaken/+archive/ubuntu/ppa")
set(DPUT_HOST "snaipewastaken-ppa")
set(DPUT_SNAPSHOT_HOST "snaipewastaken-ppa")
set(CPACK_DEBIAN_BUILD_DEPENDS "debhelper (>=5), pkg-config (>=0.20-1~)")
set(CPACK_DEBIAN_PACKAGE_INSTALL "")
set(CPACK_DEBIAN_PACKAGE_DOCS "")
set(CPACK_COMPONENT_DEV_DOCS "/usr/share/man/*")
set(CPACK_COMPONENT_DEV_INSTALL "/usr/include" "/usr/lib/*.a")

include(UploadPPA)
ENDIF(UNIX)
