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

#ifndef LAB0_HELLO_WORLD_H
#define LAB0_HELLO_WORLD_H

#include "systemc"

// Hello_world is module name
class Hello_world : public sc_core::sc_module {
 public:

  Hello_world(sc_core::sc_module_name const& s);

  sc_core::sc_signal<bool>       clk;
  sc_core::sc_signal<bool>      reset_n;
  sc_core::sc_signal<unsigned> counter;

  //SystemC callbacks
  void before_end_of_elaboration() final;
  void end_of_elaboration() final;
  void start_of_simulation() final;
  void end_of_simulation() final;

 private:
  void incr_counter();
  void say_hello() const;
};

#endif //LAB0_HELLO_WORLD_H
