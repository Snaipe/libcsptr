##
# Copyright (c) 2012 Karsten Ohme <k_o_@users.sourceforge.net>
# Based on a script from Daniel Pfeifer <daniel@pfeifer-mail.de> Copyright (c) 2010
#
# This script builds an Ubuntu/Debian source package with "make package_ubuntu". It also offers a convenient way to upload it by dput to Launchpad.
#It assumes a working install and package_source configuration (CPACK_PACKAGE_* variables in CMakeLists.txt CMake configuration file) with all files in place.
# Also a PGP key for signing the package is necessary. The key is referenced by the CPACK_DEBIAN_PACKAGE_MAINTAINER setting. For uploading the package to Launchpad this should be the Lauchpad PGP key.
# You should also be familiar with the meaning of snapshot and release versions. See https://help.launchpad.net/Packaging/PPA/BuildingASourcePackage
#
# USAGE: INCLUDE IN PROJECT
#
#  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#  include(UploadPPA)
# Add the UploadPPA.cmake file to the projects source directory.
# 
#
# From the resulting dsc file in the Debian directory also a Ubuntu/Debian package can be build:
#
# dpkg-source -x foo.dsc
# cd <CPACK_DEBIAN_PACKAGE_NAME>
# fakeroot debian/rules binary
#
# When uploading it to Launchpad with dput you must be registered there and you must have a .dput.cf in place. Follow the instructions on https://help.launchpad.net/Packaging/PPA/Uploading
#
# Known problems:
#
# If you set the build type SET(CMAKE_BUILD_TYPE "Release") then the cmakeinstall.cmake file does not install the binary files
#
#
#
# CPACK_DEBIAN_PACKAGE_NAME - used as "Source" in control file, default TOLOWER "${CPACK_PACKAGE_NAME}"
# CPACK_DEBIAN_PACKAGE_PRIORITY - used as "Priority" in control file, default "optional"
# CPACK_DEBIAN_PACKAGE_SECTION - used as "Section" in control file, default "devel"
# CPACK_DEBIAN_PACKAGE_HOMEPAGE - used in "Homepage" field in control file
# CPACK_DEBIAN_BUILD_DEPENDS - use in "Build-Depends" field in control file, cmake is automatically added
# CPACK_DEBIAN_PACKAGE_DEPENDS - used as "Depends" field in the control file, default "${shlibs:Depends}, ${misc:Depends} ${CPACK_PACKAGE_NAME} (= ${binary:Version}) " 
# for components. Instead of "binary:Version" "Source-Version" is used if CPACK_DEBIAN_PACKAGE_DISTRIBUTION is "dapper"  
# 
# CPACK_DEBIAN_PACKAGE_INSTALL - specifies which files have to be installed for the main package separated by ";". This is a space separated list. The file path must be absolute to the root directory of the installation 
# e.g. "/usr/lib/*.so". * wildcards can be used
# CPACK_DEBIAN_PACKAGE_DOCS - specifies which files have to be installed for the main package separated by ";". This is a space separated list. The file path must be absolute to the root directory of the installation 
# e.g. "/usr/share/docs/${CPACK_DEBIAN_PACKAGE_NAME}-dev/README". * wildcards can be used
#
# CPACK_PACKAGE_DESCRIPTION_FILE - used main text in "Description" field of control file
# CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR - used as upstream author in copyright file (format: Name <email>), default ${CPACK_DEBIAN_PACKAGE_MAINTAINER}
# CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR_NAME - used as upstream author name in copyright file, default ${CPACK_DEBIAN_PACKAGE_HOMEPAGE}  
# CPACK_DEBIAN_PACKAGE_UPSTREAM_COPYRIGHT_YEAR - used as the copyright year in copyright file, default this year
# CPACK_DEBIAN_PACKAGE_UPSTREAM_URL - used as upstream url in copyright file, default CPACK_DEBIAN_PACKAGE_HOMEPAGE
# CPACK_DEBIAN_PACKAGE_LICENSE - used as license indicator for the copyright file generation, possible values gpl, lgpl, bsd, apache
# CPACK_DEBIAN_PACKAGE_DISTRIBUTION - distribution name, default "precise"
#
# CPACK_DEBIAN_PACKAGE_RECOMMENDS - used as "Recommends" field in control file
# CPACK_DEBIAN_PACKAGE_SUGGESTS - used as "Suggests" field in control file
# CPACK_DEBIAN_PACKAGE_PREDEPENDS - used as "Pre-Depends" field in control file
# CPACK_DEBIAN_PACKAGE_BREAKS - used as "Breaks" field in control file
# CPACK_DEBIAN_PACKAGE_CONFLICTS - used as "Conflicts" field in control file
# CPACK_DEBIAN_PACKAGE_REPLACES - used as "Replaces" field in control file
# CPACK_DEBIAN_PACKAGE_ENHANCES - used as "Enhances" field in control file
#
#
#
# CPACK_COMPONENTS_ALL - list of additional components separated by ";" - used for the "Package" field in the control file for additional components of a package, e.g. for development files for a library this would be "dev"
# The component must match the used components in the CMake file. The component is appended to ${CPACK_DEBIAN_PACKAGE_NAME} with a dash: e.g.: ${CPACK_DEBIAN_PACKAGE_NAME}-dev
# CPACK_COMPONENT_${COMPONENT}_DISPLAY_NAME - used as additional short description for the "Description" field of additional packages. ${COMPONENT} must be in the list of CPACK_COMPONENTS_ALL, e.g. dev
# CPACK_COMPONENT_${COMPONENT}_DESCRIPTION - used as additional description for the "Description" field of additional packages. ${COMPONENT} must be in the list of CPACK_COMPONENTS_ALL, e.g. dev
# CPACK_COMPONENT_${COMPONENT}_DEPENDS - used for the "Depends" field of additional packages. ${COMPONENT} must be in the list of CPACK_COMPONENTS_ALL, e.g. dev
# CPACK_COMPONENT_${COMPONENT}_SECTION - used for the "Section" field of additional packages. ${COMPONENT} must be in the list of CPACK_COMPONENTS_ALL, e.g. dev
# CPACK_COMPONENT_${COMPONENT}_INSTALL - specifies which files have to be installed for this component separated by ";". The file path must be absolute to the root directory of the installation 
# e.g. "/usr/lib/*.so". * wildcards can be used
# CPACK_COMPONENT_${COMPONENT}_DOCS - specifies which files have to be installed for this component separated by ";". The file path must be absolute to the root directory of the installation 
# e.g. "/usr/share/docs/${CPACK_DEBIAN_PACKAGE_NAME}/README". * wildcards can be used
# CPACK_DEBIAN_PACKAGE_MANPAGES - specifies which files have to be installed as manual pages. Separated by ";".
# CPACK_DEBIAN_CUSTOM_BUILD_COMMAND - A custom build command, e.g. create documentation with something like "make doc". The build artifact's path created somewhere in the build directory must be referenced in the CMake INSTALL command by ${CMAKE_CURRENT_BINARY_DIR} variable. E.g.: INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc DESTINATION ${DOCUMENTATION_DIRECTORY} COMPONENT dev OPTIONAL)
# The OPTIONAL keyword does not work under CMake 2.6, so you have to use a more difficult construct:
# INSTALL(
#CODE "FILE (GLOB_RECURSE ALL_DOCS \"${CMAKE_CURRENT_BINARY_DIR}/doc/html/*\")"
#CODE "FILE (INSTALL DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${DOCUMENTATION_DIRECTORY}/html\" TYPE FILE FILES \${ALL_DOCS})" 
#COMPONENT dev)
# Here the "\" before ${CMAKE_INSTALL_PREFIX} is important, so this variable is resolved at runtime first.
#
# CPACK_DEBIAN_PACKAGE_MAINTAINER - used as "Maintainer" field in control and copyright file, default ${CPACK_PACKAGE_CONTACT}, also used as reference for the GPG signing key
# CPACK_DEBIAN_PACKAGE_HOMEPAGE - used as "Homepage" in control file, default ${CPACK_PACKAGE_VENDOR}
# CPACK_DEBIAN_PACKAGE_SNAPSHOT_HOMEPAGE - used as "Homepage" in control file if this is a snapshot build, default ${CPACK_PACKAGE_VENDOR}
# CPACK_PACKAGE_DESCRIPTION_SUMMARY - used as first line of all "Description" fields for all packages in control file
#
# DPUT_HOST - used as host for dput for uploading the file to Launchpad
# DPUT_SNAPSHOT_HOST - used as snapshot host for dput for uploading the file to Launchpad
# CPACK_DEBIAN_PACKAGE_TYPE - used for determining the build type, "snapshot" or "release" is possible, default snapshot
#
# The format of the resulting versioning scheme is the following: ${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_BUILD_NUMBER_PREFIX}${CPACK_DEBIAN_PACKAGE_BUILD_NUMBER}~${CPACK_DEBIAN_PACKAGE_DISTRIBUTION}
#${CPACK_DEBIAN_PACKAGE_BUILD_NUMBER_PREFIX}${CPACK_DEBIAN_PACKAGE_BUILD_NUMBER}~${CPACK_DEBIAN_PACKAGE_DISTRIBUTION} is the Debian/Ubuntu revision. See http://www.debian.org/doc/debian-policy/ch-controlfields.html#s-f-Version.
# An example is libfoo1-1.2.0-0ubuntu1~maverick 
# "0ubuntu1" stands for the first package in Ubuntu when there is no Debian package existing and "maverick" is the used Ubuntu series
#
# To generate always a unique code version for snapshots you must should also set ${CPACK_PACKAGE_VERSION} to a unique increasing package version number. This can be done by appending a [<currentVersion>+<nextVersion>SNAPSHOT<DATE>] part to the package version number. The package version number cannot contain hyphens because a Debian/Ubuntu revision follows. 
# Assuming the libtool versioning scheme http://www.gnu.org/software/libtool/manual/html_node/Libtool-versioning.html an example could be: "1.2.0+1SNAPSHOT20110402031738+0200", when the current version is 1.2.0 and the next version in development is 1.2.1. The date format is using a variant of the ISO date format.
#
# CPACK_DEBIAN_PACKAGE_BUILD_NUMBER_PREFIX - used as a prefix for the build number, default ""
# CPACK_DEBIAN_PACKAGE_BUILD_NUMBER - used for the build number, default 1
# CPACK_DEBIAN_NATIVE_PACKAGE - if set a native package is build, default not set
#
#
##

