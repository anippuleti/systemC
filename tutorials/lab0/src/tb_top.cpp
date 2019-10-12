//////////////////////////////////////////////////////////////////////////////
//The MIT License
//
//Copyright (c) 2010-2019 Google, Inc. http://angularjs.org
//
//Permission is hereby granted, free of charge, to any person obtaining
//a copy of this software and associated documentation files
//(the "Software"), to deal in the Software without restriction, including
//without limitation the rights to use, copy, modify, merge, publish,
//distribute, sublicense, and/or sell copies of the Software, and to
//permit persons to whom the Software is furnished to do so, subject to the 
//following conditions:
//
//The above copyright notice and this permission notice shall be included
//in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//////////////////////////////////////////////////////////////////////////////

#include "hello_world.h"

template<typename T>
void log(T&& val)
{
  std::cout << sc_core::sc_time_stamp() << " " << std::forward<T>(val) << std::endl;
}

void set_reset_pulse(sc_core::sc_signal<bool>& rst)
{
  using namespace sc_core;
  rst = true;
  std::cout << sc_time_stamp() << " TB: Reset pulse started" << std::endl;
  sc_start(7, sc_core::sc_time_unit::SC_NS);
  rst = false;
  sc_start(7, sc_core::sc_time_unit::SC_NS);
  rst = true;
  sc_start(16, sc_core::sc_time_unit::SC_NS);
  std::cout << sc_time_stamp() << " TB: Reset pulse completed" << std::endl;
}

void incr_clk_n(unsigned cycles, sc_core::sc_signal<bool>& clk)
{
  using namespace sc_core;
  clk = false;
  std::cout << sc_time_stamp() << " TB: Clock trigger begin" << std::endl;
  for (unsigned i = 0; i < cycles; ++i) {
    sc_start(5, sc_core::sc_time_unit::SC_NS);
    clk = true;
    sc_start(5, sc_core::sc_time_unit::SC_NS);
    clk = false;
  }
  std::cout << sc_time_stamp() << " TB: Clock trigger end" << std::endl;
}

//// sc_main is the entry point of application from SystemC kernal
int sc_main(int argc, char* argv[])
{
  Hello_world hello("HELLO");
  std::cout << "\n\n";

  log("TB: Begin simulation");
  sc_core::sc_start(0, sc_core::sc_time_unit::SC_NS);
  set_reset_pulse(hello.reset_n);
  incr_clk_n(36, hello.clk);
  set_reset_pulse(hello.reset_n);
  incr_clk_n(38, hello.clk);
  log("TB: End simulation");
  sc_core::sc_stop();

  return 0;
}
