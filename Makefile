
# kernel should be placed at first or the order of sections will be wrong
OBJS = \
	kernel.o\
	main.o\
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
	swtch.o\
	syscall.o\
	sysfile.o\
	sysproc.o\
	tmu.o\
	exp.o\
	scif.o\
	vm.o\
	trap.o\
	console.o\
	call.o\

# Cross-compiling (e.g., on Mac OS X)
# TOOLPREFIX = i386-jos-elf-

# Using native tools (e.g., on X86 Linux)
TOOLPREFIX =sh-linux-

CC = $(TOOLPREFIX)gcc
AS = $(TOOLPREFIX)gas
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump
CFLAGS = -fno-builtin -Wall -MD -ggdb -nostdinc -I. -I$(shell $(CC) -print-file-name=include)
# important!
#CFLAGS += -m4-nofpu -DDEBUG
#ASFLAGS = -DDEBUG
CFLAGS += -m4-nofpu
ASFLAGS =
# FreeBSD ld wants ``elf_i386_fbsd''
LDFLAGS += 
LIB = $(shell $(CC) -print-libgcc-file-name)

image: initcode $(OBJS) $(LIB) fs.img
	$(OBJCOPY) -I binary fs.img fs_img.o
	$(LD) $(LDFLAGS) -T xv6.lds -o linkout $(OBJS) $(LIB) -b binary initcode fs.img 
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
	_stressfs\
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
