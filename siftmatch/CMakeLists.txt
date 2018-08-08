
cmake_minimum_required(VERSION 2.8)

project(siftmatch)

SET(CMAKE_C_COMPILER "g++") 
add_definitions(-std=c++11)

find_package(OpenCV REQUIRED)

include_directories(. ${OpenCV_INCLUDE_DIRS})

find_package(Eigen3)

INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})

include_directories( "/usr/include/eigen3")


FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./../)

add_executable(siftmatchmain 
main.cpp
test_aysift.h
aysift.h
aysift.cpp
util.h
util.cpp
)

target_link_libraries(siftmatchmain ${OpenCV_LIBRARIES}  ${OpenMP_LIBS})