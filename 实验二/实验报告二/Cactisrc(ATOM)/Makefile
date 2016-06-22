#ZTEIC main makefile

FIND =/usr/bin/find
SUB_DIR		:= bin
TOP_DIR         := $(shell /bin/pwd)
DIRS            := boot libc app driver


OUT_FILE_NAME    :=main
EXE_NAME        :=$(OUT_FILE_NAME).exe
BIN_NAME        :=$(OUT_FILE_NAME).bin
DUMP_NAME       :=$(OUT_FILE_NAME).dump
#============================= Path define
INCLUDE_DIR 	= $(TOP_DIR)/include
LINK_SCRIPT_DIR = $(TOP_DIR)/linkscript
BOOT_DIR 	= $(TOP_DIR)/boot
LIBC_DIR 	= $(TOP_DIR)/libc
DRIVER_DIR 	= $(TOP_DIR)/driver
APP_DIR 	= $(TOP_DIR)/app

export FIND TOP_DIR SUB_DIR APP_DIR DRIVER_DIR LIBC_DIR BOOT_DIR INCLUDE_DIR LINK_SCRIPT_DIR \
 SUB_DIR

LINK_SCRIPT	= normal.ld

OBJDUMP_OPT	= -D -t
OBJCOPY_BIN_OPT		= -O binary

all: clean $(BIN_NAME)

subdirs: $(patsubst %, _dir_%, $(DIRS))
$(patsubst %, _dir_%, $(DIRS)) : dummy
	$(MAKE) -C $(patsubst _dir_%, %, $@)

KERNEL_OBJ	= $(BOOT_DIR)/boot.o $(APP_DIR)/app.o $(DRIVER_DIR)/driver.o
ifneq ($(NOLIBC),y)
KERNEL_LINK_OBJ	= $(LIBC_DIR)/libc.o
endif

clean: $(patsubst %, _dir_clean_%, $(DIRS))
	rm -fr bin/* *.o

$(patsubst %, _dir_clean_%, $(DIRS)) : dummy
	rm  -f $(patsubst _dir_clean_%, %, $@)/*.o
	$(MAKE) -C $(patsubst _dir_clean_%, %, $@) clean


$(BIN_NAME)	: $(EXE_NAME)
	$(OBJCOPY)  $(OBJCOPY_BIN_OPT) $(SUB_DIR)/$(EXE_NAME)	$(SUB_DIR)/$@
	$(OBJDUMP) $(OBJDUMP_OPT) $(SUB_DIR)/$(EXE_NAME) >$(SUB_DIR)/$(DUMP_NAME)


$(EXE_NAME)	:
	$(MAKE) subdirs
	@if [ ! -d $(SUB_DIR) ]; then mkdir $(SUB_DIR); fi;
	cd $(SUB_DIR)
	$(LD) $(LD_OPT) -o $(SUB_DIR)/$@ $(KERNEL_OBJ) $(EXTRA_OBJ) \
           $(LIBS) $(KERNEL_LINK_OBJ)
dummy:
include Rules.make
