cmake_minimum_required(VERSION 3.11.3)

project(xodr_traffic_sim)


set(CMAKE_CXX_STANDARD 17)

# Add the path of the cmake files to the CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# find_package(io2d REQUIRED)
find_package(Cairo)
find_package(GraphicsMagick)

find_package(SDL2 REQUIRED)
set(SDL2_GFX_PATH "thirdparty/sdl2_gfx" CACHE BOOL "" FORCE)
#find_package(SDL2_gfx REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})


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
src/renderer.cpp
)

# Add project executable
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)

target_link_libraries(${PROJECT_NAME}
    PUBLIC io2d
    ${SDL2_LIBRARIES}
  pthread  tinyxml2)
  
  target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_GFX_INCLUDE_DIRS})
  # target_link_libraries(${PROJECT_NAME} ${SDL2_GFX_LIBRARIES})