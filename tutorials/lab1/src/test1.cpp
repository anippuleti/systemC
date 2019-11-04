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
#include "fifo.h"
#include "packet.h"
#include "sysc/kernel/sc_spawn_options.h"
#include "sysc/kernel/sc_spawn.h"

struct Test1 {
  Fifo<Packet, 8> m_fifo{"t1_event_fifo"};

  void run()
  {
    unsigned const MXSZ = 8;
    set_full_spawn();
    set_empty_spawn();
    sc_core::sc_start(0, sc_core::sc_time_unit::SC_NS);
    m_fifo.trigger_reset();
    for (auto i = 0; i < MXSZ; ++i) {
      sc_core::sc_start(5, sc_core::sc_time_unit::SC_NS);
      auto p = Packet{
          static_cast<u_int64_t>(64 * i),
          static_cast<u_int32_t>(0), Cmd::Write
      };
      std::cout << "Push: " << p;
      m_fifo.push_element(std::move(p));
    }
    if (m_fifo.get_size() != MXSZ)
      throw std::runtime_error{"ERROR: Fifo depth != MXSZ"};

    for (auto i = 0; i < MXSZ; ++i) {
      sc_core::sc_start(5, sc_core::sc_time_unit::SC_NS);
      if (m_fifo.peek_element().addr != 64 * i)
        throw std::runtime_error{"ERROR: Fifo order mismatch"};
      std::cout << "Pop: " << m_fifo.get_element();
    }
    if (m_fifo.get_size() != 0)
      throw std::runtime_error{"ERROR: Fifo depth != 0"};

    sc_core::sc_start(5, sc_core::sc_time_unit::SC_NS);
  }

  void set_full_spawn()
  {
    sc_core::sc_spawn_options opts;
    opts.dont_initialize();
    opts.spawn_method();
    opts.set_sensitivity(&m_fifo.get_full_e());

    auto lmd = []() -> void {
      std::cout << "FIFO FULL: " << sc_core::sc_time_stamp() << '\n';
    };
    sc_core::sc_spawn(lmd, "sp1", &opts);

    sc_core::sc_spawn_options opts_al;
    opts_al.dont_initialize();
    opts_al.spawn_method();
    opts_al.set_sensitivity(&m_fifo.get_almost_full_e());

    auto lmd1 = []() -> void {
      std::cout << "FIFO ALMOST FULL: " << sc_core::sc_time_stamp() << '\n';
    };
    sc_core::sc_spawn(lmd1, "sp2", &opts_al);
  }

  void set_empty_spawn()
  {
    sc_core::sc_spawn_options opts;
    opts.dont_initialize();
    opts.spawn_method();
    opts.set_sensitivity(&m_fifo.get_empty_e());

    auto lmd = []() -> void {
      std::cout << "FIFO EMPTY: " << sc_core::sc_time_stamp() << '\n';
    };
    sc_core::sc_spawn(lmd, "sp3", &opts);

    sc_core::sc_spawn_options opts_al;
    opts_al.dont_initialize();
    opts_al.spawn_method();
    opts_al.set_sensitivity(&m_fifo.get_almost_empty_e());

    auto lmd1 = []() -> void {
      std::cout << "FIFO ALMOST EMPTY: " << sc_core::sc_time_stamp() << '\n';
    };
    sc_core::sc_spawn(lmd1, "sp4", &opts_al);
  }
};


int sc_main(int argc, char* argv[])
{
  Test1 t1;

  try {
    t1.run();
  } catch (std::exception const& c) {
    std::cout << "ERROR: " << sc_core::sc_time_stamp()
              << " " << c.what() << '\n';
    return 1;
  }

  return 0;
}
