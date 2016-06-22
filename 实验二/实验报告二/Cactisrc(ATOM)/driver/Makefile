
# Subdirs.
# The target object and module list name.
O_TARGET	:= driver.o

ifeq ($(BOOTROM),y)
obj-y	:= usbdrv.o hal.o
else
obj-y	:= usbdrv.o hal.o
endif

include $(TOP_DIR)/Rules.make

clean: $(patsubst %, _dir_clean_%, $(subdir-y))
	rm -f *.o

$(patsubst %, _dir_clean_%, $(subdir-y)) : dummy
	rm  -f $(patsubst _dir_clean_%, %, $@)/*.o
	$(MAKE) -C $(patsubst _dir_clean_%, %, $@) clean

