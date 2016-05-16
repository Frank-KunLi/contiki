/*
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
 *
 */

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include "dev/serial-line.h"

#include <string.h>

#include "tinydtls.h"

#ifndef DEBUG
#define DEBUG DEBUG_PRINT
#endif
#include "net/ip/uip-debug.h"

/*TESTING: Fast disable of the PRINTF */
#ifdef PRINTF
#if 0
#undef PRINTF
#define PRINTF
#endif
#endif

/* Used for testing different TinyDTLS versions */
#if  1
#include "dtls_debug.h" 
#else
#include "debug.h" 
#endif
#include "dtls.h"

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

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[UIP_LLIPH_LEN])

#define MAX_PAYLOAD_LEN 120

/* Odly, the original Contiki's examples put this as global.  */
static struct etimer pt_timer;

static struct uip_udp_conn *client_conn;
static dtls_context_t *dtls_context;
static char buf[] = "This is a quick test";
static size_t buflen = 21;

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

static void
try_send(struct dtls_context_t *ctx, session_t *dst) {
  int res;
  
  /*TESTING: Remove once everything works */
   PRINTF("TRY_SEND: Session - ");
   PRINT6ADDR(&dst->addr);
   PRINTF(":%u -\n", uip_ntohs(dst->port));
  
  res = dtls_write(ctx, dst, (uint8 *)buf, buflen);
  if (res >= 0) {
    memmove(buf, buf + res, buflen - res);
    buflen -= res;
  }
}

static int
read_from_peer(struct dtls_context_t *ctx, 
	       session_t *session, uint8 *data, size_t len) {
  size_t i;
  PRINTF("DATA RCV: ");
  for (i = 0; i < len; i++)
    PRINTF("%c", data[i]);
  return 0;
}

static int
send_to_peer(struct dtls_context_t *ctx, 
	     session_t *session, uint8 *data, size_t len) {

  struct uip_udp_conn *conn = (struct uip_udp_conn *)dtls_get_app_data(ctx);

  /*TESTING: Remove once everything works */
//   PRINTF("SEND_TO_PEER: ");
//   PRINT6ADDR(&conn->ripaddr);
//   PRINTF(":%u\n", uip_ntohs(conn->rport));
  
  uip_ipaddr_copy(&conn->ripaddr, &session->addr);
  conn->rport = session->port;

//   
   PRINTF("SEND_TO_PEER: Session: ");
   PRINT6ADDR(&session->addr);
   PRINTF(":%u\n", uip_ntohs(session->port));

  uip_udp_packet_send(conn, data, len);

  /* Restore server connection to allow data from any node */
  /* FIXME: do we want this at all? */
  memset(&conn->ripaddr, 0, sizeof(conn->ripaddr));
  memset(&conn->rport, 0, sizeof(conn->rport));

  return len;
}

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
get_psk_info(struct dtls_context_t *ctx UNUSED_PARAM,
	    const session_t *session UNUSED_PARAM,
	    dtls_credentials_type_t type,
	    const unsigned char *id, size_t id_len,
	    unsigned char *result, size_t result_length) {

  switch (type) {
  case DTLS_PSK_IDENTITY:
    if (result_length < psk_id_length) {
      dtls_warn("cannot set psk_identity -- buffer too small\n");
      return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
    }

    memcpy(result, psk_id, psk_id_length);
    return psk_id_length;
  case DTLS_PSK_KEY:
    if (id_len != psk_id_length || memcmp(psk_id, id, id_len) != 0) {
      dtls_warn("PSK for unknown id requested, exiting\n");
      return dtls_alert_fatal_create(DTLS_ALERT_ILLEGAL_PARAMETER);
    } else if (result_length < psk_key_length) {
      dtls_warn("cannot set psk -- buffer too small\n");
      return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
    }

    memcpy(result, psk_key, psk_key_length);
    return psk_key_length;
  default:
    dtls_warn("unsupported request type: %d\n", type);
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
static void
dtls_handle_read(dtls_context_t *ctx) {
  static session_t session;

  if(uip_newdata()) {
    uip_ipaddr_copy(&session.addr, &UIP_IP_BUF->srcipaddr);
    session.port = UIP_UDP_BUF->srcport;
    session.size = sizeof(session.addr) + sizeof(session.port);

    ((char *)uip_appdata)[uip_datalen()] = 0;
    PRINTF("Client received message from ");
    PRINT6ADDR(&session.addr);
    PRINTF(":%d\n", uip_ntohs(session.port));

    dtls_handle_message(ctx, &session, uip_appdata, uip_datalen());
  }
}
/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Client IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
    }
  }
}

