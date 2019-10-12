###################
#MakeFile
###################

run: build
	./sim

debug: debugbuild
	./sim

clean:
	rm -Rf sim
#If c++ does not exist on your program (/usr/bin/c++ does not exist) then change it to g++
build:
	c++ -Wall -I${SYSTEMC_HOME}/include -L${SYSTEMC_HOME}/lib-release -o sim src/hello_world.cpp src/tb_top.cpp -lsystemc -Wl,-rpath,${SYSTEMC_HOME}/lib-release

debugbuild:
	c++ -g3 -Wall -I${SYSTEMC_HOME}/include -L${SYSTEMC_HOME}/lib-debug -o sim src/hello_world.cpp src/tb_top.cpp -lsystemc -Wl,-rpath,${SYSTEMC_HOME}/lib-debug
