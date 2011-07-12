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
	$(OBJCOPY) -I binary fs.img fs_img.o
	$(LD) $(LDFLAGS) -T xv6.lds -o linkout kernel.o swtch.o call.o $(OBJS) $(LIB) -b binary initcode fs.img 
	$(OBJCOPY) -S -O binary linkout image

initcode: initcode.S
	$(CC) $(CFLAGS) -I. -c initcode.S
	$(OBJCOPY) -S -O binary initcode.o initcode

tags: $(OBJS)
	etags *.S *.c

ULIB = ulib.o usys.o printf.o umalloc.o

_%: %.o $(ULIB) $(LIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o $@ $^
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

_forktest: forktest.o $(ULIB)
	# forktest has less library code linked in - needs to be small
	# in order to be able to max out the proc table.
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _forktest forktest.o ulib.o usys.o
	$(OBJDUMP) -S _forktest > forktest.asm

mkfs: mkfs.c fs.h
	gcc -m32 -Werror -Wall -o mkfs mkfs.c

UPROGS=\
	_cat\
	_test\
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
	rm -rf *.o *.d *.asm *.sym \
	initcode image linkout fs.img mkfs \
	$(UPROGS) 
