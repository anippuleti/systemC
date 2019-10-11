###################
#MakeFile
###################

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