find_program(DEBUILD_EXECUTABLE debuild)
find_program(DPUT_EXECUTABLE dput)

if(NOT DEBUILD_EXECUTABLE OR NOT DPUT_EXECUTABLE)
  MESSAGE(WARNING "${DEBUILD_EXECUTABLE} or ${DPUT_EXECUTABLE} not installed.")
  return()
endif(NOT DEBUILD_EXECUTABLE OR NOT DPUT_EXECUTABLE)

# set default package type
IF(NOT CPACK_DEBIAN_PACKAGE_TYPE)
  set(CPACK_DEBIAN_PACKAGE_TYPE "snapshot")
ENDIF(NOT CPACK_DEBIAN_PACKAGE_TYPE)

# set default distribution
IF(NOT CPACK_DEBIAN_PACKAGE_DISTRIBUTION)
  set(CPACK_DEBIAN_PACKAGE_DISTRIBUTION "precise")
ENDIF(NOT CPACK_DEBIAN_PACKAGE_DISTRIBUTION)

# Set the version name
set(VERSION_NAME "${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_BUILD_NUMBER_PREFIX}${CPACK_DEBIAN_PACKAGE_BUILD_NUMBER}~${CPACK_DEBIAN_PACKAGE_DISTRIBUTION}")

# DEBIAN/control
# debian policy enforce lower case for package name
# Package: (mandatory)
IF(NOT CPACK_DEBIAN_PACKAGE_NAME)
  STRING(TOLOWER "${CPACK_PACKAGE_NAME}" CPACK_DEBIAN_PACKAGE_NAME)
