/****************************************************************************************************************************
  WebSockets2_Generic_Debug.h
  For WebSockets2_Generic Library
  
  Based on and modified from Gil Maimon's ArduinoWebsockets library https://github.com/gilmaimon/ArduinoWebsockets
  to support STM32F/L/H/G/WB/MP1, nRF52, SAMD21/SAMD51, SAM DUE, Teensy boards besides ESP8266 and ESP32

  The library provides simple and easy interface for websockets (Client and Server).
  
  Built by Khoi Hoang https://github.com/khoih-prog/Websockets2_Generic
  Licensed under MIT license
  Version: 1.2.3

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      14/07/2020 Initial coding/porting to support nRF52 and SAMD21/SAMD51 boards. Add SINRIC/Alexa support
  1.0.1   K Hoang      16/07/2020 Add support to Ethernet W5x00 to nRF52, SAMD21/SAMD51 and SAM DUE boards
  1.0.2   K Hoang      18/07/2020 Add support to Ethernet ENC28J60 to nRF52, SAMD21/SAMD51 and SAM DUE boards
  1.0.3   K Hoang      18/07/2020 Add support to STM32F boards using Ethernet W5x00, ENC28J60 and LAN8742A 
  1.0.4   K Hoang      27/07/2020 Add support to STM32F/L/H/G/WB/MP1 and Seeeduino SAMD21/SAMD51 using 
                                  Ethernet W5x00, ENC28J60, LAN8742A and WiFiNINA. Add examples and Packages' Patches.
  1.0.5   K Hoang      29/07/2020 Sync with ArduinoWebsockets v0.4.18 to fix ESP8266 SSL bug.
  1.0.6   K Hoang      06/08/2020 Add non-blocking WebSocketsServer feature and non-blocking examples.       
  1.0.7   K Hoang      03/10/2020 Add support to Ethernet ENC28J60 using EthernetENC and UIPEthernet v2.0.9
  1.1.0   K Hoang      08/12/2020 Add support to Teensy 4.1 using NativeEthernet  
  1.2.0   K Hoang      16/04/2021 Add limited support (client only) to ESP32-S2 and LAN8720 for STM32F4/F7
  1.2.1   K Hoang      16/04/2021 Add support to new ESP32-S2 boards. Restore Websocket Server function for ESP32-S2.
  1.2.2   K Hoang      16/04/2021 Add support to ESP32-C3
  1.2.3   K Hoang      02/05/2021 Update CA Certs and Fingerprint for EP32 and ESP8266 secured exampled.
 *****************************************************************************************************************************/

#ifndef WebSockets2_Generic_Debug_h
#define WebSockets2_Generic_Debug_h

#pragma once

#include <stdio.h>

#ifdef DEBUG_WEBSOCKETS_PORT
  #define DBG_PORT      DEBUG_WEBSOCKETS_PORT
#else
  #define DBG_PORT      Serial
#endif

// Change _WEBSOCKETS_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _WEBSOCKETS_LOGLEVEL_
  #define _WEBSOCKETS_LOGLEVEL_       2
#endif


#define LOGERROR(x)     if(_WEBSOCKETS_LOGLEVEL_>0) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define LOGERROR1(x,y)  if(_WEBSOCKETS_LOGLEVEL_>0) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }

#define LOGWARN(x)      if(_WEBSOCKETS_LOGLEVEL_>1) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define LOGWARN1(x,y)   if(_WEBSOCKETS_LOGLEVEL_>1) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }

#define LOGINFO(x)      if(_WEBSOCKETS_LOGLEVEL_>2) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define LOGINFO1(x,y)   if(_WEBSOCKETS_LOGLEVEL_>2) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define LOGINFO2(x,y,z) if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.println(z); }
#define LOGINFO3(x,y,z,w) if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.print(z); DBG_PORT.print(" "); DBG_PORT.println(w); }

#define LOGDEBUG(x)      if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.println(x); }
#define LOGDEBUG0(x)     if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); }
#define LOGDEBUG1(x,y)   if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.println(y); }
#define LOGDEBUG2(x,y,z) if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.println(z); }
#define LOGDEBUG3(x,y,z,w) if(_WEBSOCKETS_LOGLEVEL_>3) { DBG_PORT.print("[WS] "); DBG_PORT.print(x); DBG_PORT.print(" "); DBG_PORT.print(y); DBG_PORT.print(" "); DBG_PORT.print(z); DBG_PORT.print(" "); DBG_PORT.println(w); }


#endif    //WebSockets2_Generic_Debug_h
