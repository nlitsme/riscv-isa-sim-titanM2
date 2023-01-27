
# /usr/bin/riscv64-unknown-elf-objcopy -I binary -O elf32-little -B riscv --rename-section .data=.text --adjust-section-vma .data=$[0x80000] --image-base $[0x80000] --set-start $[0xa043c] evt.ec.bin  evt.o

#   --section-start .data=80000 
#   -Tldata-segment 80000 

/usr/bin/riscv64-unknown-elf-ld  \
    -z max-page-size=4096 \
    -A riscv  \
    --section-start .text=80000 \
    -b binary  evt.ec.bin \
    -Ttext-segment 80000 \
    --entry $[0xa043c] \
    --defsym _start=$[0xa043c] \
    --no-dynamic-linker  \
    --oformat elf32-littleriscv -o evt.elf

# pgm headers: at 0x38
#hexedit evt.elf -pd 38:1000,80000,80000,100000,100000

# change program flag to 'R E'
# hexedit evt.elf -pd 4c:5
# hexedit evt.elf -pd 001011f8:7
# hexedit evt.elf -ps 001011c0:text 



#/usr/bin/riscv64-unknown-elf-objcopy --rename-section .data=.text,alloc,code evt.elf  evt2.elf
