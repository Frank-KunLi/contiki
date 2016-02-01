6LoWPAN (HC06 Compression) with UDP and CoAP
===================================================

OBJETIVO GENERAL
-------------
  Escenario de prueba donde habran nodos usando IPv6 con compresion HC06 y
  utilizando CoAP (bajo UDP normal y sin DTLS)
  
  Este escenario esta basado en el ejemplo original de examples/er-coap/* 
  
  Sus componentes claves:
      1 6LoWPAN  Border router (6BR)
      N Clientes CoAP basado en el "Erbium Example Client"
      1 Servidor CoAP (Solo de ejemplo)
      
  WARNING: El cliente original tiene el problema de que posee una IPv6 en
           hardware. Debo corregir eso a que pida lo pida como skywebsense.
  NOTE: El servidor no debería estar dentro del 6LoWPAN  y además tambien tiene
        IPv6 fija.
  NOTE: Los clientes no buscan info de temperatura, pero francamente puede
        quedarse con esa configuración.
  
  
PRELIMINARIES
------------- 

TODO

COOJA HOWTO
-----------

TODO (Inspirarme er-CoAP)

  La compilacion del proyecto es por separado, ya que se tiene el 6BR 
  proveniente del ejemplo exampleipv6/...
  
  TODO
  
  Una vez Cooaja arranque se debe  activar el serial sokect (Server) 
  Y desde consola (en el directorio Home del proyecto)  ejecutar lo siguiente:
  
    make connect-router-cooja
  
  Con este commando la maquina anfitriona puede conectarse a la red de sensores.
  
  NOTE: Es posible usar otras maquinas pero por defecto es la ::1 quien se
        conecta.

Firefox
-----------        
       
  Para poder indagar la informacion de los sensores se requiere CoAP,  existe el
  plugin Copper (Cu) para ello.    En la prueba dos nodos ejemplos listos son:
  
  http://[aaaa::212:7401:1:101]:80/ (6BR Root)
  coap://[aaaa::212:7402:2:202]:5683/ (Un cliente)
  
  NOTE: EL ejemplo del 6BR no tiene CoAP implementado por eso usa HTTP. 
  
TODOs
-----

  - La idea original incluye un dispositivo intermedio que hiciese traducciones 
  CoAP/HTTP, sin embargo, no esta aun implementado y el objetivo principal
  es desarrollar un IDS que solo esta dentro del 6LoWPAN.  Por consiguiente, 
  esto estara hasta el final. 
  
CoAP NOTES
_______


- Leer res-chunks.c
- Debo asegurarme que la aplicaicon mande todo con UDP comprimido tambien