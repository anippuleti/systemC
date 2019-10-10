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
#
#  For RELEASE version of shared library
#
#    mkdir build
#    cd build
#    cmake DCMAKE_BUILD_TYPE=Release -DBUILD_SOURCE_DOCUMENTATION=ON -DCMAKE_CXX_STANDARD=14 -DCMAKE_VERBOSE_MAKEFILE=ON -DINSTALL_TO_LIB_BUILD_TYPE_DIR=ON ..
#    make -j
#    make -j check
#    make -j install
#
#  If above cmake command fails dueto missing doxygen installation,
#  Then either install it or get rid of -DBUILD_SOURCE_DOCUMENTATION=ON
#
#  For DEBUG version of shared library
#
#    cmake DCMAKE_BUILD_TYPE=Release -DBUILD_SOURCE_DOCUMENTATION=ON -DCMAKE_CXX_STANDARD=14 -DCMAKE_VERBOSE_MAKEFILE=ON -DINSTALL_TO_LIB_BUILD_TYPE_DIR=ON ..
#    make -j
#    make -j check
#    make -j install
#
#  Either you can add systemC shared library path to LD_LIBRARY_PATH so that linker is able to locate the shared
#  library while executing or adding rpath to the executable when linking the binary. 
#  It adds an entry to the binary and hints the loader to search additional path. I followed the second approach

run: build
	./sim

debug: debugbuild
	./sim

clean:
	rm -Rf sim

build:
	g++ -Wall -I${SYSTEMC_HOME}/include -L${SYSTEMC_HOME}/lib-release -o sim src/hello_world.cpp src/tb_top.cpp -lsystemc -Wl,-rpath,${SYSTEMC_HOME}/lib-release

debugbuild:
	g++ -g3 -Wall -I${SYSTEMC_HOME}/include -L${SYSTEMC_HOME}/lib-debug -o sim src/hello_world.cpp src/tb_top.cpp -lsystemc -Wl,-rpath,${SYSTEMC_HOME}/lib-debug
