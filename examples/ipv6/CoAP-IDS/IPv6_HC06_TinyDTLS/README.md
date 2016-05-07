# TinyDTLS example. 

This is using the original TinyDTLS 0.8.2 (Sourceforge) examples. Yet, the 
Makefile has been modified and a new project-conf.h generated.

The 6BR is configured for connecting the exteriro to Cooja. 
So, should be possible to use an extern TinyDTLS client (like Linux) to connect
to the server.

The example are very simple: The server will display the data sent by the 
client.

## Howto

From the ../rpl-border-router :

	make connect-router-cooja

From firefox (Optional): 

   http://[aaaa::212:7401:1:101]:80/ (6BR Root)


### Normal configuration for TinyDTLS 

TinyDTLS need to be "configured" for Contiki, this will generated the special Makefiles and preprare #define WITH_CONTIKI. 

    ./configure --with-contiki --target=sky --without-ecc --without-debug --build=x86_64-linux-gnu
    ./configure --with-contiki --target=sky --without-ecc --without-debug 
    ./configure --with-contiki
    ./configure --with-contiki --target=sky --without-psk --without-debug
    ./configure  --build=x86_64-linux-gnu
    ./configure  --build=x86_64-linux-gnu --without-ecc
    ./configure  --build=x86_64-linux-gnu --without-psk

  __NOTE__: Seems that TLS_NULL_WITH_NULL_NULL is not loaded in the client nor the server, yet is in the code.
  The client is only able to load PSK and/or ECC and the server only identify those two. 