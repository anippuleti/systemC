#include "hello_world.h"

template<typename T>
void log(T const& val)
{
  std::cout << sc_core::sc_time_stamp() << " " << val << std::endl;
}

void set_reset_pulse(sc_core::sc_signal<bool>& rst)
{
  rst = true;
  std::cout << sc_core::sc_time_stamp() << " Reset pulse started" << std::endl;
  sc_core::sc_start(7, sc_core::sc_time_unit::SC_NS);
  rst = false;
  sc_core::sc_start(7, sc_core::sc_time_unit::SC_NS);
  rst = true;
  sc_core::sc_start(16, sc_core::sc_time_unit::SC_NS);
  std::cout << sc_core::sc_time_stamp() << " Reset pulse completed" << std::endl;
}

void incr_clk_n(unsigned cycles, sc_core::sc_signal<bool>& clk)
{
  clk = false;
  std::cout << sc_core::sc_time_stamp() << " Clock trigger begin" << std::endl;
  for (unsigned i = 0; i < cycles; ++i) {
    sc_core::sc_start(5, sc_core::sc_time_unit::SC_NS);
    clk = true;
    sc_core::sc_start(5, sc_core::sc_time_unit::SC_NS);
    clk = false;
  }
  std::cout << sc_core::sc_time_stamp() << " Clock trigger end" << std::endl;
}

//// sc_main in top level function like in C++ main
int sc_main(int argc, char* argv[]) {
  Hello_world hello("HELLO");

  std::cout << "\n\n";
  sc_core::sc_start(0, sc_core::sc_time_unit::SC_NS);

  log("Begin simulation");
  set_reset_pulse(hello.reset_n);
  incr_clk_n(32, hello.clk);
  set_reset_pulse(hello.reset_n);
  incr_clk_n(32, hello.clk);
  log("End simulation");

  return(0);
}
