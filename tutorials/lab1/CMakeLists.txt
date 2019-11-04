cmake_minimum_required(VERSION 3.15)
project(test1 VERSION 1.0 LANGUAGES CXX DESCRIPTION "SystemC practice")

#Current SystemC library supports C++11 and C++14,
#C++17 is not yet supported. Setting it to C++17 cause linker issue dueto version mis-match
set(CMAKE_CXX_STANDARD 14)

#Checking if environment variable ${SYSTEMC_HOME} is set
if (NOT DEFINED ENV{SYSTEMC_HOME})
    message(FATAL_ERROR "$SYSTEMC_HOME must be set")
endif ()

#Executable: test1, includeing systemc include directory
add_executable(test1 src/test1.cpp)
add_executable(test2 src/test2.cpp)
target_include_directories(test1 PUBLIC $ENV{SYSTEMC_HOME}/include)
target_include_directories(test2 PUBLIC $ENV{SYSTEMC_HOME}/include)

#Using -DCMAKE_BUILD_TYPE option to determine which shared library build to link
#Pick Debug version if cmake build type is Debug. Library in lib-debug
#Pick Release version if cmake build type is Release. Library in lib-release
message(STATUS, "cmake build type ${CMAKE_BUILD_TYPE}")
if (${CMAKE_BUILD_TYPE} EQUAL "Release" )
    target_link_libraries(test1 PUBLIC $ENV{SYSTEMC_HOME}/lib-release/libsystemc.so)
    target_link_libraries(test2 PUBLIC $ENV{SYSTEMC_HOME}/lib-release/libsystemc.so)
else()
    target_link_libraries(test1 PUBLIC $ENV{SYSTEMC_HOME}/lib-debug/libsystemc.so)
    target_link_libraries(test2 PUBLIC $ENV{SYSTEMC_HOME}/lib-debug/libsystemc.so)
endif()
