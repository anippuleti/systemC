/// All systemc modules should include systemc.h header file
#include "systemc.h"

// Hello_world is module name
class hello_world : public sc_module {
 public:

  hello_world(sc_core::sc_module_name const& s): sc_module{s} { }

  void say_hello() {
    //Print "Hello World" to the console.
    cout << "Hello World.\n";
  }
};

// sc_main in top level function like in C++ main
int sc_main(int argc, char* argv[]) {
  hello_world hello("HELLO");
  // Print the hello world
  hello.say_hello();
  return(0);
}
