sources := $(wildcard *.c)
#headers := $(wildcard *.h)

objects := $(sources:.c=.o)
deps    := $(sources:.c=.d)
kernel  := image 
OBJS = \
	main.o\
	assert.o\
	bio.o\
	exec.o\
	file.o\
	fs.o\
	ide.o\
	kalloc.o\
	pipe.o\
	proc.o\
	spinlock.o\
	string.o\
	syscall.o\
	sysfile.o\
	sysproc.o\
	tmu.o\
	exp.o\
	scif.o\
	vm.o\
	trap.o\
	print.o\
	console.o\


# Cross-compiling (e.g., on Mac OS X)
# TOOLPREFIX = i386-jos-elf-

# Using native tools (e.g., on X86 Linux)
TOOLPREFIX =sh-linux-

CC = $(TOOLPREFIX)gcc
AS = $(TOOLPREFIX)gas
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump
CFLAGS = -fno-builtin -Wall -MD -ggdb -nostdinc -I.
CFLAGS += 
ASFLAGS = 
# FreeBSD ld wants ``elf_i386_fbsd''
LDFLAGS += 
LIB = /opt/shtools/lib/gcc/sh4-linux/3.4.6/libgcc.a

image: kernel.S swtch.S initcode call.S $(OBJS) $(LIB) fs.img
	$(CC) $(CFLAGS) -c kernel.S
	$(CC) $(CFLAGS) -c swtch.S
	$(CC) $(CFLAGS) -c call.S
#	$(CC) $(CFLAGS) -c usys.S
	$(OBJCOPY) -I binary fs.img fs_img.o
	$(LD) $(LDFLAGS) -T xv6.lds -o linkout kernel.o swtch.o call.o $(OBJS) $(LIB) -b binary initcode fs.img 
	$(OBJCOPY) -S -O binary linkout image


#initcode: helloword.o scif.o print.o assert.o spinlock.o usys.S
#	$(CC) $(CFLAGS) -I. -c initcode.S
#	$(CC) $(CFLAGS) -I. -c helloword.c
#	$(CC) $(CFLAGS) -c usys.S
#	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o initcode.out initcode.o
#	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o helloword.out helloword.o scif.o print.o assert.o spinlock.o usys.o $(LIB)
#	$(OBJCOPY) -S -O binary initcode.out initcode 
#	$(OBJCOPY) -S -O binary helloword.out initcode 
#	$(OBJDUMP) -S initcode.o > initcode.asm
#abcd: helloword.o scif.o printf.o assert.o spinlock.o usys.S init.c
#	$(CC) $(CFLAGS) -I. -c initcode.S
#	$(CC) $(CFLAGS) -I. -c init.c
#	$(CC) $(CFLAGS) -c usys.S
#	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o initcode.out initcode.o
#	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _init init.o printf.o usys.o $(LIB)
#	$(OBJCOPY) -S -O binary _init initcode 
#	$(OBJCOPY) -S -O binary init.out init 
#	$(OBJDUMP) -S initcode.o > initcode.asm
#abcd: helloword.o scif.o printf.o assert.o spinlock.o usys.S
#	$(CC) $(CFLAGS) -I. -c initcode.S
#	$(CC) $(CFLAGS) -I. -c test.c
#	$(CC) $(CFLAGS) -c usys.S
#	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o initcode.out initcode.o
#	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _init test.o printf.o usys.o $(LIB)
#	$(OBJCOPY) -S -O binary _init initcode 
#	$(OBJCOPY) -S -O binary init.out init 
#	$(OBJDUMP) -S initcode.o > initcode.asm
#abcd: scif.o printf.o assert.o spinlock.o usys.S ulib.c umalloc.c usertests.c
#	$(CC) $(CFLAGS) -I. -c initcode.S
#	$(CC) $(CFLAGS) -I. -c test.c
#	$(CC) $(CFLAGS) -I. -c usertests.c
#	$(CC) $(CFLAGS) -I. -c ulib.c
#	$(CC) $(CFLAGS) -I. -c umalloc.c
#	$(CC) $(CFLAGS) -c usys.S
#	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o initcode.out initcode.o
#	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _init test.o usertests.o printf.o usys.o umalloc.o ulib.o $(LIB)
#	$(OBJCOPY) -S -O binary _init initcode 
#	$(OBJCOPY) -S -O binary init.out init 
#	$(OBJDUMP) -S initcode.o > initcode.asm
initcode: scif.o printf.o assert.o spinlock.o usys.S ulib.c umalloc.c usertests.c cat.c echo.c forktest.c grep.c init.c kill.c ln.c ls.c mkdir.c rm.c sh.c wc.c zombie.c
	$(CC) $(CFLAGS) -I. -c initcode.S
	$(CC) $(CFLAGS) -I. -c test.c
	$(CC) $(CFLAGS) -I. -c usertests.c
	$(CC) $(CFLAGS) -I. -c echo.c
	$(CC) $(CFLAGS) -I. -c forktest.c
	$(CC) $(CFLAGS) -I. -c grep.c
	$(CC) $(CFLAGS) -I. -c init.c
	$(CC) $(CFLAGS) -I. -c kill.c
	$(CC) $(CFLAGS) -I. -c ln.c
	$(CC) $(CFLAGS) -I. -c ls.c
	$(CC) $(CFLAGS) -I. -c mkdir.c
	$(CC) $(CFLAGS) -I. -c rm.c
	$(CC) $(CFLAGS) -I. -c sh.c
	$(CC) $(CFLAGS) -I. -c zombie.c
	$(CC) $(CFLAGS) -I. -c cat.c
	$(CC) $(CFLAGS) -I. -c ulib.c
	$(CC) $(CFLAGS) -I. -c wc.c
	$(CC) $(CFLAGS) -I. -c umalloc.c
	$(CC) $(CFLAGS) -c usys.S
