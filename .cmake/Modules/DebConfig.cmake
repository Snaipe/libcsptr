# build a Debian package for Launchpad
set(CPACK_DEBIAN_PACKAGE_NAME "libcsptr-dev")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/Snaipe/libcsptr")
set(CPACK_DEBIAN_BUILD_DEPENDS debhelper cmake)
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/description.txt")

set(CPACK_DEBIAN_PACKAGE_SOURCE_COPY "${CMAKE_SOURCE_DIR}/.cmake/copy-source.sh")

set(CPACK_DEBIAN_DISTRIBUTION_NAME ubuntu)
set(CPACK_DEBIAN_DISTRIBUTION_RELEASES precise trusty vivid wily xenial)

set(DPUT_HOST "snaipewastaken-ppa")
set(DPUT_SNAPSHOT_HOST "snaipewastaken-ppa")
set(CPACK_DEBIAN_PACKAGE_DOCS "/usr/share/man/*")
set(CPACK_DEBIAN_PACKAGE_INSTALL "/usr/include" "/usr/lib/*.a")

include (DebSourcePPA)
