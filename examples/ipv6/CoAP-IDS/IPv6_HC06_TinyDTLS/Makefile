# This is a custom Makefile for the original TinyDTLS example. 
# The reason behind this custom (instead of the original) is due the changes  
# between Contiki 2.6 and the Contiki 3.0

all: dtls-client dtls-server

CONTIKI=../../../..

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls
#APPS += tinydtls/aes  tinydtls/ecc tinydtls

CONTIKI_WITH_IPV6 = 1


SMALL=1

UIP_CONF_IPV6=1

# This will configure the server IPv6 address and the 
# the IPv6 addresses used by the clients to connect to the server.
ifneq ($(ROLE),client)
	CFLAGS+= -DHARD_CODED_ADDRESS=\"aaaa::02:232\"
else
	CFLAGS+= -DUDP_CONNECTION_ADDR="fe80::ff:fe02:232" \
		 -DHARD_CODED_ADDRESS=\"aaaa::02:230\"
endif

# To try to optimize the compilation (originally from TinyDTLS).
CFLAGS += -ffunction-sections
LDFLAGS += -Wl,--gc-sections,--undefined=_reset_vector__,--undefined=InterruptVectors,--undefined=_copy_data_init__,--undefined=_clear_bss_init__,--undefined=_end_of_init__

CFLAGS += -DSHA2_USE_INTTYPES_H

include $(CONTIKI)/Makefile.include