ENDIF(NOT CPACK_DEBIAN_PACKAGE_NAME)

# Maintainer:
IF(NOT CPACK_DEBIAN_PACKAGE_MAINTAINER)
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_DEBIAN_PACKAGE_HOMEPAGE})
ENDIF(NOT CPACK_DEBIAN_PACKAGE_MAINTAINER)

# Homepage:
IF(NOT CPACK_DEBIAN_PACKAGE_HOMEPAGE)
ENDIF(NOT CPACK_DEBIAN_PACKAGE_HOMEPAGE)

# Section: (recommended)
IF(NOT CPACK_DEBIAN_PACKAGE_SECTION)
  SET(CPACK_DEBIAN_PACKAGE_SECTION "devel")
ENDIF(NOT CPACK_DEBIAN_PACKAGE_SECTION)

# Priority: (recommended)
IF(NOT CPACK_DEBIAN_PACKAGE_PRIORITY)
  SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
ENDIF(NOT CPACK_DEBIAN_PACKAGE_PRIORITY)

# Section: 
set(CPACK_DEBIAN_PACKAGE_DEPENDS "\${shlibs:Depends}, \${misc:Depends}")

# Description:  
file(STRINGS ${CPACK_PACKAGE_DESCRIPTION_FILE} DESC_LINES)
foreach(LINE ${DESC_LINES})
  set(DEB_LONG_DESCRIPTION "${DEB_LONG_DESCRIPTION} ${LINE}\n")
endforeach(LINE ${DESC_LINES})

file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/Debian)
set(DEBIAN_SOURCE_DIR ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})

file(MAKE_DIRECTORY ${DEBIAN_SOURCE_DIR}/debian)

