cmake_minimum_required(VERSION 3.12)

project(OpenCV_Multithread VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV 4.2.0 REQUIRED)
find_package(Threads REQUIRED)
add_executable(main main.cpp)

target_link_libraries(main ${OpenCV_LIBS} Threads::Threads)
