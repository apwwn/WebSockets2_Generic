/****************************************************************************************************************************
  ws_common_UIPEthernet.hpp
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
 
#pragma once

#include <Tiny_Websockets_Generic/ws_config_defs.hpp>
#include <string>
#include <Arduino.h>

namespace websockets2_generic
{
  typedef std::string WSString;
  typedef String WSInterfaceString;
  
  namespace internals2_generic
  {
    WSString fromInterfaceString(const WSInterfaceString& str);
    WSString fromInterfaceString(const WSInterfaceString&& str);
    WSInterfaceString fromInternalString(const WSString& str);
    WSInterfaceString fromInternalString(const WSString&& str);
  }   // namespace internals2_generic 
}     // namespace websockets 2_generic

#if    ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )
         
      // Using Ethernet ENC28J60
      #warning Using UIPEthernet for SAMD in ws_common_UIPEthernet.hpp
      
      #define PLATFORM_DOES_NOT_SUPPORT_BLOCKING_READ
      #define _WS_CONFIG_NO_SSL   true
      
      #include <Tiny_Websockets_Generic/network/SAMD_UIPEthernet/SAMD_UIPEthernet_tcp.hpp>
      #define WSDefaultTcpClient websockets2_generic::network2_generic::EthernetTcpClient
      #define WSDefaultTcpServer websockets2_generic::network2_generic::EthernetTcpServer
      
      #ifndef _WS_CONFIG_NO_SSL
      // OpenSSL Dependent
      #define WSDefaultSecuredTcpClient websockets2_generic::network2_generic::SecuredEthernetTcpClient
      #endif //_WS_CONFIG_NO_SSL
      
  #elif ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
      defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
      defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
   
    // Using Ethernet ENC28J60
    #warning Using UIPEthernet for NRF52 in ws_common_UIPEthernet.hpp
    
    #define PLATFORM_DOES_NOT_SUPPORT_BLOCKING_READ
    #define _WS_CONFIG_NO_SSL   true
    
    #include <Tiny_Websockets_Generic/network/nRF52_UIPEthernet/nRF52_UIPEthernet_tcp.hpp>
    #define WSDefaultTcpClient websockets2_generic::network2_generic::EthernetTcpClient
    #define WSDefaultTcpServer websockets2_generic::network2_generic::EthernetTcpServer
    
    #ifndef _WS_CONFIG_NO_SSL
    // OpenSSL Dependent
    #define WSDefaultSecuredTcpClient websockets2_generic::network2_generic::SecuredEthernetTcpClient
    #endif //_WS_CONFIG_NO_SSL      
    
    
  #elif ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
    // From v1.0.1
  
    // Using Ethernet ENC28J60
    #warning Using UIPEthernet for SAM DUE in ws_common_UIPEthernet.hpp
    
    #define PLATFORM_DOES_NOT_SUPPORT_BLOCKING_READ
    #define _WS_CONFIG_NO_SSL   true
    
    #include <Tiny_Websockets_Generic/network/DUE_UIPEthernet/DUE_UIPEthernet_tcp.hpp>
    #define WSDefaultTcpClient websockets2_generic::network2_generic::EthernetTcpClient
    #define WSDefaultTcpServer websockets2_generic::network2_generic::EthernetTcpServer
    
    #ifndef _WS_CONFIG_NO_SSL
    // OpenSSL Dependent
    #define WSDefaultSecuredTcpClient websockets2_generic::network2_generic::SecuredEthernetTcpClient
    #endif //_WS_CONFIG_NO_SSL    
    
  #elif ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
          defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
          defined(STM32WB) || defined(STM32MP1) )
    // From v1.0.3
  
    // Using Ethernet ENC28J60
    #warning Using UIPEthernet for STM32 in ws_common_UIPEthernet.hpp
    
    #define PLATFORM_DOES_NOT_SUPPORT_BLOCKING_READ
    #define _WS_CONFIG_NO_SSL   true
    
    #include <Tiny_Websockets_Generic/network/STM32_UIPEthernet/STM32_UIPEthernet_tcp.hpp>
    #define WSDefaultTcpClient websockets2_generic::network2_generic::EthernetTcpClient
    #define WSDefaultTcpServer websockets2_generic::network2_generic::EthernetTcpServer
    
    #ifndef _WS_CONFIG_NO_SSL
    // OpenSSL Dependent
    #define WSDefaultSecuredTcpClient websockets2_generic::network2_generic::SecuredEthernetTcpClient
    #endif //_WS_CONFIG_NO_SSL     
    
  #endif  
