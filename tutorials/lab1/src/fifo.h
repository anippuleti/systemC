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

#ifndef LAB1_FIFO_H
#define LAB1_FIFO_H

#include <vector>
#include "systemc"

template<typename T, std::size_t V>
class Fifo : public sc_core::sc_module {
  static_assert(V > 1, "FIFO depth must be greater than 1");
  static_assert(std::is_default_constructible<T>::value,
      "Type T must be default constructable"
  );
 public:
  explicit Fifo(sc_core::sc_module_name const& s);
  ~Fifo() final;

  //API to get reference handle to FIFO related output events.
  //Note: Client code must not make a copy
  sc_core::sc_event const& get_almost_full_e() const;
  sc_core::sc_event const& get_full_e() const;
  sc_core::sc_event const& get_almost_empty_e() const;
  sc_core::sc_event const& get_empty_e() const;

  //Trigger HW reset
  void trigger_reset();
  //API to push (write) transactions into FIFO. Only rvalue method is provided
  //FIFO micro-architecture does not allow client to have access to element
  void push_element(T&& p);
  //API to pop out transactions from FIFO. Method returns the element
  T get_element();

  //Back door access to peek into first element of FIFO that will be poped out next
  T const& peek_element() const;
  //Back door access to size of the Queue
  std::size_t get_size() const;

 private:
  std::vector<T> fifo;
  sc_core::sc_event almost_full;
  sc_core::sc_event full;
  sc_core::sc_event almost_empty;
  sc_core::sc_event empty;

  void trigger_almost_full() noexcept;
  void trigger_full() noexcept;
  void trigger_almost_empty() noexcept;
  void trigger_empty() noexcept;
};

template<typename T, std::size_t V>
Fifo<T, V>::Fifo(sc_core::sc_module_name const& s):
  sc_core::sc_module(s) { }

template<typename T, std::size_t V>
Fifo<T, V>::~Fifo() = default;

template<typename T, std::size_t V>
sc_core::sc_event const& Fifo<T, V>::get_almost_full_e() const
{
  return almost_full;
}

template<typename T, std::size_t V>
sc_core::sc_event const& Fifo<T, V>::get_full_e() const
{
  return full;
}

template<typename T, std::size_t V>
sc_core::sc_event const& Fifo<T, V>::get_almost_empty_e() const
{
  return almost_empty;
}

template<typename T, std::size_t V>
sc_core::sc_event const& Fifo<T, V>::get_empty_e() const
{
  return empty;
}

template<typename T, std::size_t V>
void Fifo<T,V>::trigger_reset()
{
  fifo.erase(fifo.cbegin(), fifo.cend());
  trigger_empty();
}

template<typename T, std::size_t V>
void Fifo<T,V>::push_element(T&& p)
{
  if (fifo.size() == V)
    throw std::runtime_error{"Overflow triggered since Fifo is already full"};

  fifo.push_back(std::move(p));

  if (fifo.size() == V - 1)
    trigger_almost_full();
  else if (fifo.size() == V)
    trigger_full();
}

template<typename T, std::size_t V>
T Fifo<T, V>::get_element()
{
  if (fifo.size() == 0)
    throw std::runtime_error{"Underflow triggered since Fifo is already empty"};

  auto p = fifo.front();
  fifo.erase(fifo.cbegin());
  if (fifo.size() == 0)
    trigger_empty();
  else if (fifo.size() == 1)
    trigger_almost_empty();

  return p;
}

template <typename T, std::size_t V>
T const& Fifo<T,V>::peek_element() const
{
  return fifo.front();
}

template <typename T, std::size_t V>
std::size_t Fifo<T,V>::get_size() const
{
  return fifo.size();
}

template<typename T, std::size_t V>
void Fifo<T,V>::trigger_almost_full() noexcept
{
  almost_full.notify();
}

template<typename T, std::size_t V>
void Fifo<T,V>::trigger_full() noexcept
{
  full.notify();
}

template<typename T, std::size_t V>
void Fifo<T,V>::trigger_almost_empty() noexcept
{
  almost_empty.notify();
}

template<typename T, std::size_t V>
void Fifo<T,V>::trigger_empty() noexcept
{
  empty.notify();
}

#endif //LAB1_FIFO_H
