#include "common.h"
#include "devices.h"

rom_device_t::rom_device_t(std::vector<char> data)
  : data(data)
{
}

bool rom_device_t::load(reg_t addr, size_t len, uint8_t* bytes)
{
  if (addr + len > data.size()) {
    printf("rom load %08lx-%08lx failed\n", addr, addr+len);
    return false;
  }
  printf("rom load %08lx-%08lx ok\n", addr, addr+len);
  memcpy(bytes, &data[addr], len);
  return true;
}

bool rom_device_t::store(reg_t UNUSED addr, size_t UNUSED len, const uint8_t UNUSED *bytes)
{
  return false;
}
