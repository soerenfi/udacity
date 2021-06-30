cmake_minimum_required(VERSION 3.4)

project(MagnumTriangleExample CXX)
find_package(Magnum REQUIRED GL GlfwApplication)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(magnum-triangle MyApplication.cpp)
target_link_libraries(magnum-triangle PRIVATE
    Corrade::Main
    Magnum::GL
    Magnum::Application
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders)

install(TARGETS magnum-triangle DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT magnum-triangle)


add_subdirectory(mouseinteraction)