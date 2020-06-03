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

#include "event_handler.h"
#include "agent.h"
#include <iostream>
#include <memory>

class Top : sc_core::sc_module {
 public:
  Top(sc_core::sc_module_name const& s, unsigned nagents);
  ~Top() noexcept override;

 private:
  Event_handler                       i_sys_trigger;
  std::vector<std::unique_ptr<Agent>> i_agentq;

  std::vector<sc_core::sc_signal<bool>> agent_trigq;
  std::vector<sc_core::sc_signal<bool>> agent_doneq;
  sc_core::sc_signal<bool>              handler_trig;
  sc_core::sc_signal<bool>              clk;

  void connect_submodules(unsigned nagents);
  void init_clk_thread();
};

Top::Top(const sc_core::sc_module_name &s, unsigned int nagents):
    sc_core::sc_module(s),
    i_sys_trigger("Event_Trigger", nagents),
    i_agentq(),
    agent_trigq(nagents),
    agent_doneq(nagents),
    handler_trig(),
    clk()
{
  connect_submodules(nagents);
  init_clk_thread();
}

Top::~Top() noexcept = default;

void Top::connect_submodules(unsigned nagents)
{
  auto lmd = [](std::size_t idx) {
    std::string s{"Agent-"};
    s += std::to_string(idx);
    return s;
  };

  for(std::size_t idx = 0; idx < nagents; ++idx) {
    //Construct Agent unique pointer.
    //Since copy constructor of sc_module() is explicitly deleted and move constructor
    //is not explicitly implemented, we need to construct Agents on heap and forward the
    //ownership of the pointer
    i_agentq.emplace_back(std::make_unique<Agent>(lmd(idx).c_str()));
    //Agent connectivity
    i_agentq[idx]->event_out.bind(agent_trigq[idx]);
    i_agentq[idx]->agent_done.bind(agent_doneq[idx]);
    i_agentq[idx]->event_in.bind(handler_trig);
    i_agentq[idx]->clk.bind(clk);

    //Event handler connectivity
    i_sys_trigger.event_inq[idx].bind(agent_trigq[idx]);
    i_sys_trigger.agent_doneq[idx].bind(agent_doneq[idx]);
  }
  //Must be outside of loop. Binding twice to same signals
  //will cause runtime error
  i_sys_trigger.event_out.bind(handler_trig);
  i_sys_trigger.clk.bind(clk);
}

void Top::init_clk_thread()
{
  sc_core::sc_spawn_options opts;
  auto lmd1 = [this]() -> void {
    while (true) {
      clk.write(false);
      wait(5, sc_core::sc_time_unit::SC_NS);
      clk.write(true);
      wait(5, sc_core::sc_time_unit::SC_NS);
    }
  };
  sc_core::sc_spawn(lmd1, "Clock-Thread", &opts);
}

//
// Running basic test for 1 ms
//
void run_simulation()
{
  const sc_core::sc_time TIME_OUT(1, sc_core::sc_time_unit::SC_MS);
  sc_core::sc_time       cur_time(0, sc_core::sc_time_unit::SC_NS);
  while (cur_time < TIME_OUT) {
    sc_core::sc_start(5, sc_core::sc_time_unit::SC_NS);
    cur_time = sc_core::sc_time_stamp();
  }
}

int sc_main(int argc, char** argv)
{
  auto nume = static_cast<unsigned>(*argv[1]) - static_cast<unsigned>('0');
  std::cout << "Number of Agents: " << nume << std::endl;
  if (argc != 2)
    throw std::runtime_error{"ERROR: Must provide number of Agent instances"};
  Top dut("DUT", nume);

  std::cout << "Test Start" << "\n";
  std::cout << "Initiating Elaboration Phase" << "\n";
  sc_core::sc_start(0, sc_core::sc_time_unit::SC_NS);
  run_simulation();
  std::cout << "Test End: " << sc_core::sc_time_stamp() << "\n";
  sc_core::sc_stop();

  return 0;
}
