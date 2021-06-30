# Install script for directory: /lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/lhome/sofinde/udacity/capstone/build/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/libio2d_cairo.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/xcairo.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/xcairo_brushes_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/xcairo_helpers.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/xcairo_paths_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/xcairo_surface_state_props_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/xcairo_surfaces_image_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/xcairo_surfaces_impl.h"
    "/lhome/sofinde/udacity/capstone/thirdparty/io2d/P0267_RefImpl/P0267_RefImpl/cairo/xio2d_cairo_main.h"
    )
endif()

