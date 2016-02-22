/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
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
 *      CoAP Secure (CoAPS) client example.
 * \author
 *      Raul Fuentes Samaniego <fuentess@telecom-sudparis.eu>
 * \note 
 * 		This is an client model which will connect only to one specific
 * 		server requesting CoAP resources. 
 * 		set_connection_address() requires attention if the server IPv6
 * 		addresses will be modified.
 * 
 *      WARNING: If the debugs are configured this client is in the limit 
 * 		of the ROM space.
 */


//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include "er-coaps-engine.h"

#if PLATFORM_HAS_BUTTON
#include "dev/button-sensor.h"
#endif

#include <string.h>

#include "tinydtls.h"
//#include "net/ip/uip-debug.h"

#ifdef TINYDTLS_DEBUG
#include "tinydtls_debug.h" 
#endif
//#include "dtls.h"

#if TINYDTLS_DEBUG
#include <stdio.h>
#define printf(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) printf("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) printf("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define printf(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif


//Definidos en contiki/core/*udp*
//#define LOCAL_PORT      UIP_HTONS(COAP_DEFAULT_PORT + 1)
//#define REMOTE_PORT     UIP_HTONS(COAP_DEFAULT_PORT)

#define LOCAL_PORT      UIP_HTONS(20220 + 1)
#define REMOTE_PORT     UIP_HTONS(20220)

/*NOTE: PRobably the most important element */
#define TOGGLE_INTERVAL 100


//Defines de TinyDTLS
#ifdef DTLS_PSK
/* The PSK information for DTLS */
/* make sure that default identity and key fit into buffer, i.e.
 * sizeof(PSK_DEFAULT_IDENTITY) - 1 <= PSK_ID_MAXLEN and
 * sizeof(PSK_DEFAULT_KEY) - 1 <= PSK_MAXLEN
*/

#define PSK_ID_MAXLEN 32
#define PSK_MAXLEN 32
#define PSK_DEFAULT_IDENTITY "Client_identity"
#define PSK_DEFAULT_KEY      "secretPSK"
#endif /* DTLS_PSK */


#define UIP_IP_BUF_   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF_  ((struct uip_udp_hdr *)&uip_buf[UIP_LLIPH_LEN])

uip_ipaddr_t server_ipaddr;
static struct uip_udp_conn *client_conn;
static dtls_context_t *dtls_context_client;



static uint8_t dtls_connected = 0;

/* FIXME: This is the most unsecure and unprofessional way to  work with keys 
 * TODO:  This should be with #ifdef DTLS_ECC ?
 */
static const unsigned char ecdsa_priv_key[] = {
			0x41, 0xC1, 0xCB, 0x6B, 0x51, 0x24, 0x7A, 0x14,
			0x43, 0x21, 0x43, 0x5B, 0x7A, 0x80, 0xE7, 0x14,
			0x89, 0x6A, 0x33, 0xBB, 0xAD, 0x72, 0x94, 0xCA,
			0x40, 0x14, 0x55, 0xA1, 0x94, 0xA9, 0x49, 0xFA};

static const unsigned char ecdsa_pub_key_x[] = {
			0x36, 0xDF, 0xE2, 0xC6, 0xF9, 0xF2, 0xED, 0x29,
			0xDA, 0x0A, 0x9A, 0x8F, 0x62, 0x68, 0x4E, 0x91,
			0x63, 0x75, 0xBA, 0x10, 0x30, 0x0C, 0x28, 0xC5,
			0xE4, 0x7C, 0xFB, 0xF2, 0x5F, 0xA5, 0x8F, 0x52};

static const unsigned char ecdsa_pub_key_y[] = {
			0x71, 0xA0, 0xD4, 0xFC, 0xDE, 0x1A, 0xB8, 0x78,
			0x5A, 0x3C, 0x78, 0x69, 0x35, 0xA7, 0xCF, 0xAB,
			0xE9, 0x3F, 0x98, 0x72, 0x09, 0xDA, 0xED, 0x0B,
			0x4F, 0xAB, 0xC3, 0x6F, 0xC7, 0x72, 0xF8, 0x29};
			

/* Example URIs that can be queried.
   WARNING: Light and toogle  depends on hardware (which are defined at 
   compilation time in the project-conf.h).
   However, only will result in failed requests
   
   WARNING: Something happens when the NUMBER_OF_URLS is one
   Memory don't fit anymore (by 2 bytes)
*/
#define NUMBER_OF_URLS 4
/* leading and ending slashes only for demo purposes, get cropped automatically when setting the Uri-Path */
//char *service_urls[NUMBER_OF_URLS] =
//{ ".well-known/core", "./actuators/toggle", "battery/", "error/in//path" };
char *service_urls[NUMBER_OF_URLS] =
{ ".well-known/core", "./actuators/toggle", "test/hello", "sensors/light"  };
/*char *service_urls[NUMBER_OF_URLS] =
{ ".well-known/core" };*/
static int uri_switch = 0;



/*---------------------------------------------------------------------------*/
/* NOTE: Relacionados con las llaves con TinyDTLS */

