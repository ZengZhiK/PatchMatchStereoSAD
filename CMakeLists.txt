cmake_minimum_required(VERSION 3.22)
project(PatchMatch)

set(CMAKE_CXX_STANDARD 11)
message("=> FLAG ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

set(OpenCV_DIR "D:\\DevTools\\OpenCV\\opencv-4.7.0\\mingw-build")
find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(PatchMatch main.cpp pm.cpp pm.h)

target_link_libraries(PatchMatch ${OpenCV_LIBS} OpenMP::OpenMP_CXX)
