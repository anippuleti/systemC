###################
#MakeFile
###################

CC           := c++
SYSTEMC_HOME := /home/anippuleti/lib/systemc/systemc-2.3.3
CPPFLAGS     := -Wall -std=c++14 -I $(SYSTEMC_HOME)/include
VPATH        := src
LIBS_PATH    := -L$(SYSTEMC_HOME)/lib-release -Wl,-rpath,${SYSTEMC_HOME}/lib-release
VERBOSITY    := 0

DCPPFLAGS    := -g3 -Wall -std=c++14 -I${SYSTEMC_HOME}/include
DLIBS_PATH   := -L$(SYSTEMC_HOME)/lib-debug -Wl,-rpath,${SYSTEMC_HOME}/lib-debug

build: hello_world.cpp tb_top.cpp 
	$(CC) $(CPPFLAGS) $(LIBS_PATH) $^ -lsystemc -o lab0

debug: hello_world.cpp tb_top.cpp 
	$(CC) $(DCPPFLAGS) $(DLIBS_PATH) $^ -lsystemc -o lab0

.PHONY: clean
clean:
	rm -Rf lab0

run: build
	./lab0 $(VERBOSITY)

dbg: debug
	./lab0 $(VERBOSITY)

