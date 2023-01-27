#pragma once

#include "abstract_device.h"

// bootrom

class pixelrom_device_t : public abstract_device_t {
 private:
  std::vector<uint8_t> _data;
 public:
  pixelrom_device_t(/*std::string name*/)
      : _data(0x4000)
  {
      setdw(0x1000, 0x00000297); // auipc      t0, pc, #0___
      setdw(0x1004, 0x00028593); // mv         a1, t0
      setdw(0x1008, 0xf1402573); // csrr       a0, mhartid
      setdw(0x100c, 0x0182a283); // lw         t0, [t0, #+18]
      setdw(0x1010, 0x00030137); // lui        sp, 0x30
      setdw(0x1014, 0x00028067); // jalr       zero, t0, #+0
      setdw(0x1018, 0x000a043c);

      // map boot fns to 'ret' at 0x11xx
      setdw(0x004c, 0x0000114c);
      setdw(0x0050, 0x00001150);
      setdw(0x0054, 0x00001154);
      setdw(0x0058, 0x00001158);
      setdw(0x005c, 0x0000115c);

      // 'ret'
      setdw(0x114c, 0x80828082);
      setdw(0x1150, 0x80828082);
      setdw(0x1154, 0x80828082);
      setdw(0x1158, 0x80828082);
      setdw(0x115c, 0x80828082);

      setdw(0x0070, 0x000a256a);
      setdw(0x007c, 0x0000117c);
      setdw(0x009c, 0x0000119c);
      setdw(0x00a0, 0x000011a0);
      setdw(0x0080, 0x00001180);

      // 'ret 0'
      setdw(0x117c, 0x80824501);
      setdw(0x119c, 0x80824501);
      setdw(0x11a0, 0x80824501);
      setdw(0x1180, 0x80824501);
      printf("pixelrom loaded\n");
  }
  void set_start(uint32_t addr)
  {
      setdw(0x1018, addr);
  }
  void setdw(int ofs, uint32_t value)
  {
      *(uint32_t*)&_data[ofs] = value;
  }
  bool load(reg_t ofs, size_t len, uint8_t* bytes) override
  {
    if (ofs + len > _data.size())
      return false;
    printf("bootrom load %08lx\n", ofs);
    memcpy(bytes, &_data[ofs], len);
    return true;
  }
  bool store(reg_t ofs, size_t len, const uint8_t* bytes) override
  {
    return false;
  }
};


