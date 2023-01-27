uint32_t x = RS1;
int i=0;
while (x && (x&1)==0) {
   x >>= 1;
   i ++;
}
WRITE_RD(i);

