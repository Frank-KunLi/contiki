6LoWPAN (HC06 Compression) with UDP and CoAP
Tentative:  Tiny-CoAPS
===================================================

General objective 
-------------
  Basic scenario with nodes using HC06 dispatchs with 6LoWPAN. 
  The nodes are using CoAP upper an (tiny)DTLS layer, or CoAP Secure (CoAPS).

  This scenario is based in the original example examples/er-coap/*

  Key components:
   - 1 6LoWPAN  Border router (6BR).
   - N  CoAPS client.
   - 1  CoAPS server. 

PRELIMINARIES
------------- 

TODO

COOJA HOWTO
-----------

TODO 

  The 6BR is located in:
     contiki/examples/ipv6/rpl-border-router/border-router.c

  The client and server are here. The server alose requires ./resources/.

  
TinyDTLS
-----------
	
  In apps/tinydtls is located the current version of TinyDTLs. 
  The new official site is located in: 

     https://projects.eclipse.org/proposals/tinydtls

  Previous  to February 2016, Tinydtls was located in:
     http://sourceforge.net/p/tinydtls/code/ci/master/tree/

  More info about configuration in the project-conf.h file.
