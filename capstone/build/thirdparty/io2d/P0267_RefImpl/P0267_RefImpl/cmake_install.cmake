# Install script for directory: /lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/lhome/sofinde/udacity/capstone/build/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/libio2d_core.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/test_process.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/test_renderer.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xbrushes.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xbrushes_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xcolor.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xdiagnostics.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xgraphicsmath.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xgraphicsmath_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xgraphicsmathfloat.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xgraphicsmathfloat_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xinput.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xinterchangebuffer.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xio2d.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xio2d_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xpath.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xpath_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xpathbuilder_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xsurfaces.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xsurfaces_enums.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xsurfaces_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xsurfacesprops_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xsystemheaders.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/xtext.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/io2d/io2dConfig.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/io2d/io2dConfig.cmake"
         "/lhome/sofinde/udacity/capstone/build/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/CMakeFiles/Export/lib/cmake/io2d/io2dConfig.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/io2d/io2dConfig-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/io2d/io2dConfig.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/io2d" TYPE FILE FILES "/lhome/sofinde/udacity/capstone/build/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/CMakeFiles/Export/lib/cmake/io2d/io2dConfig.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/io2d" TYPE FILE FILES "/lhome/sofinde/udacity/capstone/build/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/CMakeFiles/Export/lib/cmake/io2d/io2dConfig-noconfig.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/lhome/sofinde/udacity/capstone/build/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/cmake_install.cmake")
  include("/lhome/sofinde/udacity/capstone/build/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/xlib/cmake_install.cmake")

endif()

