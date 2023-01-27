#pragma once

#include "abstract_device.h"
// some kind of mmu type device
class pixel4011_device_t : public abstract_device_t {
 private:
  std::vector<uint8_t> _data;
  int _state_008;
  int _state_0044;
 public:
  pixel4011_device_t(/*std::string name*/)
      : _data(1024), _state_008(-1), _state_0044(0)
  {
      setdw(0x0008, 0);
      setdw(0x0014, 0);
      setdw(0x0018, 0);
      setdw(0x001c, 0);
      setdw(0x009c, 0);
  }
  void setdw(int ofs, uint32_t val)
  {
      *(uint32_t*)&_data[ofs] = val;
  }
  uint32_t getdw(int ofs)
  {
      return *(uint32_t*)&_data[ofs];
  }

  bool load(reg_t ofs, size_t len, uint8_t* bytes) override
  {
    if (ofs + len > _data.size())
      return false;
    if (ofs==0x0004 || ofs==0x0008) {
        uint32_t val = getdw(ofs);
        if (val!=0xe89d48b7) {
            switch(_state_008) {
                case -1:
                    _state_008 = 2;
                    break;
                case 0:
                    _state_008 = -1;
                    setdw(ofs, 0);
                    break;
                default:
                    _state_008--;
            }
        }
    }
    memcpy(bytes, &_data[ofs], len);
    return true;
  }
  bool store(reg_t ofs, size_t len, const uint8_t* bytes) override
  {
    if (ofs + len > _data.size())
      return false;
    if (ofs==0x0010 && len==4) {
        uint32_t val = *(uint32_t*)bytes;
        if (val==0x00fb0043)
        {
            // 'flash' erase
            setdw(0x0017b000, 0xFFFFFFFF);
            setdw(0x0017b004, 0xFFFFFFFF);
            _state_0044 = 0;
        }
        else if (val==0x007b0044)
        {
            // 'flash' ?
            if (_state_0044==0) {
                setdw(0x0017b000, 0x73614aff);
                _state_0044 = 1;
            }
            else {
                setdw(0x0017b000, 0x73614af0);
            }
            setdw(0x0017b004, 1);
        }
        else if (val==0x00f98003)
        {
            // 'flash' erase
            setdw(0x000f9800, 0xFFFFFFFF);
            setdw(0x000f9804, 0xFFFFFFFF);
            _state_0044 = 0;
        }
        else if (val==0x00798004)
        {
            // 'flash' ?
            if (_state_0044==0) {
                setdw(0x000f9800, 0x73614aff);
                _state_0044 = 1;
            }
            else {
                setdw(0x000f9800, 0x73614af0);
            }
            setdw(0x000f9804, 1);
        }
    }

    memcpy(&_data[ofs], bytes, len);

    return true;
  }
};