##############################################################################
# debian/control
set(DEBIAN_CONTROL ${DEBIAN_SOURCE_DIR}/debian/control)
file(WRITE ${DEBIAN_CONTROL}
  "Source: ${CPACK_DEBIAN_PACKAGE_NAME}\n"
  "Section: ${CPACK_DEBIAN_PACKAGE_SECTION}\n"
  "Priority: ${CPACK_DEBIAN_PACKAGE_PRIORITY}\n"
  "Maintainer: ${CPACK_DEBIAN_PACKAGE_MAINTAINER}\n"
  "Build-Depends: "
  )

foreach(DEP ${CPACK_DEBIAN_BUILD_DEPENDS})
  file(APPEND ${DEBIAN_CONTROL} "${DEP}, ")
endforeach(DEP ${CPACK_DEBIAN_BUILD_DEPENDS})  

file(APPEND ${DEBIAN_CONTROL} "cmake\n"
  "Standards-Version: 3.8.4\n"
  )


IF(CPACK_DEBIAN_PACKAGE_TYPE STREQUAL "snapshot")
  file(APPEND ${DEBIAN_CONTROL} "Homepage: ${CPACK_DEBIAN_PACKAGE_SNAPSHOT_HOMEPAGE}\n")
ELSE(CPACK_DEBIAN_PACKAGE_TYPE STREQUAL "snapshot")
  file(APPEND ${DEBIAN_CONTROL} "Homepage: ${CPACK_DEBIAN_PACKAGE_HOMEPAGE}\n")
ENDIF(CPACK_DEBIAN_PACKAGE_TYPE STREQUAL "snapshot")

file(APPEND ${DEBIAN_CONTROL}
  "\n"
  "Package: ${CPACK_DEBIAN_PACKAGE_NAME}\n"
  "Architecture: any\n"
  "Depends: ${CPACK_DEBIAN_PACKAGE_DEPENDS}\n"
  )

# Recommends:  
IF(CPACK_DEBIAN_PACKAGE_RECOMMENDS)
file(APPEND ${DEBIAN_CONTROL}
  "Recommends: ${CPACK_DEBIAN_PACKAGE_RECOMMENDS}\n"
  )
ENDIF(CPACK_DEBIAN_PACKAGE_RECOMMENDS)

# Suggests
IF(CPACK_DEBIAN_PACKAGE_SUGGESTS)
  file(APPEND ${DEBIAN_CONTROL}
    "Suggests: ${CPACK_DEBIAN_PACKAGE_SUGGESTS}\n"
    )
ENDIF(CPACK_DEBIAN_PACKAGE_SUGGESTS)

# Pre-Depends:
IF(CPACK_DEBIAN_PACKAGE_PREDEPENDS)
file(APPEND ${DEBIAN_CONTROL}
  "Pre-Depends: ${CPACK_DEBIAN_PACKAGE_PREDEPENDS}\n"
  )
ENDIF(CPACK_DEBIAN_PACKAGE_PREDEPENDS)

# Conflicts: 
IF(CPACK_DEBIAN_PACKAGE_CONFLICTS)
file(APPEND ${DEBIAN_CONTROL}
  "Conflicts: ${CPACK_DEBIAN_PACKAGE_CONFLICTS}\n"
  )
ENDIF(CPACK_DEBIAN_PACKAGE_CONFLICTS)

# Enhances:
IF(CPACK_DEBIAN_PACKAGE_ENHANCES)
file(APPEND ${DEBIAN_CONTROL}
  "Enhances: ${CPACK_DEBIAN_PACKAGE_ENHANCES}\n"
  )
ENDIF(CPACK_DEBIAN_PACKAGE_ENHANCES)

# Replaces:
IF(CPACK_DEBIAN_PACKAGE_REPLACES)
file(APPEND ${DEBIAN_CONTROL}
  "Replaces: ${CPACK_DEBIAN_PACKAGE_REPLACES}\n"
  )
ENDIF(CPACK_DEBIAN_PACKAGE_REPLACES)

file(APPEND ${DEBIAN_CONTROL}
  "Description: ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n"
  "${DEB_LONG_DESCRIPTION}"
  )
  
