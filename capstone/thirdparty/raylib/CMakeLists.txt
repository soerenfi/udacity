cmake_minimum_required(VERSION 3.11.3)

project(raylib_example)


set(CMAKE_CXX_STANDARD 17)

# Add the path of the cmake files to the CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)



add_subdirectory(raylib)

# Add project executable
add_executable(${PROJECT_NAME}
 main.cpp
)
# Add project executable

target_link_libraries(${PROJECT_NAME}
    PUBLIC raylib
  )