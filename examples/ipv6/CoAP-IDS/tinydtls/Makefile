#This is a custom Makefile for the original TinyDTLS example 
# the reason behind this custom (instead of the original) is due the changes  
#between Contiki 2.6 and the Contiki 3.0

all: dtls-client dtls-server

CONTIKI=../../../..

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls
#APPS += tinydtls/aes  tinydtls/ecc tinydtls

CONTIKI_WITH_IPV6 = 1
include $(CONTIKI)/Makefile.include



# Thos are lines of TinyDTLS  they have the following legend: 
# "usually, you should not need changing anything beyond this line"
# BULLSHIT! I'm unable to compile the original example due too much 
# ROM memory requirement (PRobably I'm doing something wrong)
########################################################################

# the library's version (RAFS: Unknow why are here)
#VERSION:=0.8.2

# tools (RAFS: Unknow why are here)
#SHELL = /bin/sh
#MKDIR = mkdir

#WARNING RAFS: AL DEMONIO!
#abs_builddir = /home/user/contiki/examples/ipv6/CoAP-IDS/tinydtls/
#top_builddir = /home/user/contiki/apps/tinydtls
#top_srcdir:= /home/user/contiki/apps/tinydtls

UIP_CONF_IPV6=1

ifneq ($(ROLE),client)
	CFLAGS+= -DHARD_CODED_ADDRESS=\"aaaa::02:232\"
else
	CFLAGS+= -DUDP_CONNECTION_ADDR="fe80::ff:fe02:232" \
		 -DHARD_CODED_ADDRESS=\"aaaa::02:230\"
endif

CFLAGS += -ffunction-sections
LDFLAGS += -Wl,--gc-sections,--undefined=_reset_vector__,--undefined=InterruptVectors,--undefined=_copy_data_init__,--undefined=_clear_bss_init__,--undefined=_end_of_init__

CFLAGS += -DSHA2_USE_INTTYPES_H

#ifeq ("1", "1")
include $(CONTIKI)/Makefile.include
#endif