SRC_DIR = .
INC_DIR = .

SRC = $(shell find $(SRC_DIR) -name '*.c')
OBJ = ${SRC:.c=.o cpu/interrupt.o} # file extension replacement

# Change this if your cross-compiler is somewhere else
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -g -I $(INC_DIR)

QEMU = /usr/bin/qemu-system-i386
QEMU_RUN_FLAGS = -fda os-image.bin -device piix3-ide,id=ide -drive id=disk,file=image.img,format=raw,if=none -device ide-hd,drive=disk,bus=ide.0
QEMU_DEBUG_FLAGS = -s -S

# First rule is run by default
os-image.bin: boot/boot_sector.bin kernel.bin
	cat $^ > os-image.bin

# '--oformat binary' deletes all symbols as a collateral
# don't need to 'strip' them manually
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	$(QEMU) $(QEMU_RUN_FLAGS)

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	$(QEMU) $(QEMU_RUN_FLAGS) $(QEMU_DEBUG_FLAGS) &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
%.o: %.c
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf **/*.bin **/*.o **/*.elf os-image.bin
