# Subdirs.
# The target object and module list name.
O_TARGET	:= $(APP)all.o

APP_SUBDIR=$(shell  $(FIND) . -not -name "." -type d -print)
obj-y    +=$(shell $(FIND) . -name "*.[cCSs]" -print | sed -e s/"\."[cCSs]/.o/ | sed -e s/"\.""\/"/" "/g )

include $(TOP_DIR)/Rules.make

clean: $(patsubst %, _dir_clean_%, $(APP_SUBDIR))
	rm -f *.o

$(patsubst %, _dir_clean_%, $(APP_SUBDIR)) : dummy
	rm  -f $(patsubst _dir_clean_%, %, $@)/*.o

