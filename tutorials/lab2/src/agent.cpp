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

#include "agent.h"
#include "sysc/kernel/sc_spawn_options.h"
#include "sysc/kernel/sc_spawn.h"
#include <iostream>

namespace detail {
class Random_generator {
  std::seed_seq seed_seq;
  std::mt19937 gen;
  std::uniform_int_distribution<unsigned> e_dist;
  std::uniform_int_distribution<unsigned> t_dist;
 public:

  Random_generator(int e_min, int e_max, int t_min, int t_max):
    seed_seq{5,4,3,2,1},
    e_dist(e_min, e_max),
    t_dist(t_min, t_max)
  {
    std::vector<uint32_t> seeds(1);
    seed_seq.generate(seeds.begin(), seeds.end());
    std::cout << "Seed selected: " << seeds[0] << std::endl;
    gen.seed(seeds[0]);
  }

  unsigned rand_max_events() { return e_dist(gen); }
  unsigned rand_wait_times() { return t_dist(gen); }
};

} //end of namespace detail

std::unique_ptr<detail::Random_generator> Agent::r_gen =
    std::make_unique<detail::Random_generator>(100, 300, 50, 500);

Agent::Agent(const sc_core::sc_module_name &s):
    sc_core::sc_module(s),
    max_events{0}
{
    max_events = r_gen->rand_max_events();
    std::cout << "Max events for instance "
              << s << ": " << max_events << std::endl;
}

Agent::~Agent() noexcept = default;


void Agent::start_of_simulation()
{
  std::string s{name()};
  s.append("-agent-thread");
  sc_core::sc_spawn_options opts;
  auto lmd = [this]() -> void {
    for (std::size_t e = 0; e < max_events; ++e) {
      double wt_time = 1.0;
      wt_time *= r_gen->rand_wait_times();
      wait(wt_time, sc_core::sc_time_unit::SC_NS);
      std::cout << "instance " << name() << " triggered " << e
                << " event at time: " << sc_core::sc_time_stamp() <<std::endl;
      event_out.write(true);
      wait(event_in.posedge_event());
      wait(clk.posedge_event());
      event_out.write(false);
    }
    //End of the module
    std::cout << "instance " << name()
              << " completed processing all events at time "
              << sc_core::sc_time_stamp() <<std::endl;
    agent_done.write(true);
  };

  sc_core::sc_spawn(lmd, s.c_str(), &opts);
}
