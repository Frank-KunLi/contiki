6LoWPAN (HC06 Compression) with UDP and CoAP
Tentative:  Tiny-CoAPS
===================================================

OBJETIVO GENERAL
-------------
  Escenario de prueba donde habran nodos usando IPv6 con compresion HC06
  utilizando (Tiny)DTLS  y comunicación CoAP  
  Este escenario esta basado en el ejemplo original de examples/er-coap/* 
  Además  del ejecutado en Lithe (pero aguas que ya hay partes obsoletes)
  
  Sus componentes claves:
      1 6LoWPAN  Border router (6BR)
      N Clientes CoAPS basado en el "Erbium Example Client" y Lithe
      1 Servidor CoAP (Solo de ejemplo)
      
  WARNING: El cliente original tiene el problema de que posee una IPv6 en
           hardware. Debo corregir eso a que lo pida como skywebsense.
  NOTE: Los clientes no buscan info de temperatura, pero francamente puede
        quedarse con esa configuración.
  NOTE: Los componentes que intengran al servidor CoAP estarán a un minimo.
  
  
PRELIMINARIES
------------- 

TODO

COOJA HOWTO
-----------

TODO (Inspirarme en er-CoAP)

  La compilacion del proyecto es por separado, ya que se tiene el 6BR 
  proveniente del ejemplo:
     contiki/examples/ipv6/rpl-border-router/border-router.c
  
  Mientras que el cliente proviene de esta carpeta (Junto a la carpeta sources)
  El servidor tambien estará en esta carpeta
  
  TODO
  
  Una vez Cooaja arranque se debe  activar el serial sokect (Server) 
  Y desde consola (en el directorio Home del proyecto)  ejecutar lo siguiente:
  
    make connect-router-cooja
  
  Con este commando la maquina anfitriona puede conectarse a la red de sensores.
  
  NOTE: Es posible usar otras maquinas pero por defecto es la ::1 quien se
        conecta.
  WARNING: ESto solo servirá ANTES de integrar CoAPS 

Firefox
-----------        
       
  Para poder indagar la informacion de los sensores se requiere CoAP,  existe el
  plugin Copper (Cu) para ello.    En la prueba dos nodos ejemplos listos son:
  
  http://[aaaa::212:7401:1:101]:80/ (6BR Root)
  coap://[aaaa::212:7402:2:202]:5683/ (Un cliente)
  
  NOTE: EL ejemplo del 6BR no tiene CoAP implementado por eso usa HTTP. 
  
TinyDTLS
-----------

  Esto se hace una unica vez, descargar la ultima version de TinyDTLS. 
  El actual sitio oficial es de Eclipse: 
     https://projects.eclipse.org/proposals/tinydtls
  Pero al momento de esto, el código liberado sigue siendo el original
  en sourceforge:
     http://sourceforge.net/p/tinydtls/code/ci/master/tree/
  
  Para esto, se trata de Downloads/tinydtls-0.8.2.tar.gz
  TinyDTLS se debe de instalar en contiki/Apps. 
  Seguir la info de contiki/apps/tinydtls/doc/html/index.html:
  
  To use tinyDTLS as Contiki application, place the source code in the directory
  apps/tinydtls in the Contiki source tree and invoke configure with the option
  -with-contiki. This will define WITH_CONTIKI in tinydtls.h and include 
  Makefile.contiki in the main Makefile. To cross-compile for another platform
  you will need to set your host and build system accordingly. For example, 
  when configuring for ARM, you would invoke 
  
     ./configure --with-contiki --build=x86_64-linux-gnu --host=arm-none-eabi 

      on an x86_64 linux host.

      Then, create a Contiki project with APPS += tinydtls in its Makefile
  
TODOs
-----

  He aqui un problema. Tengo dos Apps que deben funcionar como una sola: 
  er-coap y tinydtls.
  Lo mas sano sera crear una tercera apps con las librerias de ambos 
  Asegurarme que compile y de ahi ir adpatando er-coap send y receive 
  con  TinyDTLS. 
  
  El problema de overflow que los ejemplos de tinyDTLS provocan   no lo he 
  podido ubicar pero no creo que se deba a que la lbireria es grande
  si no alguna falla en esos dos codigos.
  
  2016: 
  Basandome en Lithe ya he podido identificar  los elementos claves. 
  TinyDTLS y ER-Coap seran una tercera carpeta (sano para evitar tragedias con
  "Git pull"). 
  
  Creo que el unico que requiere sufrir cambios es er-coap. Posiblemente 
  renombrarlo a tiny-coap para diferenciar la libreria original de mi ad-hoc.
  
  Los archivos originales de tinydtls son muy complejos pues no esta pensado
  solo para Contiki, ir eliminando algunos de ellos (Iniciando con examples
  y tests)
  