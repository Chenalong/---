# ZTEIC make file version 1.0.0
# Makefile for the stdlib.
#

# Subdirs.
# The target object and module list name.
O_TARGET	:= libc.o
obj-y	:= ctype.o string.o vsprintf.o

include $(TOP_DIR)/Rules.make

clean: $(patsubst %, _dir_clean_%, $(subdir-y))
	rm -f *.o

$(patsubst %, _dir_clean_%, $(subdir-y)) : dummy
	rm  -f $(patsubst _dir_clean_%, %, $@)/*.o
	$(MAKE) -C $(patsubst _dir_clean_%, %, $@) clean

