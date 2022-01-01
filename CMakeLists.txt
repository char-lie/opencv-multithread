cmake_minimum_required(VERSION 3.12)

project(OPENCV_MULTITHREAD VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
  main
  lib/main.cpp
  lib/thread_handle.cpp
  lib/processors.cpp
  lib/average_intensity_calculator.cpp
)

find_package(OpenCV 4.2.0 REQUIRED)
target_link_libraries(main ${OpenCV_LIBS})

find_package(Threads REQUIRED)
target_link_libraries(main Threads::Threads)

target_include_directories(
  main
  PUBLIC
  ${OpenCV_INCLUDE_DIRS};
  "${OPENCV_MULTITHREAD_SOURCE_DIR}/include"
)
