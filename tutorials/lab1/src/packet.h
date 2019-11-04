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
#ifndef PACKET_H
#define PACKET_H

enum class Cmd {Read, Write};

struct Packet {
  inline Packet() = default;
  inline Packet(u_int64_t a, u_int32_t i, Cmd c):
      addr{a},
      id{i},
      cmd{c}
  { }

  friend std::ostream& operator<<(std::ostream&, Packet const& );

  u_int64_t addr{0};
  u_int32_t  id{0};
  Cmd       cmd{Cmd::Read};
};

std::ostream& operator<<(std::ostream& os, Packet const& pkt)
{
  std::string s =  pkt.cmd == Cmd::Read ? " Cmd: Read" : " Cmd: Write";
  return os << sc_core::sc_time_stamp() << " addr: " << pkt.addr
            << " id: " << pkt.id << s << '\n';
}

#endif //PACKET_H