#ifdef DTLS_PSK
static unsigned char psk_id[PSK_ID_MAXLEN] = PSK_DEFAULT_IDENTITY;
static size_t psk_id_length = sizeof(PSK_DEFAULT_IDENTITY) - 1;
static unsigned char psk_key[PSK_MAXLEN] = PSK_DEFAULT_KEY;
static size_t psk_key_length = sizeof(PSK_DEFAULT_KEY) - 1;

#ifdef __GNUC__
#define UNUSED_PARAM __attribute__((unused))
#else
#define UNUSED_PARAM
#endif /* __GNUC__ */

/* This function is the "key store" for tinyDTLS. It is called to
 * retrieve a key for the given identity within this particular
 * session. */
static int
get_psk_info_client(struct dtls_context_t *ctx UNUSED_PARAM,
	    const session_t *session UNUSED_PARAM,
	    dtls_credentials_type_t type,
	    const unsigned char *id, size_t id_len,
	    unsigned char *result, size_t result_length) {

  switch (type) {
  case DTLS_PSK_IDENTITY:
    if (result_length < psk_id_length) {
#if TINYDTLS_DEBUG
      dtls_warn("cannot set psk_identity -- buffer too small\n");
#endif
      return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
    }

    memcpy(result, psk_id, psk_id_length);
    return psk_id_length;
  case DTLS_PSK_KEY:
    if (id_len != psk_id_length || memcmp(psk_id, id, id_len) != 0) {
#if TINYDTLS_DEBUG
      dtls_warn("PSK for unknown id requested, exiting\n");
#endif
      return dtls_alert_fatal_create(DTLS_ALERT_ILLEGAL_PARAMETER);
    } else if (result_length < psk_key_length) {
#if TINYDTLS_DEBUG		
		dtls_warn("cannot set psk -- buffer too small\n");
#endif		
      return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
    }

    memcpy(result, psk_key, psk_key_length);
    return psk_key_length;
  default:
#if TINYDTLS_DEBUG
    dtls_warn("unsupported request type: %d\n", type);
#endif
  }

  return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
}
#endif /* DTLS_PSK */

#ifdef DTLS_ECC
static int
get_ecdsa_key(struct dtls_context_t *ctx,
	      const session_t *session,
	      const dtls_ecdsa_key_t **result) {
  static const dtls_ecdsa_key_t ecdsa_key = {
    .curve = DTLS_ECDH_CURVE_SECP256R1,
    .priv_key = ecdsa_priv_key,
    .pub_key_x = ecdsa_pub_key_x,
    .pub_key_y = ecdsa_pub_key_y
  };

  *result = &ecdsa_key;
  return 0;
}

static int
verify_ecdsa_key(struct dtls_context_t *ctx,
		 const session_t *session,
		 const unsigned char *other_pub_x,
		 const unsigned char *other_pub_y,
		 size_t key_size) {
  return 0;
}
#endif /* DTLS_ECC */

/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*NOTE: Tomados directamente de Lithe (y validados con TinyDTLS 0.8.2*/

/* This function is will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void
client_chunk_handler(void *response)
{
  const uint8_t *chunk;

  int len = coap_get_payload(response, &chunk);

  printf("|%.*s", len, (char *)chunk);
}


 static int
read_from_peer_client(struct dtls_context_t *ctx, 
	       session_t *session, uint8 *data, size_t len) {
  
	
	//NOTE: The data read is a CoAP header with its payload)
	
	/* Set upip length*/
  uip_len = len;
  memmove(uip_appdata, data, len);
  
  coap_receive(ctx, session);
  return 0;
}
static int
send_to_peer_client(struct dtls_context_t *ctx, 
	     session_t *session, uint8 *data, size_t len) {

  struct uip_udp_conn *conn = (struct uip_udp_conn *)dtls_get_app_data(ctx);

  uip_ipaddr_copy(&conn->ripaddr, &session->addr);
  conn->rport = session->port;

  printf("send to ");
  PRINT6ADDR(&conn->ripaddr);
  printf(":%u\n", uip_ntohs(conn->rport));

  uip_udp_packet_send(conn, data, len);

  /* Restore server connection to allow data from any node */
  /* FIXME: do we want this at all? */
  memset(&conn->ripaddr, 0, sizeof(client_conn->ripaddr));
  memset(&conn->rport, 0, sizeof(conn->rport));

  return len;
}

/* */ 
static int
on_event_client(struct dtls_context_t *ctx, session_t *session, dtls_alert_level_t level,
              unsigned short code) {
  if (code == DTLS_EVENT_CONNECTED) {
    dtls_connected = 1;
    PRINTF("DTLS-Client Connected\n");
  }
  return 0;
}



static void
dtls_handle_read_client(dtls_context_t *ctx) {
  static session_t session;

  if(uip_newdata()) {
    uip_ipaddr_copy(&session.addr, &UIP_IP_BUF_->srcipaddr);
    session.port = UIP_UDP_BUF_->srcport;
    session.size = sizeof(session.addr) + sizeof(session.port);

    ((char *)uip_appdata)[uip_datalen()] = 0;
    PRINTF("Client received %u Byte message from ", uip_datalen());
    PRINT6ADDR(&session.addr);
    PRINTF(":%d\n", uip_ntohs(session.port));

    dtls_handle_message(ctx, &session, uip_appdata, uip_datalen());
  }
}

