cmake_minimum_required(VERSION 3.11.3)

project(xodr_traffic_sim)


set(CMAKE_CXX_STANDARD 17)

# Add the path of the cmake files to the CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# find_package(io2d REQUIRED)
find_package(Cairo)
find_package(GraphicsMagick)

add_subdirectory(thirdparty/raylib)

# Set IO2D flags
set(IO2D_WITHOUT_SAMPLES 1)
set(IO2D_WITHOUT_TESTS 1)


add_subdirectory(thirdparty/tinyxml2)
add_subdirectory(thirdparty/io2d)

# Add project executable
add_executable(${PROJECT_NAME}
 src/main.cpp
src/tsim_map.cpp
src/tsim_object.cpp
src/opendrive_parser.cpp
src/tsim_graphics.cpp
#src/graphics_raylib.cpp
)

# Add project executable

target_link_libraries(${PROJECT_NAME}
    PUBLIC io2d
    raylib
  pthread  tinyxml2)