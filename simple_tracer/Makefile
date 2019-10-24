BUILDROOT_DIR := ..
KDIR := $(BUILDROOT_DIR)/output/build/linux-custom
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-uclibc-gcc
APP = simple_tracer
all:
	$(COMPILER) -o $(APP) $(APP).c -Wall
	cp $(APP) $(BUILDROOT_DIR)/output/target/bin
	
clean:
	rm -f *.o 
	rm -f $(APP)
