rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

homepath := /home/adam/osdev-stivale-test/
arch ?= x86_64
kernel := output/kernel.elf
iso := output/kernel.iso
SOURCEDIR := /home/adam/osdev-stivale-test/src
BUILDDIR := /home/adam/osdev-stivale-test/output
DIRS = boot memoryManager common busComm idt time

GCC_FLAGS ?= -O2 -g -Wall -Wextra -pipe -fno-stack-protector -fpermissive

LDFLAGS ?=


override INTERNALCFLAGS :=   \
	-I.                  \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic             \
	-mabi=sysv           \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mno-red-zone        \
	-mcmodel=kernel      \
	-fsanitize=address   \
	-static-libasan      \
	-MMD


override INTERNALLDFLAGS :=    \
	-Tlinker.ld            \
	-nostdlib              \
	-zmax-page-size=0x1000 \
	-static


SOURCEDIRS = $(foreach dir, $(DIRS), $(addprefix $(SOURCEDIR)/, $(dir)))
TARGETDIRS = $(foreach dir, $(DIRS), $(addprefix $(BUILDDIR)/, $(dir)))

cpp_source_files = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.cpp))
cpp_object_files := $(patsubst $(SOURCEDIRS),$(TARGETDIRS),$(cpp_source_files:.cpp=.o))

assembly_source_files = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.asm))
assembly_object_files := $(patsubst $(SOURCEDIRS),$(BUILDDIR),$(assembly_source_files:.asm=.o))


objects := $(cpp_object_files) 
SRC := $(cpp_source_files) 

linker_script := linker.ld
grub_cfg := isofiles/boot/grub/grub.cfg

BUILD_DIR ?= ./output/


.PHONY: all clean run iso

all: $(kernel)

clean:
	rm -R $(cpp_object_files) $(assembly_object_files)
	@mkdir -p output

run: $(iso)
	qemu-system-x86_64 -cdrom $(iso) -no-reboot -no-shutdown -d int -s -S -D qemu.log

debug: $(iso)
	@bochs -q


iso: $(iso)

$(iso): $(kernel) $(grub-cfg)
	@mkdir -p iso_dir
	@cp -v output/kernel.elf limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin iso_dir/
	@xorriso -as mkisofs -b limine-cd.bin         -no-emul-boot -boot-load-size 4 -boot-info-table    --efi-boot limine-eltorito-efi.bin         -efi-boot-part --efi-boot-image --protective-msdos-label    iso_dir -o $(iso)

$(kernel): $(linker_script) $(assembly_object_files) $(cpp_object_files) 
	ld $(LDFLAGS) $(INTERNALLDFLAGS) -o $(kernel) $(assembly_object_files) $(cpp_object_files)



%.o: %.asm
	@mkdir -p $(shell dirname $@)
	@echo $(cpp_object_files)
	nasm -felf64 -F dwarf -g -o $@ $<



%.o: %.cpp
	@mkdir -p $(shell dirname $@)
	@echo $<
	g++ $(GCC_FLAGS) -c $< -o $@