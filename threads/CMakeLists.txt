cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
project (threads)

add_executable(${PROJECT_NAME}
    src/threads.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
)

target_link_libraries(${PROJECT_NAME}
    pthread
)
