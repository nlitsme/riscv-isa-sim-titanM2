uint32_t x = RS1;
uint32_t y = 0;
y |= (x&0xff)<<24;
y |= (x&0xff00)<<8;
y |= (x&0xff0000)>>8;
y |= (x&0xff000000)>>24;
WRITE_RD(y);
