ROOT_DIR := $(notdir $(CURDIR))
ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

HOST_MKIFS := mkifs

SUFFIXES := .build .ifs .bin

.PHONY: all clean

all: ifs-bcm2835.bin 

clean: 
	$(RM_HOST) ifs-bcm2835.bin 

ifs-bcm2835.bin: bcm2835.build
	$(HOST_MKIFS) -r../install $(MKIFSFLAGS) $^ $@