static void
set_connection_address(uip_ipaddr_t *ipaddr)
{
  /*
   * NOTE: For this test we use directly a static IPv6 Add. 
   */
  uip_ip6addr(ipaddr,0xfe80,0,0,0,0x0200,0x0000,0x0000,0x0002);
  //uip_ip6addr(ipaddr, 0xaaaa, 0, 0, 0, 0x0200, 0, 0, 2);
}

void
init_dtls(session_t *dst) {
  static dtls_handler_t cb = {
    .write = send_to_peer,
    .read  = read_from_peer,
    .event = NULL,
#ifdef DTLS_PSK
    .get_psk_info = get_psk_info,
#endif /* DTLS_PSK */
#ifdef DTLS_ECC
    .get_ecdsa_key = get_ecdsa_key,
    .verify_ecdsa_key = verify_ecdsa_key
#endif /* DTLS_ECC */
  };
    
  PRINTF("DTLS client ( %s ) started\n", PACKAGE_STRING);
#ifdef DTLS_PSK
PRINTF("PSK-");
#endif  
#ifdef DTLS_ECC
PRINTF("ECC");
#endif  
PRINTF("\n");


  /*Different scope addresses*/
  uip_ipaddr_t ipaddr;
//  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0x0200, 0, 0, 3);
uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0x0200, 0, 0, 3);
  //uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
  
  print_local_addresses();

  
  dst->size = sizeof(dst->addr) + sizeof(dst->port);
  dst->port = UIP_HTONS(20220);

  set_connection_address(&dst->addr);
  client_conn = udp_new(&dst->addr, 0, NULL);
  udp_bind(client_conn, dst->port);

  PRINTF("Set conn to: ");
  PRINT6ADDR(&client_conn->ripaddr);
  PRINTF(":%d ", UIP_HTONS(client_conn->rport));
  PRINTF("Local port: %d\n", UIP_HTONS(client_conn->lport) );

  dtls_set_log_level(DTLS_LOG_INFO);

  dtls_context = dtls_new_context(client_conn);
  if (dtls_context){
    dtls_set_handler(dtls_context, &cb);
  }
  
}

/*---------------------------------------------------------------------------*/

PROCESS(udp_server_process, "UDP server process");
AUTOSTART_PROCESSES(&udp_server_process);

PROCESS_THREAD(udp_server_process, ev, data)
{
  static int connected = 0;
  static session_t dst;
  static int iBool = 1;

  
  PROCESS_BEGIN();

  dtls_init();

  init_dtls(&dst);
  serial_line_init();

  if (!dtls_context) {
    dtls_emerg("can't create context\n");
    PROCESS_EXIT();
  }

  while(iBool) {  
	/* 
	 *NOTE:Something is freezing the client and I'm very sure is the PROCESS_YIELD 
	 */
    etimer_set(&pt_timer, CLOCK_SECOND*3);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&pt_timer));
	
    if(ev == tcpip_event) {
      dtls_handle_read(dtls_context);
    } 

    /*
	 * TESTING: The original example used an serial connection, for my testing
	 * is a simple text to transmit (or a CoAP command). 
	 */
    if (buflen) {
      if (connected == 0){
		  PRINTF(" DEBUG: Client set connection to: ");
				PRINT6ADDR(&dst.addr);
				PRINTF(":%d\n", uip_ntohs(dst.port));
		connected = dtls_connect(dtls_context, &dst) >= 0;
		continue; //To remove?
	  }
	  
      try_send(dtls_context, &dst);
    }
    else {
	  PRINTF("Finish\n");
	  iBool = 0;
	}
  }
  
  /*TODO: Release the resources */
  dtls_free_context(dtls_context);
  connected = 0;
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
