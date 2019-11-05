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
#include "systemc"
#include "sysc/kernel/sc_spawn_options.h"
#include "sysc/kernel/sc_spawn.h"
#include <functional>
#include <random>
#include "fifo.h"
#include "packet.h"
#include "test2_trigger.h"

class Test2 : public sc_core::sc_module {
 public:

  explicit Test2(sc_core::sc_module_name const& s);
  ~Test2() final;


  void end_of_elaboration() final;
  void start_of_simulation() final;
  void end_of_simulation() final;

  void run_simulation();

 private:
  Fifo<Packet, 48> m_fifo;
  std::uniform_int_distribution<unsigned> m_dist;
  std::mt19937 gen;
  static constexpr uint64_t SIMTIME = 10000000; //10 million iterations
  uint64_t push_count{0};
  uint64_t pop_count{0};
  sc_core::sc_event puh_clk;
  sc_core::sc_event pop_clk;
  bool is_empty{true};
  bool is_full{false};
  Ready is_fifo_push_rdy{Ready::DEASSERTED};
  Ready is_fifo_pop_rdy{Ready::DEASSERTED};

  //packet checks
  uint64_t t_push_addr{0};
  uint64_t t_pop_addr{0};

  void set_dut_cbs();
  void set_test_cbs();
  auto gen_clk_periods() -> std::pair<unsigned, unsigned>;
  void push_pkts_into_fifo();
  void pop_pkts_outof_fifo();

};

Test2::Test2(sc_core::sc_module_name const &s):
    sc_core::sc_module(s),
    m_fifo("Fifo"),
    m_dist(1, 16)
{
  gen.seed(19937);
}

Test2::~Test2() = default;

void Test2::run_simulation()
{
  set_dut_cbs();
  set_test_cbs();
  std::pair<unsigned, unsigned> clk_periods;

  //Run for 10 msec
  for (uint64_t i = 0; i < SIMTIME; ++i) {
    if (i % 256 == 0)
      clk_periods = gen_clk_periods();

    if (i % clk_periods.first == 0)
      puh_clk.notify();

    if (i % clk_periods.second == 0)
      pop_clk.notify();

    //All pending immediate events are triggered on invocation of sc_start()
    //Events triggered by invocation of the notify() are processed before incrementing the simulation.
    //Also if multiple events are triggered, order of events processed is un-deterministic .
    sc_core::sc_start(1, sc_core::sc_time_unit::SC_NS);

    if (is_fifo_push_rdy == Ready::ASSERTED) {
      push_pkts_into_fifo();
      ++push_count;
      is_fifo_push_rdy = Ready::DEASSERTED;
    }

    if (is_fifo_pop_rdy == Ready::ASSERTED) {
      pop_pkts_outof_fifo();
      ++pop_count;
      is_fifo_pop_rdy = Ready::DEASSERTED;
    }
  }

  //Run until all Transactions are poped out
  auto lpcnt = 0;
  while (m_fifo.get_size() > 0 ) {
    if (lpcnt % clk_periods.second == 0)
      pop_clk.notify();

    sc_core::sc_start(1, sc_core::sc_time_unit::SC_NS);
    if (is_fifo_pop_rdy == Ready::ASSERTED) {
      pop_pkts_outof_fifo();
      ++pop_count;
      is_fifo_pop_rdy = Ready::DEASSERTED;
    }
    ++lpcnt;
  }
  sc_core::sc_start(1, sc_core::sc_time_unit::SC_NS);
}

void Test2::end_of_elaboration()
{
  using namespace sc_core;
  std::cout << sc_time_stamp() << " End of Elaboration invoked" << "\n";
}
void Test2::start_of_simulation()
{
  using namespace sc_core;
  std::cout << sc_time_stamp() << " Start of Simulation invoked" << "\n";
}

void Test2::end_of_simulation()
{
  using namespace sc_core;
  std::cout << sc_time_stamp() << " End of Simulation invoked" << "\n";
  std::cout << sc_time_stamp() << " Total packets pushed into FIFO: " << push_count << "\n";
  std::cout << sc_time_stamp() << " Total packets poped  from FIFO: " << pop_count  << "\n";
}

