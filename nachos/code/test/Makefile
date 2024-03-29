# Use regular make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
# Several things to be aware of: Nachos assumes that the location of
# the program startup routine (the location where the kernel jumps to
# when the program initially starts up) is location 0.  This means:
# start.o must be the first .o passed to ld, so that routine "Start"
# gets loaded at location 0
#
# if you are cross-compiling, you need to point to the right executables
# and change the flags to "ld" and the build procedure for "as".

NACHOS_ROOT = ../../
NACHOS_SYS := $(shell $(NACHOS_ROOT)/bin/nachos_sys)
NACHOS_ARCH := $(shell $(NACHOS_ROOT)/bin/nachos_arch)

ifeq ($(NACHOS_SYS),SOLARIS_SYS)
# Solaris/PC
ifeq ($(NACHOS_ARCH),X86_ARCH)
GCCDIR	= /net/cremi/aguermou/xgcc/decstation-ultrix/bin
endif
# Solaris/Sparc
ifeq ($(NACHOS_ARCH),SPARC_ARCH)
GCCDIR	= /net/cremi/aguermou/xgcc/decstation-ultrix/bin
endif
endif

ifeq ($(NACHOS_SYS),LINUX_SYS)
# Linux: fill in with the full path to the binutils and gcc.
# For instance:
GCCDIR	= /net/cremi/aguermou/xgcc/decstation-ultrix/bin
endif

ifeq ($(NACHOS_SYS),MAC_OS_SYS)
# Mac OS X
GCCDIR = /Volumes/NachOS/decstation-ultrix/bin
endif

LDFLAGS = -T script -N
ASFLAGS = 
CPPFLAGS = $(INCDIR) -DCHANGED


# If you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)


CC	:=	$(GCCDIR)/gcc
AS	:=	$(GCCDIR)/as
LD	:=	$(GCCDIR)/ld
OBJDUMP	:=	$(GCCDIR)/objdump

CPP	:=	$(GCCDIR)/gcc -E -P
INCDIR	:=	-I../userprog -I../threads
CFLAGS 	:=	-DIN_USER_MODE -c $(INCDIR) -G 0 -Wall -O2 -DCHANGED

SOURCES	:=	$(wildcard *.c)
PROGS0	:=	$(patsubst %.c,%,$(SOURCES))
PATTERN	:= vsprintf
PROGS 	:=	$(filter-out $(PATTERN), $(PROGS0))


.PHONY: all

all: $(PROGS)

start.o: start.S ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.S > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

%.o: %.c ../userprog/syscall.h
	$(CC) $(CFLAGS) -c $<

vsprintf.o: vsprintf.c
	$(CC) $(CFLAGS) -c $<


# LB: Caution! start.o should appear *before* $< for the load!

%.coff: %.o start.o vsprintf.o
	$(LD) $(LDFLAGS) start.o vsprintf.o $< -o $@

%.s: %.coff
	$(OBJDUMP) -d $< | sed -e 's/\<zero\>/r0/g;s/\<at\>/r1/g;s/\<v0\>/r2/g;s/\<v1\>/r3/g;s/\<a0\>/r4/g;s/\<a1\>/r5/g;s/\<a2\>/r6/g;s/\<a3\>/r7/g;s/\<t0\>/r8/g;s/\<gp\>/r28/g;s/\<sp\>/r29/g;s/\<s8\>/r30/g;s/\<ra\>/r31/g;' > $@

$(PROGS): %: %.coff
	../bin/coff2noff $< $@


# Cleaning rule
.PHONY: clean
clean:
	rm -f core *.coff *.o *.s $(PROGS)
