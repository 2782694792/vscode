cmake_minimum_required(VERSION 3.0.0)
project(CMGCC7.3.0 VERSION 0.1.0)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

set(INCLUDE_DIR include)
include_directories(${INCLUDE_DIR})
set(LIB_DIR lib)
link_directories(${LIB_DIR})
aux_source_directory(main MAIN)
set(src SRC)
# 动态库文件
#add_library(cpps_lib_shared SHARED "2212\\*.cpp")
# 链接hellolib库，注意下面子目录src的CMakeLists
# link_directories("${PROJECT_SOURCE_DIR}/lib")
# target_link_libraries(hello1 hellolib)

set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -Wall -fexec-charset=GBK")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -Wall -g2 -gdb")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -Wall")
link_libraries(ws2_32)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/Debug)
add_executable(${PROJECT_NAME}_main "src/Struct Patterns/Composite.cpp")
# add_executable(${PROJECT_NAME}_Filter "src/Struct Patterns/Filter.cpp")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