#	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o initcode.out initcode.o
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _usertests usertests.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _echo echo.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _forktest forktest.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _grep grep.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _init init.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _kill kill.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _ln ln.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _ls ls.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _mkdir mkdir.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _rm rm.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _sh sh.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _zombie zombie.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _wc wc.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _cat cat.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _test test.o printf.o usys.o umalloc.o ulib.o $(LIB)
	$(OBJCOPY) -S -O binary initcode.o initcode
#xv6.img: bootblock kernel fs.img
#	dd if=/dev/zero of=xv6.img count=10000
#	dd if=bootblock of=xv6.img conv=notrunc
#	dd if=kernel of=xv6.img seek=1 conv=notrunc

#bootblock: bootasm.S bootmain.c
#	$(CC) $(CFLAGS) -O -nostdinc -I. -c bootmain.c
#	$(CC) $(CFLAGS) -nostdinc -I. -c bootasm.S
#	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 -o bootblock.o bootasm.o bootmain.o
#	$(OBJDUMP) -S bootblock.o > bootblock.asm
#	$(OBJCOPY) -S -O binary bootblock.o bootblock
#	./sign.pl bootblock

#bootother: bootother.S
#	$(CC) $(CFLAGS) -nostdinc -I. -c bootother.S
#	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7000 -o bootother.out bootother.o
#	$(OBJCOPY) -S -O binary bootother.out bootother
#	$(OBJDUMP) -S bootother.o > bootother.asm

#initcode: initcode.S
#	$(CC) $(CFLAGS) -nostdinc -I. -c initcode.S
#	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o initcode.out initcode.o
#	$(OBJCOPY) -S -O binary initcode.out initcode
#	$(OBJDUMP) -S initcode.o > initcode.asm

#kernel: $(OBJS) bootother initcode
#	$(LD) $(LDFLAGS) -Ttext 0x100000 -e main -o kernel $(OBJS) -b binary initcode bootother
#	$(OBJDUMP) -S kernel > kernel.asm
#	$(OBJDUMP) -t kernel | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym

tags: $(OBJS)
	etags *.S *.c

#vectors.S: vectors.pl
#	perl vectors.pl > vectors.S

#ULIB = ulib.o usys.o printf.o umalloc.o

#_%: %.o $(ULIB)
#	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o $@ $^
#	$(OBJDUMP) -S $@ > $*.asm
#	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

#_forktest: forktest.o $(ULIB)
	# forktest has less library code linked in - needs to be small
	# in order to be able to max out the proc table.
#	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _forktest forktest.o ulib.o usys.o
#	$(OBJDUMP) -S _forktest > forktest.asm

#mkfs: mkfs.c fs.h
#	gcc -m32 werror -Wall -o mkfs mkfs.c

UPROGS=\
	_cat\
	_echo\
	_forktest\
	_grep\
	_init\
	_kill\
	_ln\
	_ls\
	_mkdir\
	_rm\
	_sh\
	_usertests\
	_wc\
	_zombie\


fs.img: mkfs README $(UPROGS) initcode
	./mkfs fs.img README $(UPROGS)

-include *.d

clean:
	rm -rf $(objects)
	rm -rf $(deps)
	rm -rf $(kernel)
	rm -rf $(UPROGS)


# make a printout
#FILES = $(shell grep -v '^\#' runoff.list)
#PRINT = runoff.list $(FILES)

xv6.pdf: $(PRINT)
	./runoff

print: xv6.pdf

# run in emulators

bochs : fs.img xv6.img
	if [ ! -e .bochsrc ]; then ln -s dot-bochsrc .bochsrc; fi
	bochs -q

qemu: fs.img xv6.img
	qemu -parallel stdio -hdb fs.img xv6.img

