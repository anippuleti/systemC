Steps to install SystemC (version: 2.3.3) on linux machines
 1. Download SystemC from https://accellera.org/downloads/standards/systemc
 2. Extract the tar file and cd to systemc-2.3.3
 3. Using CMake build scripts is easier.
 4. Below are the steps I followed but please refer to cmake/INSTALL_USING_CMAKE file for details
 5. For bash shell: export SYSTEMC_HOME=<installation path>
 6. For csh shell: setenv SYSTEMC_HOME <installation path>

  For RELEASE version of shared library

    mkdir build
    cd build
    cmake DCMAKE_BUILD_TYPE=Release -DBUILD_SOURCE_DOCUMENTATION=ON -DCMAKE_CXX_STANDARD=14 -DCMAKE_VERBOSE_MAKEFILE=ON -DINSTALL_TO_LIB_BUILD_TYPE_DIR=ON ..
    make -j
    make -j check
    make -j install

  If above cmake command fails dueto missing doxygen installation, Then either install it or get rid of -DBUILD_SOURCE_DOCUMENTATION=ON

  For DEBUG version of shared library

    cmake DCMAKE_BUILD_TYPE=Release -DBUILD_SOURCE_DOCUMENTATION=ON -DCMAKE_CXX_STANDARD=14 -DCMAKE_VERBOSE_MAKEFILE=ON -DINSTALL_TO_LIB_BUILD_TYPE_DIR=ON ..
    make -j
    make -j check
    make -j install

Either you can add systemC shared library path to LD_LIBRARY_PATH so that linker is able to locate the shared library or add it to gcc commandline with switch "rpath". It adds an entry to the binary and hints the loader to search additional path. I followed the second approach, please refer to makefile for exact command

To build and execute lab0: make run

Description of the Program
1. Implement a SystemC program that displays "Hello World" when-ever 4-bit counter wraps. The module runs on positive edge triggered clock with asynchronous negetive edge trigger capability. 
2. Goal of this lab is to explain the process of installation of SystemC and Elaboration and Simulation semantics of the SystemC-Kernal.

