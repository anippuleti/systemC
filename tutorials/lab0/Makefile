###################
#MakeFile
###################

###################
#Steps to install SystemC (version: 2.3.3) on linux machines
#  Download SystemC from https://accellera.org/downloads/standards/systemc
#  Extract the tar file and cd to systemc-2.3.3
#  Using CMake build scripts is easier. 
#  Below are the steps I followed but please refer to cmake/INSTALL_USING_CMAKE file for details
#    For bash shell
#      export SYSTEMC_HOME=<installation path>
#    For csh shell
#      steenv SYSTEMC_HOME <installation path>
#    mkdir build
#    cd build
#    cmake -DBUILD_SOURCE_DOCUMENTATION=ON -DCMAKE_CXX_STANDARD=14 -DCMAKE_VERBOSE_MAKEFILE=ON ..
#    make -j
#    make -j check
#    make -j install
#
#  If above cmake command fails dueto missing doxygen installation,
#  Then either install it or get rid of -DBUILD_SOURCE_DOCUMENTATION=ON
#  
#  Either you can add systemC shared library path to LD_LIBRARY_PATH so that linker is able to locate the shared
#  library while executing or adding rpath to the executable when linking the binary. 
#  It adds an entry to the binary and hints the loader to search additional path. I followed the second approach

run: clean build
	./sim

clean:
	rm -Rf sim

build: clean
	g++ -Wall -I${SYSTEMC_HOME}/include -L${SYSTEMC_HOME}/lib -o sim src/hello_world.cxx -lsystemc -Wl,-rpath,${SYSTEMC_HOME}/lib

