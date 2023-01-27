#pragma once

#include "abstract_device.h"
// some kind of mmu type device
class pixel4010_device_t : public abstract_device_t {
 private:
  std::vector<uint8_t> _data;
 public:
  pixel4010_device_t(/*std::string name*/)
      : _data(1024)
  {
      setdw(0x0270, 0x80000);
      setdw(0x0274, 0x20000);
  }
  void setdw(int ofs, uint32_t value)
  {
      *(uint32_t*)&_data[ofs] = value;
  }
  bool load(reg_t ofs, size_t len, uint8_t* bytes) override
  {
    if (ofs + len > _data.size())
      return false;
    memcpy(bytes, &_data[ofs], len);
    return true;
  }
  bool store(reg_t ofs, size_t len, const uint8_t* bytes) override
  {
    if (ofs + len > _data.size())
      return false;
    memcpy(&_data[ofs], bytes, len);
    return true;
  }
};


