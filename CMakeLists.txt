cmake_minimum_required(VERSION 3.22)
project(PatchMatch)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fopenmp")

set(OpenCV_DIR "D:\\DevTools\\OpenCV\\opencv-4.7.0\\mingw-build")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(PatchMatch main.cpp pm.h pm.cpp)

target_link_libraries(PatchMatch ${OpenCV_LIBS})