foreach(COMPONENT ${CPACK_COMPONENTS_ALL})
  string(TOUPPER ${COMPONENT} UPPER_COMPONENT)
  set(DEPENDS "${CPACK_DEBIAN_PACKAGE_NAME}")
  IF(CPACK_DEBIAN_PACKAGE_DISTRIBUTION STREQUAL "dapper")
    set(DEPENDS "${DEPENDS} (= \${Source-Version})")
  ELSE(CPACK_DEBIAN_PACKAGE_DISTRIBUTION STREQUAL "dapper")
    set(DEPENDS "${DEPENDS} (= \${binary:Version})")
  ENDIF(CPACK_DEBIAN_PACKAGE_DISTRIBUTION STREQUAL "dapper")   
  foreach(DEP ${CPACK_COMPONENT_${UPPER_COMPONENT}_DEPENDS})
      set(DEPENDS "${DEPENDS}, ${CPACK_DEBIAN_PACKAGE_NAME}-${DEP}")
  endforeach(DEP ${CPACK_COMPONENT_${UPPER_COMPONENT}_DEPENDS})
  file(APPEND ${DEBIAN_CONTROL} "\n"
    "Package: ${CPACK_DEBIAN_PACKAGE_NAME}-${COMPONENT}\n"
    "Section: ${CPACK_COMPONENT_${UPPER_COMPONENT}_SECTION}\n"
    "Architecture: any\n"
    "Depends: ${DEPENDS}\n"
    "Description: ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}"
    ": ${CPACK_COMPONENT_${UPPER_COMPONENT}_DISPLAY_NAME}\n"
    "${DEB_LONG_DESCRIPTION}"
    " .\n"
    " ${CPACK_COMPONENT_${UPPER_COMPONENT}_DESCRIPTION}\n"
    )
endforeach(COMPONENT ${CPACK_COMPONENTS_ALL})

##############################################################################
# debian/copyright
set(DEBIAN_COPYRIGHT ${DEBIAN_SOURCE_DIR}/debian/copyright)

IF(NOT CPACK_DEBIAN_PACKAGE_LICENSE)
  set(CPACK_DEBIAN_PACKAGE_LICENSE gpl)
ENDIF(NOT CPACK_DEBIAN_PACKAGE_LICENSE)

IF(NOT CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR)
  set(CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR ${CPACK_DEBIAN_PACKAGE_MAINTAINER})
ENDIF(NOT CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR)

IF(NOT CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR_NAME)
  set(CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR_NAME ${CPACK_DEBIAN_PACKAGE_HOMEPAGE})
ENDIF(NOT CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR_NAME)

IF(NOT CPACK_DEBIAN_PACKAGE_UPSTREAM_URL)
  set(CPACK_DEBIAN_PACKAGE_UPSTREAM_URL ${CPACK_DEBIAN_PACKAGE_HOMEPAGE})
ENDIF(NOT CPACK_DEBIAN_PACKAGE_UPSTREAM_URL) 

IF(NOT CPACK_DEBIAN_PACKAGE_UPSTREAM_COPYRIGHT_YEAR)
  execute_process(COMMAND date +%Y OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_UPSTREAM_COPYRIGHT_YEAR)
  STRING(REPLACE "\n" "" CPACK_DEBIAN_PACKAGE_UPSTREAM_COPYRIGHT_YEAR ${CPACK_DEBIAN_PACKAGE_UPSTREAM_COPYRIGHT_YEAR})
ENDIF(NOT CPACK_DEBIAN_PACKAGE_UPSTREAM_COPYRIGHT_YEAR) 

execute_process(COMMAND date +%Y OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_YEAR)
STRING(REPLACE "\n" "" CPACK_DEBIAN_PACKAGE_YEAR ${CPACK_DEBIAN_PACKAGE_YEAR})

execute_process(COMMAND date -R OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_DATE)
STRING(REPLACE "\n" "" CPACK_DEBIAN_PACKAGE_DATE ${CPACK_DEBIAN_PACKAGE_DATE})

set(POSSIBLE_LICENSES gpl lgpl bsd apache expat)
list(FIND POSSIBLE_LICENSES ${CPACK_DEBIAN_PACKAGE_LICENSE} CPACK_DEBIAN_PACKAGE_LICENSE_FOUND)

IF(CPACK_DEBIAN_PACKAGE_LICENSE_FOUND EQUAL "-1")
  message(FATAL_ERROR "License ${CPACK_DEBIAN_PACKAGE_LICENSE} is unknown.")
ENDIF(CPACK_DEBIAN_PACKAGE_LICENSE_FOUND EQUAL "-1")

find_path(COPYRIGHT_FILES_DIRECTORY copyright.${CPACK_DEBIAN_PACKAGE_LICENSE} PATHS ${CMAKE_MODULE_PATH})
execute_process(COMMAND ${CMAKE_COMMAND} -E
  copy ${COPYRIGHT_FILES_DIRECTORY}/copyright.${CPACK_DEBIAN_PACKAGE_LICENSE} ${DEBIAN_COPYRIGHT}
  )