void Test2::set_dut_cbs()
{
  using namespace sc_core;
  { //Full
    sc_core::sc_spawn_options opts;
    opts.spawn_method();
    opts.dont_initialize();
    opts.set_sensitivity(&m_fifo.get_full_e());
    auto lmd = [this]() -> void {
      this->is_full = true;
      std::cout <<  sc_time_stamp() << " FIFO Full triggered" << "\n";
    };

    sc_core::sc_spawn(lmd, "Full_lmd1", &opts);
  }

  { //Almost Full
    sc_core::sc_spawn_options opts;
    opts.spawn_method();
    opts.dont_initialize();
    opts.set_sensitivity(&m_fifo.get_almost_full_e());
    auto lmd = [this]() -> void {
      this->is_full = false;
      std::cout << sc_time_stamp() << " FIFO Almost Full triggered" << "\n";
    };

    sc_core::sc_spawn(lmd, "Full_lmd2", &opts);
  }

  { //Empty
    sc_core::sc_spawn_options opts;
    opts.spawn_method();
    opts.dont_initialize();
    opts.set_sensitivity(&m_fifo.get_empty_e());
    auto lmd = [this]() -> void {
      this->is_empty = true;
      std::cout << sc_time_stamp() << " FIFO Empty triggered" << "\n";
    };

    sc_core::sc_spawn(lmd, "Empty_lmd1", &opts);
  }

  { //Almost Empty
    sc_core::sc_spawn_options opts;
    opts.spawn_method();
    opts.dont_initialize();
    opts.set_sensitivity(&m_fifo.get_almost_empty_e());
    auto lmd = [this]() -> void {
      this->is_empty = false;
      std::cout << sc_time_stamp() << " FIFO Almost Empty triggered" << "\n";
    };

    sc_core::sc_spawn(lmd, "Empty_lmd2", &opts);
  }
}

void Test2::set_test_cbs()
{
  { //Push into fifo
    sc_core::sc_spawn_options opts;
    opts.spawn_method();
    opts.dont_initialize();
    opts.set_sensitivity(&puh_clk);
    Produce_trigger pt;
    auto f1 = std::bind(&Produce_trigger::operator(), &pt, std::cref(is_full));
    sc_core::sc_spawn(&is_fifo_push_rdy, f1, "P1", &opts);
  }

  { //Pop out of fifo
    sc_core::sc_spawn_options opts;
    opts.spawn_method();
    opts.dont_initialize();
    opts.set_sensitivity(&pop_clk);
    Consume_trigger ct;
    auto f1 = std::bind(&Consume_trigger::operator(), &ct, std::cref(is_empty));
    sc_core::sc_spawn(&is_fifo_pop_rdy, f1, "P2", &opts);
  }
}

std::pair<unsigned, unsigned> Test2::gen_clk_periods()
{
  return {m_dist(gen), m_dist(gen)};
}

void Test2::push_pkts_into_fifo()
{
  auto p = Packet{t_push_addr, 0, Cmd::Read};
  std::cout << "Push Packet: " << p;
  m_fifo.push_element(std::move(p));
  t_push_addr = t_push_addr + 64;
}

void Test2::pop_pkts_outof_fifo()
{
  auto p = m_fifo.get_element();
  assert(p.addr == t_pop_addr);
  std::cout << "Pop Packet: " << p;
  t_pop_addr = t_pop_addr + 64;
}


////////////////
//sc_main()
////////////////
int sc_main(int argv, char* argc[])
{
  std::cout << "TEST2 Start" << "\n";
  Test2 m_test{sc_core::sc_module_name{"Test2"}};
  std::cout << "Initiating Elaboration Phase" << "\n";
  sc_core::sc_start(0, sc_core::sc_time_unit::SC_NS);
  std::cout << "Starting Test Sequence" << "\n";
  try {
    m_test.run_simulation();
  } catch (std::runtime_error& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "End of Test Sequence" << "\n";
  sc_core::sc_stop();
  std::cout << "Test End" << "\n";
  return 0;
}
