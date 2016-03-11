<?xml version="1.0" encoding="UTF-8"?>
<simconf>
  <project EXPORT="discard">[APPS_DIR]/mrm</project>
  <project EXPORT="discard">[APPS_DIR]/mspsim</project>
  <project EXPORT="discard">[APPS_DIR]/avrora</project>
  <project EXPORT="discard">[APPS_DIR]/serial_socket</project>
  <project EXPORT="discard">[APPS_DIR]/collect-view</project>
  <project EXPORT="discard">[APPS_DIR]/powertracker</project>
  <simulation>
    <title>TinyDTLS-Testing</title>
    <randomseed>123456</randomseed>
    <motedelay_us>1000000</motedelay_us>
    <radiomedium>
      org.contikios.cooja.radiomediums.UDGM
      <transmitting_range>50.0</transmitting_range>
      <interference_range>100.0</interference_range>
      <success_ratio_tx>1.0</success_ratio_tx>
      <success_ratio_rx>1.0</success_ratio_rx>
    </radiomedium>
    <events>
      <logoutput>40000</logoutput>
    </events>
    <motetype>
      org.contikios.cooja.mspmote.WismoteMoteType
      <identifier>wismote1</identifier>
      <description>TinyDTLS-Client</description>
      <source EXPORT="discard">[CONTIKI_DIR]/examples/ipv6/CoAP-IDS/tinydtls/dtls-client.c</source>
      <commands EXPORT="discard">make dtls-client.wismote TARGET=wismote</commands>
      <firmware EXPORT="copy">[CONTIKI_DIR]/examples/ipv6/CoAP-IDS/tinydtls/dtls-client.wismote</firmware>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.RimeAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspButton</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDefaultSerial</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspLED</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
    </motetype>
    <motetype>
      org.contikios.cooja.mspmote.WismoteMoteType
      <identifier>wismote2</identifier>
      <description>TinyDTLS-Server</description>
      <source EXPORT="discard">[CONTIKI_DIR]/examples/ipv6/CoAP-IDS/tinydtls/dtls-server.c</source>
      <commands EXPORT="discard">make dtls-server.wismote TARGET=wismote</commands>
      <firmware EXPORT="copy">[CONTIKI_DIR]/examples/ipv6/CoAP-IDS/tinydtls/dtls-server.wismote</firmware>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.RimeAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspButton</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDefaultSerial</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspLED</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
    </motetype>
    <motetype>
      org.contikios.cooja.mspmote.SkyMoteType
      <identifier>sky1</identifier>
      <description>6BR</description>
      <source EXPORT="discard">[CONTIKI_DIR]/examples/ipv6/rpl-border-router/border-router.c</source>
      <commands EXPORT="discard">make border-router.sky TARGET=sky</commands>
      <firmware EXPORT="copy">[CONTIKI_DIR]/examples/ipv6/rpl-border-router/border-router.sky</firmware>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.RimeAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyButton</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyFlash</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyCoffeeFilesystem</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspSerial</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyLED</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyTemperature</moteinterface>
    </motetype>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>49.158259264408805</x>
        <y>84.8066753058556</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>1</id>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspDefaultSerial
        <history>eader: (%zu bytes): 00000000 16 FE FD 00 00 00 00 00  00 00 07 00 36  DEBG retransmit unencrypted: (%zu bytes): 00000000 01 00 00 2A 00 00 00 00  00 00 00 2A FE FD 00 00  00000010 00 02 5A 8D 8B CB 68 34  81 30 26 25 67 95 14 2F  00000020 BD 53 B2 6D 03 21 80 28  B9 6E FE B2 5F CA 00 00  00000030 00 02 C0 A8 01 00  send to [fe80:0000:0000:0000:0200:000~;</history>
      </interface_config>
      <motetype_identifier>wismote1</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>29.60355830292546</x>
        <y>71.82355977872811</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>2</id>
      </interface_config>
      <motetype_identifier>wismote2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>64.12279847660403</x>
        <y>65.0837905310638</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>3</id>
      </interface_config>
      <motetype_identifier>sky1</motetype_identifier>
    </mote>
  </simulation>
  <plugin>
    org.contikios.cooja.plugins.SimControl
    <width>280</width>
    <z>3</z>
    <height>160</height>
    <location_x>400</location_x>
    <location_y>0</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.Visualizer
    <plugin_config>
      <moterelations>true</moterelations>
      <skin>org.contikios.cooja.plugins.skins.IDVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.AddressVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.MoteTypeVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.UDGMVisualizerSkin</skin>
      <viewport>2.7270540285563394 0.0 0.0 2.7270540285563394 68.79973284499756 -24.490232503003824</viewport>
    </plugin_config>
    <width>400</width>
    <z>4</z>
    <height>400</height>
    <location_x>2</location_x>
    <location_y>3</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.LogListener
    <plugin_config>
      <filter />
      <formatted_time />
      <coloring />
    </plugin_config>
    <width>1520</width>
    <z>1</z>
    <height>279</height>
    <location_x>404</location_x>
    <location_y>164</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.TimeLine
    <plugin_config>
      <mote>0</mote>
      <mote>1</mote>
      <mote>2</mote>
      <showRadioRXTX />
      <showRadioHW />
      <showLEDs />
      <zoomfactor>500.0</zoomfactor>
    </plugin_config>
    <width>1920</width>
    <z>7</z>
    <height>166</height>
    <location_x>-1</location_x>
    <location_y>979</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.Notes
    <plugin_config>
      <notes>This is a fast testing for TinyDTLS (0.8.2)  with the default example. 
however the project-conf.h and Makefile are custom 

the node 1 is a DTLS client
The node 2 is a DTLS server 
The node 3 is 6BR  node (which can connect exterior nodes to Cooja!)

The client default behavior is not firendly, it don't like to send too often. And if there is a problem is hard to catch.
If you want to use a extern client, like the test program that come with tinydtls you need to be sure 
that the motes and the extern program are compiled with the same pair of cipher suites,
 
Example: mote server: ./configure --with-contiki --without-ecc (plus check the apps/tinydtls/makefile.tinydtls )
               linux client: ./configure --without-ecc; make

If you are to use   a extern client,  remember that the 6BR will work like a bridge, so first connect to its web server to check
the valid routes, and remember that in Cooja you are watching the link-local scope but in the web interface you will
look the Global scope AND if you are using Wireshark to capture the traffic in the tunnel you will be watching the 
2002:db8:: scope (is academic?) this last chaos probably come from the 6BR configuration. 

A simple example of the chaos: 
 From the extern device: ping6 aaaa::200:0:0:2 
If you are capturing packets from the tunnel (or cooja) you will see that the 6BR is not using the aaaa:: scope but 
instead the 2001:db8:: scope. Technically not big issue as is normal in IPv6 domain.

If you don't want to confuse DO NOT CHANGE the default MAC addresses, neither move the motes in the Network's view (Cooja).

In the default configuration I'm able to connect to the server mote with this command:
      ./dtls-client aaaa::200:0:0:2 20220


For helping to be sure to which node are you connecting, check the "Via part" in the 6bR routes.

P.d.  The Radio messages (6LowPAN with pcap) are exported to a pcap file that is in the default location for Cooja
(normally at tools/cooja/build )</notes>
      <decorations>true</decorations>
    </plugin_config>
    <width>1240</width>
    <z>0</z>
    <height>160</height>
    <location_x>680</location_x>
    <location_y>0</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.RadioLogger
    <plugin_config>
      <split>150</split>
      <formatted_time />
      <showdups>false</showdups>
      <hidenodests>false</hidenodests>
      <analyzers name="6lowpan-pcap" />
    </plugin_config>
    <width>1509</width>
    <z>2</z>
    <height>518</height>
    <location_x>407</location_x>
    <location_y>451</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.serialsocket.SerialSocketServer
    <mote_arg>2</mote_arg>
    <plugin_config>
      <port>60002</port>
      <bound>true</bound>
    </plugin_config>
    <width>362</width>
    <z>6</z>
    <height>116</height>
    <location_x>16</location_x>
    <location_y>425</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.MoteInterfaceViewer
    <mote_arg>0</mote_arg>
    <plugin_config>
      <interface>Serial port</interface>
      <scrollpos>0,0</scrollpos>
    </plugin_config>
    <width>350</width>
    <z>5</z>
    <height>300</height>
    <location_x>25</location_x>
    <location_y>616</location_y>
  </plugin>
</simconf>

