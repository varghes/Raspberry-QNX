define PINFO
PINFO DESCRIPTION=mmc/sd disk driver
endef
ifneq ($(wildcard $(PWD)/$(NAME).use),)
USEFILE=$(PWD)/$(NAME).use
endif
PUBLIC_INCVPATH += $(wildcard $(PROJECT_ROOT)/$(SECTION)/public )
