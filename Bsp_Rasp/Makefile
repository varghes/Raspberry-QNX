# This is the top-level Makefile for all source packages.
# It makes all the code in the "src" directory, then installs it
# in the "install" directory, then makes the images in
# the images directory (if present).

ROOT_DIR := $(notdir $(CURDIR))
ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)
unexport ROOT_DIR

.PHONY: all install clean links make_links dummy images prebuilt binaries

# Expands to a single newline character
define NEWLINE


endef

SUFFIXES := .mk

all: install links $(if $(wildcard images/*),images)
	@echo done

subdirs:=$(subst /Makefile,,$(wildcard */[Mm]akefile))

clean:
	$(foreach dir,$(subdirs), $(MAKE) -C$(dir) clean $(NEWLINE))
	-$(RM_HOST) -r install/*

install: $(if $(wildcard prebuilt/*),prebuilt)
	$(MAKE) -Csrc hinstall
	$(MAKE) -Csrc install

#
# Have to invoke "make_links" target because the first make expands
# the $(wildcard ...) too soon - we might not have copied things into
# the "install" tree yet.
#
links:
	$(MAKE) make_links

make_links:
	$(foreach file,$(wildcard install/*/boot/build/*),cd images;$(LN_HOST) ../$(file) $(notdir $(file));cd ..; )

images:
	$(MAKE) -Cimages

prebuilt:
	cp -rf prebuilt/* install