# Here also CONFIGURE_FILE might be better
FILE(READ ${DEBIAN_COPYRIGHT} COPYRIGHT_TEMP)
STRING(REPLACE "<Maintainer>" ${CPACK_DEBIAN_PACKAGE_MAINTAINER} COPYRIGHT_TEMP ${COPYRIGHT_TEMP})
STRING(REPLACE "<Date>" ${CPACK_DEBIAN_PACKAGE_DATE} COPYRIGHT_TEMP ${COPYRIGHT_TEMP})
STRING(REPLACE "<Year>" ${CPACK_DEBIAN_PACKAGE_YEAR} COPYRIGHT_TEMP ${COPYRIGHT_TEMP})
STRING(REPLACE "<UpstreamURL>" ${CPACK_DEBIAN_PACKAGE_UPSTREAM_URL} COPYRIGHT_TEMP ${COPYRIGHT_TEMP})
STRING(REPLACE "<UpstreamAuthor>" ${CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR} COPYRIGHT_TEMP ${COPYRIGHT_TEMP})
STRING(REPLACE "<YearUpstreamCopyright>" ${CPACK_DEBIAN_PACKAGE_UPSTREAM_COPYRIGHT_YEAR} COPYRIGHT_TEMP ${COPYRIGHT_TEMP})
STRING(REPLACE "<UpstreamAuthorName>" ${CPACK_DEBIAN_PACKAGE_UPSTREAM_AUTHOR_NAME} COPYRIGHT_TEMP ${COPYRIGHT_TEMP})     
FILE(WRITE ${DEBIAN_COPYRIGHT} ${COPYRIGHT_TEMP})

##############################################################################
# debian/${CPACK_DEBIAN_PACKAGE_NAME}.install
set(DEBIAN_INSTALL_FILE ${DEBIAN_SOURCE_DIR}/debian/${CPACK_DEBIAN_PACKAGE_NAME}.install)
foreach(INSTALL_UNIT ${CPACK_DEBIAN_PACKAGE_INSTALL})
  file(APPEND ${DEBIAN_INSTALL_FILE} "debian/tmp${INSTALL_UNIT}\n")
endforeach(INSTALL_UNIT ${CPACK_DEBIAN_PACKAGE_INSTALL})  

##############################################################################
# debian/${CPACK_DEBIAN_PACKAGE_NAME}.docs
set(DEBIAN_DOCS_FILE ${DEBIAN_SOURCE_DIR}/debian/${CPACK_DEBIAN_PACKAGE_NAME}.docs)
foreach(INSTALL_UNIT ${CPACK_DEBIAN_PACKAGE_DOCS})
  file(APPEND ${DEBIAN_DOCS_FILE} "debian/tmp${INSTALL_UNIT}\n")
endforeach(INSTALL_UNIT ${CPACK_DEBIAN_PACKAGE_DOCS})  

##############################################################################
# debian/${CPACK_DEBIAN_PACKAGE_NAME}.manpages
set(DEBIAN_MANPAGES_FILE ${DEBIAN_SOURCE_DIR}/debian/${CPACK_DEBIAN_PACKAGE_NAME}.manpages)
foreach(INSTALL_UNIT ${CPACK_DEBIAN_PACKAGE_MANPAGES})
  file(APPEND ${DEBIAN_MANPAGES_FILE} "debian/tmp${INSTALL_UNIT}\n")
endforeach(INSTALL_UNIT ${CPACK_DEBIAN_PACKAGE_MANPAGES})  

foreach(COMPONENT ${CPACK_COMPONENTS_ALL})
string(TOUPPER ${COMPONENT} UPPER_COMPONENT)

##############################################################################
# debian/${CPACK_DEBIAN_PACKAGE_NAME}-${COMPONENT}.install
set(DEBIAN_INSTALL_FILE ${DEBIAN_SOURCE_DIR}/debian/${CPACK_DEBIAN_PACKAGE_NAME}-${COMPONENT}.install)
foreach(INSTALL_UNIT ${CPACK_COMPONENT_${UPPER_COMPONENT}_INSTALL})
  file(APPEND ${DEBIAN_INSTALL_FILE} "debian/tmp${INSTALL_UNIT}\n")
endforeach(INSTALL_UNIT ${CPACK_COMPONENT_${UPPER_COMPONENT}_INSTALL})  

##############################################################################
# debian/${CPACK_DEBIAN_PACKAGE_NAME}-${COMPONENT}.docs
set(DEBIAN_DOCS_FILE ${DEBIAN_SOURCE_DIR}/debian/${CPACK_DEBIAN_PACKAGE_NAME}-${COMPONENT}.docs)
foreach(INSTALL_UNIT ${CPACK_COMPONENT_${UPPER_COMPONENT}_DOCS})
  file(APPEND ${DEBIAN_DOCS_FILE} "debian/tmp${INSTALL_UNIT}\n")
