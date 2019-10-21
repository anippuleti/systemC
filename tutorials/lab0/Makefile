###################
#MakeFile
###################

VERBOSITY := 0

run: build
	./lab0 $(VERBOSITY)

debug: debugbuild
	./lab0 $(VERBOSITY)

clean:
	rm -Rf lab0
#If c++ does not exist on your program (/usr/bin/c++ does not exist) then change it to g++
build:
	c++ -Wall -std=c++14 -I${SYSTEMC_HOME}/include -L${SYSTEMC_HOME}/lib-release -o lab0 src/hello_world.cpp src/tb_top.cpp -lsystemc -Wl,-rpath,${SYSTEMC_HOME}/lib-release

debugbuild:
	c++ -g3 -Wall  -std=c++14 -I${SYSTEMC_HOME}/include -L${SYSTEMC_HOME}/lib-debug -o lab0 src/hello_world.cpp src/tb_top.cpp -lsystemc -Wl,-rpath,${SYSTEMC_HOME}/lib-debug
