cmake_minimum_required(VERSION 3.12)
project(lab2)

#Current SystemC library supports C++11 and C++14,
#C++17 is not yet supported. Setting it to C++17 causes linker issue due-to version mis-match
set(CMAKE_CXX_STANDARD 14)

#Checking if environment variable ${SYSTEMC_HOME} is set
if (NOT DEFINED ENV{SYSTEMC_HOME})
    message(FATAL_ERROR "$SYSTEMC_HOME must be set")
endif ()

#Executable: test1, includeing systemc include directory
add_executable(lab2 src/agent.cpp src/agent.cpp src/agent.cpp src/agent.h src/top.cpp)
target_include_directories(lab2 PUBLIC $ENV{SYSTEMC_HOME}/include)
add_definitions("-Wall -Wextra -Wpedantic -Werror")

#Using -DCMAKE_BUILD_TYPE option to determine which shared library build to link
#Pick Debug version if cmake build type is Debug. Library in lib-debug
#Pick Release version if cmake build type is Release. Library in lib-release
message(STATUS, "cmake build type ${CMAKE_BUILD_TYPE}")
if (${CMAKE_BUILD_TYPE} EQUAL "Release" )
    target_link_libraries(lab2 PUBLIC $ENV{SYSTEMC_HOME}/lib-release/libsystemc.so.2.3.3)
else()
    target_link_libraries(lab2 PUBLIC $ENV{SYSTEMC_HOME}/lib-debug/libsystemc.so.2.3.3)
endif()