/*---------------------------------------------------------------------------*/
/* NOTE: Tomados de TinyDTLS 0.8.2 (basados en el init_dtls de Lithe pero 
		 actualizados (PARTICULARMENTE LAS LLAVES). */

//FIXME: This only useful for a single server
static void
set_connection_address(uip_ipaddr_t *ipaddr)
{
  uip_ip6addr(ipaddr,0xfe80, 0, 0, 0, 0x0200, 0x0000, 0x0000, 0x0003);  
//  uip_ip6addr(&server_ipaddr, 0xfe80, 0, 0, 0, 0x0200, 0x0000, 0x0000, 0x0003);
  //FIXME: The #define es por el coap_request_block
//  #define SERVER_NODE(ipaddr)   uip_ip6addr(ipaddr, 0xfe80, 0, 0, 0, 0x0200, 0x0000, 0x0000, 0x0003)   
// SERVER_NODE(&server_ipaddr);
  
  //TODO Something to localize other server.
  
}


static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  printf("Client IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
    }
  }
}

/*---------------------------------------------------------------------------*/
void
init_dtls_client(session_t *dst) {
	
  static dtls_handler_t cb_client = {
    .write = send_to_peer_client,
    .read  = read_from_peer_client,
    .event = on_event_client,
#ifdef DTLS_PSK
    .get_psk_info = get_psk_info_client,
#endif /* DTLS_PSK */
#ifdef DTLS_ECC
    .get_ecdsa_key = get_ecdsa_key,
    .verify_ecdsa_key = verify_ecdsa_key
#endif /* DTLS_ECC */

/*FIXME: Aqui deberia ir DTLS_Null (No cifrado)???*/
  };
  
  
  printf("CoAPS   client ( %s ) started\n", PACKAGE_STRING);

  print_local_addresses();

  dst->size = sizeof(dst->addr) + sizeof(dst->port);
  dst->port = REMOTE_PORT;


  set_connection_address(&dst->addr);
  client_conn = udp_new(&dst->addr, 0, NULL);
  udp_bind(client_conn, LOCAL_PORT);

  printf("set connection address to ");
  PRINT6ADDR(&dst->addr);
  printf(":%d\n", uip_ntohs(dst->port));

  //set_log_level(DTLS_LOG_DEBUG); //No no no
  
  dtls_context_client = dtls_new_context(client_conn);
  if (dtls_context_client)
    dtls_set_handler(dtls_context_client, &cb_client);
}
/*---------------------------------------------------------------------------*/

/* NOTE: One single process to run everything
 */


PROCESS(coaps_client, "CoAPS Client Example");
AUTOSTART_PROCESSES(&coaps_client);


PROCESS_THREAD(coaps_client, ev, data)
{
  static int connected = 0;
  static struct etimer app_et;
  static coap_packet_t request[1]; 
  static session_t dst_process; 


  PROCESS_BEGIN();

  dtls_init();

  init_dtls_client(&dst_process);


  if (!dtls_context_client) {
#if TINYDTLS_DEBUG
    dtls_emerg( "cannot create context! \n");
#else 
	printf("cannot create context! (Epic fail) \n");
#endif
    PROCESS_EXIT();
  }
  

  /* receives all CoAP messages */
  //coap_receiver_init(); //NOTE: TOO OLD (legacy)
  coap_register_as_transaction_handler();
  PRINTF("Initializing CoAPS client!\n");
  
  connected = dtls_connect(dtls_context_client, &dst_process) >= 0;
  
  etimer_set(&app_et, CLOCK_SECOND * TOGGLE_INTERVAL);
  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {

      dtls_handle_read_client(dtls_context_client);
    } else if (ev == PROCESS_EVENT_TIMER) {
		
		
		if	(etimer_expired(&app_et) && dtls_connected) {
			coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
			coap_set_header_uri_path(request, service_urls[uri_switch]);

			PRINTF("--Requesting %s--\n", service_urls[uri_switch]);


			COAP_BLOCKING_REQUEST(&(dst_process.addr), &(dst_process.port), request, 
									client_chunk_handler, dtls_context_client, &dst_process);

			PRINTF("\n--Done--\n");

			uri_switch = (uri_switch+1) % NUMBER_OF_URLS;
			etimer_reset(&app_et);
		}
		else if (etimer_expired(&app_et)) { 
			etimer_reset(&app_et);
			
		}
		
		//If the previous fail we need to try a new connection
		if (!connected) {
			connected = dtls_connect(dtls_context_client, &dst_process) >= 0;
			etimer_set(&app_et, CLOCK_SECOND * TOGGLE_INTERVAL);
			continue;
		}
		/* retransmissions are handled here */
		coap_check_transactions();
    
	}
  }
  
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/


