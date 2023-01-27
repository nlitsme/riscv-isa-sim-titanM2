#pragma once

#include "abstract_device.h"

// timer

class pixel4062_device_t : public abstract_device_t {
 private:
  std::vector<uint8_t> _data;
 public:
  pixel4062_device_t(/*std::string name*/)
      : _data(256)
  {
      setdw(0x0004, 0);
      setdw(0x0010, 0);
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
    if (len==4 && ofs==0x0010) {
        uint32_t val = *(uint32_t*)bytes;
        if ((val&9)==9) {
            val = 0;
        }
        setdw(ofs, val);
        return true;
    }
    else if (len==4 && ofs==0x0000) {
        uint32_t val = *(uint32_t*)bytes;
        if (val)
            setdw(0x0004, 1);
        else
            setdw(0x0004, 0);
    }

    memcpy(&_data[ofs], bytes, len);
    return true;
  }
};

