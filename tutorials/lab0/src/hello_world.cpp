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

Hello_world::Hello_world(sc_core::sc_module_name const& s, unsigned vlevel):
  sc_module{s},
  verbosity{vlevel}
{
  
  sc_core::sc_spawn_options m_spawn_opt;
  //Set flag to not to invoke on initialization
  m_spawn_opt.dont_initialize();
  //Set flag to indicate that the function Object is a SC_METHOD
  m_spawn_opt.spawn_method();
  //Set sensitivity list
  m_spawn_opt.set_sensitivity(&reset_n.negedge_event());
  m_spawn_opt.set_sensitivity(&clk.posedge_event());

  //Lamda
  auto lmd1 = [this]() -> void {
    this->incr_counter();
  };

  sc_core::sc_spawn(lmd1, "SP1", &m_spawn_opt);
}

Hello_world::~Hello_world() = default;

void Hello_world::say_hello() const
{
  std::cout << sc_core::sc_time_stamp() << " Hello World!\n";
}

void Hello_world::incr_counter()
{
  using namespace std;
  using namespace sc_core;
  if (reset_n && verbosity > 2)
    cout << sc_time_stamp() << " counter before incrementing: " << counter;
  if (!reset_n)
    counter = 0;
  else
    counter = counter + 1;

  //NOTE: Incremented counter value is observed only in updated phase
  //Incrementation happens in evaluation phase but assignment happens
  //in update phase
  //If ran with verbosity level > 2 then below print indicates that the
  //value printed is equal to prior value before incrementing.
  if (reset_n && verbosity > 2)
    cout << " counter after incrementing: " << counter << '\n';
  if (counter == 16 && reset_n) {
    say_hello();
    counter = 0;
  }
}

void Hello_world::before_end_of_elaboration()
{
  using namespace sc_core;
  if (verbosity > 1)
    std::cout << sc_time_stamp() << " CB: before_end_of_elab" << '\n';
}

void Hello_world::end_of_elaboration()
{
  using namespace sc_core;
  if (verbosity > 1)
    std::cout << sc_time_stamp() << " CB: end_of_elab" << '\n';
}

void Hello_world::start_of_simulation()
{
  using namespace sc_core;
  if (verbosity > 1)
    std::cout << sc_time_stamp() << " CB: start_of_sim" << '\n';
}

void Hello_world::end_of_simulation()
{
  using namespace sc_core;
  if (verbosity > 1)
    std::cout << sc_time_stamp() << " CB: end_of_sim" << '\n';
}

