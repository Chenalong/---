# Subdirs.
# The target object and module list name.
O_TARGET	:= app.o

subdir-y+= $(APP)
obj-y   += $(APP)/$(APP)all.o

include $(TOP_DIR)/Rules.make

CLEAN_DIRS :=$(shell  $(FIND) . -not -name "." -type d -print)
clean: $(patsubst %, _dir_clean_%, $(CLEAN_DIRS))
	rm -f *.o

$(patsubst %, _dir_clean_%, $(CLEAN_DIRS)) : dummy
	rm  -f $(patsubst _dir_clean_%, %, $@)/*.o
