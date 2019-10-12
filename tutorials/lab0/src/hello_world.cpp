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
#include "sysc/kernel/sc_spawn_options.h"
#include "sysc/kernel/sc_spawn.h"

Hello_world::Hello_world(sc_core::sc_module_name const& s): sc_module{s}
{
  sc_core::sc_spawn_options m_spawn_opt;
  m_spawn_opt.dont_initialize();
  m_spawn_opt.spawn_method();
  m_spawn_opt.set_sensitivity(&reset_n.negedge_event());
  m_spawn_opt.set_sensitivity(&clk.posedge_event());

  auto lmd1 = [this]() -> void {
    this->incr_counter();
  };

  sc_core::sc_spawn(lmd1, "SP1", &m_spawn_opt);
}

void Hello_world::say_hello() const
{
  std::cout << sc_core::sc_time_stamp() << " Hello World\n";
}

void Hello_world::incr_counter()
{
  using namespace std;
  using namespace sc_core;
  if (reset_n)
    cout << sc_time_stamp() << " counter before incrementing: " << counter;
  if (!reset_n)
    counter = 0;
  else
    counter = counter + 1;

  if (reset_n)
    cout << " counter after incrementing: " << counter << "\n";
  if (counter == 16 && reset_n) {
    say_hello();
    counter = 0;
  }
}

void Hello_world::before_end_of_elaboration()
{
  using namespace sc_core;
  std::cout << sc_time_stamp() << " CB: before_end_of_elab" << std::endl;
}

void Hello_world::end_of_elaboration()
{
  using namespace sc_core;
  std::cout << sc_time_stamp() << " CB: end_of_elab" << std::endl;
}

void Hello_world::start_of_simulation()
{
  using namespace sc_core;
  std::cout << sc_time_stamp() << " CB: start_of_sim" << std::endl;
}

void Hello_world::end_of_simulation()
{
  using namespace sc_core;
  std::cout << sc_time_stamp() << " CB: end_of_sim" << std::endl;
}
