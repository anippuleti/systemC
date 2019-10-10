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
  if (!reset_n)
    counter = 0;
  else
    counter = counter + 1;

  if (counter == 15 && reset_n) {
    say_hello();
    counter = 0;
  }
}
