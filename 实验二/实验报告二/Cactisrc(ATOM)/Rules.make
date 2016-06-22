#ZTEIC makefile version 1.0

.PHONY: dummy

MACRO_C		=  -DSMIC_PLL  -DBANK0=32 -DTLB_SIZE=4000 -DVERBOSE  -DENDIAN=1
ifeq ($(HOST),y)
MACRO_C		+= -DHOST_MODE_EN=1
else
MACRO_C		+= -DSIM_FPGA -DHOST_MODE_EN=0
endif
ifeq ($(BOOTROM),y)
MACRO_C		+= -DBOOTROM_MODE=1
else
MACRO_C		+= -DBOOTROM_MODE=0
endif

#============================= Default define in make file, the lowest priority.
GCC_PATH	= $(ARCA_GCC_PATH)
GLIB_C_INCLUDE  = $(ARCA_GLIB_C_INCLUDE)
PREFIX		= $(GCC_PATH)arca-
ELF_DIR		= arca-linux
GCC_VER		= 3.0
GCC_EXTRA_OPT   = -ml

#============================= Tools define
GCC      	= $(PREFIX)gcc
AS		= $(PREFIX)as
LD		= $(PREFIX)ld
OBJCOPY 	= $(PREFIX)objcopy
OBJDUMP 	= $(PREFIX)objdump

#============================= Link lib define
LIB_M_C		= -L $(GCC_EXEC_PREFIX)../../$(ELF_DIR)/lib/$(ML_DIR) 
LIB_GCC		= -L $(GCC_EXEC_PREFIX)$(ELF_DIR)/$(GCC_VER)/$(ML_DIR) -lgcc
LIBS		= $(LIB_M_C)$(LIB_GCC)

#============================= Internal macro define 
INCLUDE		= $(INCLUDE_DIR)/instructions.h	$(INCLUDE_DIR)/test.h
LIB_GCC_INCLUDE = $(GCC_EXEC_PREFIX)$(ELF_DIR)/$(GCC_VER)/include
GCC_DEFAULT_OPT	= -I $(INCLUDE_DIR) -I . -I $(LIB_GCC_INCLUDE) -I $(GLIB_C_INCLUDE) -I $(LIBC_DIR) -Os

GCC_OPT  	= $(GCC_DEFAULT_OPT) $(GCC_EXTRA_OPT) $(MACRO_C) 
LD_OPT 		= -T $(LINK_SCRIPT_DIR)/$(LINK_SCRIPT) -nostdlib
# -nostdlib -static

SUB_DIRS	:= $(subdir-y)
# Common rules

%.o: %.c
	$(GCC) $(GCC_OPT) -c -o $@ $<
%.o: %.S
	$(GCC) $(GCC_OPT) -c -o $@ $<
%.o: %.C
	$(GCC) $(GCC_OPT) -xc -c -o  $@ $<
%.o: %.s
	$(GCC) $(GCC_OPT) -c -o $@ $<

first_rule: sub_dirs 
	$(MAKE) all_targets


all_targets: $(O_TARGET)

#
# Rule to compile a set of .o files into one .o file
#
ifdef O_TARGET
$(O_TARGET): $(obj-y)
	$(LD) -r -o $@ $(obj-y)
endif

subdir-list = $(sort $(patsubst %,_subdir_%,$(SUB_DIRS)))

ifneq "$(SUB_DIRS)" ""
sub_dirs: dummy $(subdir-list)
else
sub_dirs: dummy
endif

$(subdir-list) : dummy
	$(MAKE) -C $(patsubst _subdir_%,%,$@)
dummy:
