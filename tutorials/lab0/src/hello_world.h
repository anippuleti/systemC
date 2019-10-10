#ifndef LAB0_HELLO_WORLD_H
#define LAB0_HELLO_WORLD_H

#include "systemc"

// Hello_world is module name
class Hello_world : public sc_core::sc_module {
 public:

  Hello_world(sc_core::sc_module_name const& s);

  sc_core::sc_signal<bool>       clk;
  sc_core::sc_signal<bool>      reset_n;
  sc_core::sc_signal<unsigned> counter;

 private:
  void incr_counter();
  void say_hello() const;
};

#endif //LAB0_HELLO_WORLD_H