endforeach(INSTALL_UNIT ${CPACK_COMPONENT_${UPPER_COMPONENT}_DOCS})  

##############################################################################
# debian/${CPACK_DEBIAN_PACKAGE_NAME}-${COMPONENT}.manpages
set(DEBIAN_MANPAGES_FILE ${DEBIAN_SOURCE_DIR}/debian/${CPACK_DEBIAN_PACKAGE_NAME}-${COMPONENT}.manpages)
foreach(INSTALL_UNIT ${CPACK_COMPONENT_${UPPER_COMPONENT}_MANPAGES})
  file(APPEND ${DEBIAN_MANPAGES_FILE} "debian/tmp${INSTALL_UNIT}\n")
endforeach(INSTALL_UNIT ${CPACK_COMPONENT_${UPPER_COMPONENT}_MANPAGES})  

endforeach(COMPONENT ${CPACK_COMPONENTS_ALL})

##############################################################################
# debian/rules
set(DEBIAN_RULES ${DEBIAN_SOURCE_DIR}/debian/rules)
file(WRITE ${DEBIAN_RULES}
  "#!/usr/bin/make -f\n"
  "\n"
  "BUILDDIR = build_dir\n"
  "\n"
  "build:\n"
  "	mkdir $(BUILDDIR)\n"
  "	cd $(BUILDDIR); cmake ..\n"
  "	cd $(BUILDDIR); ${CPACK_DEBIAN_CUSTOM_BUILD_COMMAND}\n"
  "	make -C $(BUILDDIR) preinstall\n"
  "	touch build\n"
  "\n"
  "binary: binary-indep binary-arch\n"
  "\n"
  "binary-indep: build\n"
  "\n"
  "binary-arch: build\n"
  "	cd $(BUILDDIR); cmake -DCMAKE_INSTALL_PREFIX=../debian/tmp/usr -P cmake_install.cmake\n"
  "	dh_testdir\n"
  "	dh_testroot\n"
  "	dh_installchangelogs\n"
  "	dh_installdocs\n" 
  "	dh_installman\n" 
  "	dh_install\n"
  "	dh_link\n"
  "	dh_strip\n"
  "	dh_compress\n"
  "	dh_fixperms\n"
  "	dh_listpackages\n"
  "	dh_makeshlibs\n"
  "	dh_gencontrol\n"
  "	dh_md5sums\n"
  "	dh_builddeb\n"
  )

# sh_libs is not resolved. Maybe OK, maybe wrong. This is a try.
#  "	dpkg-shlibdeps -e./debian/tmp/usr/lib/libglobalplatform.so\n"
#  "	dh_shlibdeps\n"
#  "	mkdir debian/tmp/DEBIAN\n"
#"	dpkg-gensymbols -plibglobalplatform\n"
#  "	dpkg-shlibdeps -elibglobalplatform\n"
#  "	dpkg-gencontrol -p${CPACK_DEBIAN_PACKAGE_NAME}\n"
#  "	dpkg --build debian/tmp ..\n"

  
foreach(COMPONENT ${CPACK_COMPONENTS_ALL})
  set(PATH debian/tmp_${COMPONENT})
  set(PACKAGE ${CPACK_DEBIAN_PACKAGE_NAME}-${COMPONENT})
  file(APPEND ${DEBIAN_RULES}
    "	cd $(BUILDDIR); cmake -DCOMPONENT=${COMPONENT} -DCMAKE_INSTALL_PREFIX=../${PATH}/usr -P cmake_install.cmake\n"
    "	dh_testdir\n"
    "	dh_testroot\n"
    "	dh_installchangelogs\n"
    "	dh_installdocs\n"
    "	dh_installman\n" 
    "	dh_install\n"
    "	dh_link\n"
    "	dh_strip\n"
    "	dh_compress\n"
    "	dh_fixperms\n"
    "	dh_listpackages\n"
    "	dh_makeshlibs\n"
    "	dh_gencontrol\n"
    "	dh_md5sums\n"
    "	dh_builddeb\n"
    )
endforeach(COMPONENT ${CPACK_COMPONENTS_ALL})

# sh_libs is not resolved. Maybe OK, maybe wrong. This is a try.
#    "	dpkg-shlibdeps -e./debian/tmp/usr/lib/libglobalplatform.so\n"
#    "	dh_shlibdeps\n"
#    "	mkdir ${PATH}/DEBIAN\n"
#  "	dh_shlibdeps -L libglobalplatform6 -l ../debian/tmp/usr/lib\n"
#   "	dpkg-gensymbols -plibglobalplatform -P${PATH}\n"
#   "	dpkg-shlibdeps -elibglobalplatform\n"
#    "	dpkg-gencontrol -p${PACKAGE} -P${PATH}\n"
#    "	dpkg --build ${PATH} ..\n"
 
