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

#ifndef LAB2_AGENT_H
#define LAB2_AGENT_H

#include <random>
#include <memory>
#include "systemc"

//fwd declaration
namespace detail {
class Random_generator;
}

class Agent : public sc_core::sc_module {
 public:
  sc_core::sc_in<bool>   clk;
  sc_core::sc_in<bool>   event_in;
  sc_core::sc_out<bool>  event_out;
  sc_core::sc_out<bool>  agent_done;

  explicit Agent(sc_core::sc_module_name const& s);
  ~Agent() noexcept override;

 protected:
  void start_of_simulation() override;

 private:
  unsigned max_events;
  static std::unique_ptr<detail::Random_generator> r_gen;
};

#endif //LAB2_AGENT_H
