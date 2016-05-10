all: coaps-client coaps-server
# use target "er-plugtest-server" explicitly when requried 

CONTIKI=../../../..

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"




# automatically build RESTful resources
# RAFS: Original code 
REST_RESOURCES_DIR = ./resources
ifndef TARGET
REST_RESOURCES_FILES = $(notdir $(shell find $(REST_RESOURCES_DIR) -name '*.c'))
else
ifeq ($(TARGET), native)
REST_RESOURCES_FILES = $(notdir $(shell find $(REST_RESOURCES_DIR) -name '*.c'))
else
REST_RESOURCES_FILES = $(notdir $(shell find $(REST_RESOURCES_DIR) -name '*.c' ! -name 'res-plugtest*'))
endif
endif

PROJECTDIRS += $(REST_RESOURCES_DIR)
PROJECT_SOURCEFILES += $(REST_RESOURCES_FILES)

# linker optimizations
# RAFS: Original code 
SMALL=1

# REST Engine shall use Erbium CoAP implementation
APPS += coap-dtls
APPS += rest-engine
APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls

APPS += er-coaps



# optional rules to get assembly
# RAFS: Original code (NOTE: Requires analyze)
#CUSTOM_RULE_C_TO_OBJECTDIR_O = 1
#CUSTOM_RULE_S_TO_OBJECTDIR_O = 1

CONTIKI_WITH_IPV6 = 1
include $(CONTIKI)/Makefile.include


# Things taken from the tinydtls example Makefile
# Because are not the same syntax
########################################################################
#WARNING: The original Makefile from Contiki put the  IPv6 address  
# in hard way.  As we are to use other configuration, those ines were 
# transferred to the sourcodes.

#TODO: Averiguar que jodido hace esto
CFLAGS += -ffunction-sections
LDFLAGS += -Wl,--gc-sections,--undefined=_reset_vector__,--undefined=InterruptVectors,--undefined=_copy_data_init__,--undefined=_clear_bss_init__,--undefined=_end_of_init__

CFLAGS += -DSHA2_USE_INTTYPES_H

# Those are for letting communication between Cooja and the real world
# Though the real world are other VM.. bleh
########################################################################

# border router rules
# RAFS: Original code (Need deep analyze )
$(CONTIKI)/tools/tunslip6:	$(CONTIKI)/tools/tunslip6.c
	(cd $(CONTIKI)/tools && $(MAKE) tunslip6)

connect-router:	$(CONTIKI)/tools/tunslip6
	sudo $(CONTIKI)/tools/tunslip6 aaaa::1/64

connect-router-cooja:	$(CONTIKI)/tools/tunslip6
	sudo $(CONTIKI)/tools/tunslip6 -a 127.0.0.1 -p 60002 aaaa::1/64

connect-router-native:	$(CONTIKI)/examples/ipv6/native-border-router/border-router.native
	sudo $(CONTIKI)/exmples/ipv6/native-border-router/border-router.native -a 127.0.0.1 -p 60002 aaaa::1/64

connect-minimal:
	sudo ip address add fdfd::1/64 dev tap0 
