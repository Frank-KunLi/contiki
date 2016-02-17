
 
######################################################################################## 
 Observacioens de dtls.c/h
  - Ya invoca debug.h 
  - Para el conf.h es posible usar WITH_SHA256 = 1 
     Wait... WITH_CONTIKI tambien aparece, será para el ./configure ?
  - Hay una serie de Define  para defitinr dtls  version (se obtendra del
    Makefile original?)
  - 
  
  Ok, hay algo raro en linea 124, la estructura cert_asn1_header  es hardcoded 
  de una llave hay que validad que hace esto.
  
  Ok, en estos momentos TinyDTLS no posee ningun tipo de compresion (Bueno si)
  solo soporta TLS_COMPRESSION_NULL 
  
  Ciphers soportados: 
    TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8
    TLS_PSK_WITH_AES_128_CCM_8
    TLS_NULL_WITH_NULL_NULL
  Por consiguiente puedes configurar TinyDTLS para PSK  y ECDHE-ECDSA
  (Validar el paper de performance con estos numeros)
  PERO parece que deben ser definidos (#def DTLS_PSK ó DTLS_ECC )
  No son mutuamente excluyentes, se podría validar el impacto en memoria con uno 
  o los dos (pero francamente, solo debería ser uno)
  
  Si soporta todos los tipos de mensajes para el handshake (o al menos los 
  reconoce)
  Tambien los relacionae al ECDHE_ECDSA cipher.
  
  MMM el código  maneja la posibilidad de enviar alertas de 
  DTLS_ALERT_INSUFFICIENT_SECURITY  cuando todo falla (incluyend oque no se acepte
  TLS_COMPRESSION_NULL) aunque el RFC pide no enviar estos mensajes creo que
  que aquí no esta  del todo incorrecto.
  
  check_server_key_exchange_psk( ... )   al parecer  si pueden enviar esos 
  mensjaes
  
  Curioso, TinyDTLS soporta los mensajes Finished, tengo que averiguar porque
  no los envía
  
  
  En el h todo parece estar en orden, aunque los #define estan al medio 
############################################################################################

######################################################################################## 
 Observacioens de dtls_config
 
  AJA! Aqui estan  varias cosas comentadas
  Lo curioso es que DTLS_ECC y DTLS_PSK estan para ser compilados, sin embargo en las pruebas
  solo PSK aparecía como opción del cliente...
  
  mm capaz que esto se altera c on el ./configure YEP. Este archivo se modifica de acuerdo
  a las configuraciones del ./configure por lo tanto, no debe de ser considerado.
  
  ./configure --with-contiki  --target=sky --without-ecc --without-debug --build=x86_64-linux-gnu
  ./configure --with-contiki  --target=sky --without-ecc --without-debug 
  ./configure  --build=x86_64-linux-gnu
  ./configure  --build=x86_64-linux-gnu --without-ecc
  ./configure  --build=x86_64-linux-gnu --without-ecc

  NOTA: AL parecer TLS_NULL_WITH_NULL_NULL no es aceptado en el cliente ni server
  PERO esta en el codigo
  Ok, encontrado el problema. El lciente solo puede generar PSK o ECC  y el server
  solo puede reconocer esos dos. O sea, NULL_NULL nunca sera  una opcion valida de cipher suite.
######################################################################################## 
 Observacioens de dtls_time.c|h
 
  No estoy seguro pero croe que WITH_CONTIKI DTLS_ECC DTLS_PSK son para los conf.h
  o Makefile porque PERO debe haber algo que los active o no.  LS Test solo me 
  reconocen un unico cipher suite no los dos (Client Hello messg)
 
 
 