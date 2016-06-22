# ZTEIC make file version 1.0.0
# Makefile for the bootcode.
#

# Subdirs.

O_TARGET	:= boot.o


obj-y	:= head.o init.o trap.o


include $(TOP_DIR)/Rules.make

clean: $(patsubst %, _dir_clean_%, $(subdir-y))
	rm -f *.o

$(patsubst %, _dir_clean_%, $(subdir-y)) : dummy
	rm  -f $(patsubst _dir_clean_%, %, $@)/*.o
	$(MAKE) -C $(patsubst _dir_clean_%, %, $@) clean
