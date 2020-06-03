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

#ifndef LAB2_EVENT_HANDLER_H
#define LAB2_EVENT_HANDLER_H

#include "systemc"
#include "sysc/kernel/sc_spawn_options.h"
#include "sysc/kernel/sc_spawn.h"
#include <vector>
#include <algorithm>

class Event_handler : public sc_core::sc_module {
 public:
  Event_handler(sc_core::sc_module_name const&s, unsigned nagents);
  ~Event_handler() noexcept override;

  //void before_end_of_elaboration() override;
  void start_of_simulation() override;

 private:
  struct Event_info {
    enum class Pulse { SET0, SET1 };
    Pulse trigger_pulse{Pulse::SET0};
    Pulse done_pulse{Pulse::SET0};

    void set()   { trigger_pulse = Pulse::SET1; }
    void reset() { trigger_pulse = Pulse::SET0; }
    void done()  { done_pulse    = Pulse::SET1; }
    bool operator()() const
    {
      return done_pulse == Pulse::SET1 || trigger_pulse == Pulse::SET1;
    }
  };

  unsigned num_agents;
  std::vector<Event_info> agent_info;
  sc_core::sc_event       event_trigger;
  void wait4clk_cycles(unsigned n = 1);
  void set_event_trigger();
  void set_event_trig_methods();
  void set_event_trig_thread();

 public:
  //using declaratives
  using Itr  = std::vector<Event_handler::Event_info>::iterator;
  using CItr = std::vector<Event_handler::Event_info>::const_iterator;
  //Port list
  sc_core::sc_in<bool>              clk;
  std::vector<sc_core::sc_in<bool>> event_inq;
  std::vector<sc_core::sc_in<bool>> agent_doneq;
  sc_core::sc_out<bool>             event_out;

  Itr  begin();
  Itr  end();
  CItr cbegin() const;
  CItr cend() const;
  bool all_agents_triggered() const;
}; //end of class

//using declaratives
using Itr  = Event_handler::Itr;
using CItr = Event_handler::CItr;

Event_handler::Event_handler(const sc_core::sc_module_name &s, unsigned int nagents):
    sc_core::sc_module(s),
    num_agents{nagents},
    agent_info(nagents),
    clk(),
    event_inq(nagents),
    agent_doneq(nagents),
    event_out()
{
}

Event_handler::~Event_handler() noexcept = default;

bool Event_handler::all_agents_triggered() const
{
  return std::all_of(
      this->cbegin(),
      this->cend(),
      [](Event_info const& e) { return e(); });
}

void Event_handler::wait4clk_cycles(unsigned n)
{
  for (unsigned i = 0; i < n; ++i)
    wait(clk.posedge_event());
}

void Event_handler::set_event_trigger()
{
  event_trigger.notify(sc_core::SC_ZERO_TIME);
}

void Event_handler::set_event_trig_methods()
{
  for (std::size_t idx = 0; idx < num_agents; ++idx) {
    {
      std::string s{"event_triggered_"};
      s+= std::to_string(idx);
      sc_core::sc_spawn_options opt;
      opt.dont_initialize();
      opt.spawn_method();
      opt.set_sensitivity(&event_inq[idx].posedge_event());
      auto lmd = [this, idx]() -> void {
        std::next(this->begin(), idx)->set();
        if (this->all_agents_triggered())
          this->set_event_trigger();
      };
      sc_spawn(lmd, s.c_str(), &opt);
    }

    {
      std::string s{"done_triggered_"};
      s+= std::to_string(idx);
      sc_core::sc_spawn_options opt;
      opt.dont_initialize();
      opt.spawn_method();
      opt.set_sensitivity(&agent_doneq[idx].posedge_event());
      auto lbd = [this, idx]() -> void {
        std::next(this->begin(), idx)->done();
        if (this->all_agents_triggered())
          this->set_event_trigger();
      };
      sc_spawn(lbd, s.c_str(), &opt);
    }
  }
}

void Event_handler::set_event_trig_thread()
{
  sc_core::sc_spawn_options opt;
  auto lmd = [this]() -> void {
    while (!this->all_agents_triggered()) {
      wait(this->event_trigger);
      std::cout << "Event Handler: Assert event_out pulse at time: "
                << sc_core::sc_time_stamp() << std::endl;
      event_out.write(true);
      wait4clk_cycles(4);
      std::cout << "Event Handler: De-assert event_out pulse at time: "
                << sc_core::sc_time_stamp() << std::endl;
      event_out.write(false);
      std::for_each(this->begin(), this->end(),
                    [](Event_info& e) { e.reset(); }
      );
    }
  };
  sc_core::sc_spawn(lmd, "Event_handler_thread", &opt);
}

void Event_handler::start_of_simulation()
{
  set_event_trig_methods();
  set_event_trig_thread();
}

Itr  Event_handler::begin() { return agent_info.begin(); }
Itr  Event_handler::end()   { return agent_info.end();   }
CItr Event_handler::cbegin() const { return agent_info.cbegin(); }
CItr Event_handler::cend() const { return agent_info.cend(); }

#endif //LAB2_EVENT_HANDLER_H
