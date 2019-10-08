
This lab is to test the intallation of SystemC and run a basic hello world example. 
We will also go over some basic build blocks for SystemC programs like 
sc_module, sc_module_name and sensitivity list

SystemC reference manul can be downloaded from: https://ieeexplore.ieee.org/document/6134619

Steps to install SystemC (version: 2.3.3) on linux machines
  Download SystemC from https://accellera.org/downloads/standards/systemc
  Extract the tar file and cd to systemc-2.3.3
  Using CMake build scripts is easier. 
  Below are the steps I followed but please refer to cmake/INSTALL_USING_CMAKE file for details
    For bash shell
      export SYSTEMC_HOME=<installation path>
    For csh shell
      steenv SYSTEMC_HOME <installation path>
    mkdir build
    cd build
    cmake -DBUILD_SOURCE_DOCUMENTATION=ON -DCMAKE_CXX_STANDARD=14 -DCMAKE_VERBOSE_MAKEFILE=ON ..
    make -j
    make -j check
    make -j install

  If above cmake command fails dueto missing doxygen installation,
  Then either install it or get rid of -DBUILD_SOURCE_DOCUMENTATION=ON
  
  Either you can add systemC shared library path to LD_LIBRARY_PATH so that linker is able to locate the shared
  library while executing or adding rpath to the executable when linking the binary. 
  It adds an entry to the binary and hints the loader to search additional path. I followed the second approach

