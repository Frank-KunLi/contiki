########################################################################
# Original Makefile is too alien for Contiki 3.0 (and probably 2.6)
all: dtls-server dtls-client


CONTIKI=../../../..

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

PROJECTDIRS += $(REST_RESOURCES_DIR)
PROJECT_SOURCEFILES += $(REST_RESOURCES_FILES)

APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls

CONTIKI_WITH_IPV6 = 1
include $(CONTIKI)/Makefile.include


#######################################################################
# TinyDTLS originally try to optimize space when compiling
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