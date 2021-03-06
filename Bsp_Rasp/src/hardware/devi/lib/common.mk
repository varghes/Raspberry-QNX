ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

INSTALLDIR=/dev/null

define PINFO
PINFO DESCRIPTION=Input DDK common library
endef

NAME=input
INSTALLDIR=usr/lib

EXTRA_SILENT_VARIANTS=sengine

LIBS+=keymap

include $(MKFILES_ROOT)/qmacros.mk

-include $(PROJECT_ROOT)/roots.mk
#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../../install
   USE_INSTALL_ROOT=1
##############################################################
ifndef DEVI_ROOT
DEVI_ROOT=$(PROJECT_ROOT)/..
endif
ifndef KEYMAP_ROOT
KEYMAP_ROOT=$(PROJECT_ROOT)/../keymap
endif

EXTRA_INCVPATH += $(DEVI_ROOT)/include $(DEVI_ROOT)/public $(DEVI_ROOT)/private
EXTRA_LIBVPATH += $(KEYMAP_ROOT)/$(OS)/$(CPU)/$(COMPOUND_VARIANT)

include $(MKFILES_ROOT)/qtargets.mk
