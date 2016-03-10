/*
 * Copyright (c) 2013, Todo
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      IPv6 HC06 CoAP (Normal UDP)
 * \author
 *      Raul Armando Fuentes Samaniego <fuentess@telecom...>
 */

/* This project-conf reflex one similar for CoAP Secure  
 * However, in this project is being used CoAP and thus is possible 
 * to connect to the nodes using a webbrowser that supports CoAP. 
 * 
 * If you are using Cooja you can run the following once the simulation
 * is running:
 *     make connect-router-cooja
 *
 * Then in Firefox (with the plugin Copper or Cu already installed) 
 * try the following addresses: 
 *   http://[aaaa::212:7401:1:101]:80/ (The 6BR Router)
 *   coap://[aaaa::212:7402:2:202]:5683/ (The CoAP server)
 */ 


/*  Basic configuration for the nodes. 
 *  802.15.4 for the framers. 
 *  CSMA/CA for the transmission 
 *  The CC24240 drivers for the signal 
 *  And the default (contikimac)  RDC driver (not stated here) 
 */

//#undef NETSTACK_CONF_RADIO
//#define NETSTACK_CONF_RADIO   cc2420_driver

#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER  framer_802154

/*  The default confguration for AES-128 is by hardware 
 *  however is not possible to measure in simulation. 
 *  aes_128_driver executed software implementation of AES-128.
 */
//#undef AES_128_CONF 
//#define AES_128_CONF cc2420_aes_128_driver //Hardware
//#define AES_128_CONF aes_128_driver	    //Software


#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     csma_driver

/* 
 * Contikimac requires an small tresshold for the MTU. 
 * Original paper say 23 bytes, but the source-code for Contiki 3.x has 63
 * If the tresshold is not passed there will not be compression WHICH  
 * is bad because force the nodes to have IPv6 and 6LoWPAN properties.
 * Plus for the IDS will be getting hard time with  both  protocols
 * IEEE 802.15.4  seem to have 17.5 bytes + Payload
 * 6LoWPAN header is 5-6 Bytes + Payload
 * UDP 8 bytes + app data || 6UDP  6 bytes + data
 * 
 * The tresshold will be 23 because should be enough for the app 
 * Though this force do not display TCP and check the ICMPv6 tresshold.
 * 
 * NOTE: For this project the 6BR router is compiled from their own example folder 
 * Thus is not using this file for their own configuration.
 */
#undef SICSLOWPAN_CONF_COMPRESSION_THRESHOLD
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD     23


// This enable 6LoWPAN + 6UDP (Or Contiki equivalent)
#undef SICSLOWPAN_CONF_COMPRESSION
#define SICSLOWPAN_CONF_COMPRESSION     SICSLOWPAN_COMPRESSION_HC06 

/* 
 * The following are from the CoAP examples
 * No he tomado todos, solo lo que creo que son importantes para el experimento.
 */

/* Disabling TCP on CoAP nodes. */
#undef UIP_CONF_TCP
#define UIP_CONF_TCP                   0


/* Increase rpl-border-router IP-buffer when using more than 64. */
//RAFS TODO: Indagar que jodido hace esto 
#undef REST_MAX_CHUNK_SIZE
#define REST_MAX_CHUNK_SIZE            48


/* Multiplies with chunk size, be aware of memory constraints. */
#undef COAP_MAX_OPEN_TRANSACTIONS
#define COAP_MAX_OPEN_TRANSACTIONS     4


#undef PLATFORM_HAS_BUTTON
#define PLATFORM_HAS_BUTTON 1

#undef PLATFORM_HAS_LEDS
#define PLATFORM_HAS_LEDS 1

/* Filtering .well-known/core per query can be disabled to save space.
#undef COAP_LINK_FORMAT_FILTERING
#define COAP_LINK_FORMAT_FILTERING     0
#undef COAP_PROXY_OPTION_PROCESSING
#define COAP_PROXY_OPTION_PROCESSING   0  */

