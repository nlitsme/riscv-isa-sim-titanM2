#pragma once

#include "abstract_device.h"

// timer

class pixel4063_device_t : public abstract_device_t {
 private:
  std::vector<uint8_t> _data;
  uint64_t _usec;
 public:
  pixel4063_device_t(/*std::string name*/)
      : _data(256), _usec(0)
  {
      setdw(0x0014, 0);
      setdw(0x001c, 0);
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

    // increase usecs
    _usec += 15;
    setdw(0x14, _usec);
    setdw(0x1c, _usec>>32);

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