file(APPEND ${DEBIAN_RULES}
  "\n"
  "clean:\n"
  "	rm -f build\n"
  "	rm -rf $(BUILDDIR)\n"
  "\n"
  ".PHONY: binary binary-arch binary-indep clean\n"
  )

execute_process(COMMAND chmod +x ${DEBIAN_RULES})

##############################################################################
# debian/compat - 5 is used to also support dapper
file(WRITE ${DEBIAN_SOURCE_DIR}/debian/compat "5")

##############################################################################
# debian/source/format
IF(CPACK_DEBIAN_NATIVE_PACKAGE)
  file(WRITE ${DEBIAN_SOURCE_DIR}/debian/source/format "3.0 (native)")
ELSE(CPACK_DEBIAN_NATIVE_PACKAGE)
  file(WRITE ${DEBIAN_SOURCE_DIR}/debian/source/format "3.0 (quilt)")
ENDIF(CPACK_DEBIAN_NATIVE_PACKAGE)

##############################################################################
# debian/changelog


set(DEBIAN_CHANGELOG ${DEBIAN_SOURCE_DIR}/debian/changelog)
execute_process(COMMAND date -R  OUTPUT_VARIABLE DATE_TIME)


file(WRITE ${DEBIAN_CHANGELOG}
  "${CPACK_DEBIAN_PACKAGE_NAME} (${VERSION_NAME}) ${CPACK_DEBIAN_PACKAGE_DISTRIBUTION}; urgency=low\n\n"
  "  * Package built with CMake\n\n"
  " -- ${CPACK_DEBIAN_PACKAGE_MAINTAINER}  ${DATE_TIME}"
  )

#INSTALL(FILES ${CMAKE_CURRENT_BUILD_DIR}/*.so DESTINATION lib${LIB_SUFFIX})

##############################################################################
# debuild -S
set(DEB_SOURCE_CHANGES
  ${CPACK_DEBIAN_PACKAGE_NAME}_${VERSION_NAME}_source.changes
  )

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Debian/${DEB_SOURCE_CHANGES}
  COMMAND ${DEBUILD_EXECUTABLE} -S 
DEPENDS ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}.orig.tar.gz
  WORKING_DIRECTORY ${DEBIAN_SOURCE_DIR}  
)

IF(${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} STREQUAL ${CPACK_SOURCE_PACKAGE_FILE_NAME})
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}.orig.tar.gz
  COMMAND make -C ${CMAKE_BINARY_DIR} package_source  
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}.orig.tar.gz
  COMMAND tar xzf ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}.orig.tar.gz -C ${CMAKE_BINARY_DIR}/Debian/
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
ELSE(${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} STREQUAL ${CPACK_SOURCE_PACKAGE_FILE_NAME})
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}.orig.tar.gz
  COMMAND make -C ${CMAKE_BINARY_DIR} package_source  
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}.orig.tar.gz
  COMMAND tar xzf ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}.orig.tar.gz -C ${CMAKE_BINARY_DIR}/Debian/
  COMMAND cp -unR ${CMAKE_BINARY_DIR}/Debian/${CPACK_SOURCE_PACKAGE_FILE_NAME}/* ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
ENDIF(${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} STREQUAL ${CPACK_SOURCE_PACKAGE_FILE_NAME})

add_custom_target(package_ubuntu 
  DEPENDS ${CMAKE_BINARY_DIR}/Debian/${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}.orig.tar.gz ${CMAKE_BINARY_DIR}/Debian/${DEB_SOURCE_CHANGES}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian
  )

##############################################################################
# dput ppa:your-lp-id/ppa <source.changes>
IF(CPACK_DEBIAN_PACKAGE_TYPE STREQUAL "snapshot")
  set(USED_DPUT_HOST ${DPUT_SNAPSHOT_HOST})
ELSE(CPACK_DEBIAN_PACKAGE_TYPE STREQUAL "snapshot")
  set(USED_DPUT_HOST ${DPUT_HOST})
ENDIF(CPACK_DEBIAN_PACKAGE_TYPE STREQUAL "snapshot")


add_custom_target(dput ${DPUT_EXECUTABLE} ${USED_DPUT_HOST} ${DEB_SOURCE_CHANGES}
  DEPENDS ${CMAKE_BINARY_DIR}/Debian/${DEB_SOURCE_CHANGES}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian
  )

# Also clean Debian directory
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${DEBIAN_SOURCE_DIR})
