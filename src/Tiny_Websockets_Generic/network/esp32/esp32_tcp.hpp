/****************************************************************************************************************************
  esp32_tcp.hpp
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

#ifdef ESP32 

#include <Tiny_Websockets_Generic/internals/ws_common.hpp>
#include <Tiny_Websockets_Generic/network/tcp_client.hpp>
#include <Tiny_Websockets_Generic/network/tcp_server.hpp>
#include <Tiny_Websockets_Generic/network/generic_esp/generic_esp_clients.hpp>

#include <WiFi.h>
#include <HTTPClient.h>

namespace websockets2_generic
{
  namespace network2_generic
  {
    typedef GenericEspTcpClient<WiFiClient> Esp32TcpClient;
    
    class SecuredEsp32TcpClient : public GenericEspTcpClient<WiFiClientSecure>
    {
      public:
        void setCACert(const char* ca_cert)
        {
          this->client.setCACert(ca_cert);
        }
    
        void setCertificate(const char* client_ca)
        {
          this->client.setCertificate(client_ca);
        }
    
        void setPrivateKey(const char* private_key)
        {
          this->client.setPrivateKey(private_key);
        }

        void setInsecure()
        {
            this->client.setInsecure();
        }
    };
    
    
    class Esp32TcpServer : public TcpServer
    {
      public:
        Esp32TcpServer() {}
    
        bool poll() override
        {
          yield();
          return server.hasClient();
        }
    
        bool listen(const uint16_t port) override
        {
          yield();
          server = WiFiServer(port);
          server.begin(port);
          return available();
        }
           
        TcpClient* accept() override
        {
          while (available())
          {
            auto client = server.available();
    
            if (client)
            {
              return new Esp32TcpClient{client};
            }
            // KH, from v1.0.6, add to enable non-blocking when no WS Client
            else
            {
              // Return NULL Client. Remember to test for NULL and process correctly
              return NULL;
            }  
          }
          
          return new Esp32TcpClient;
        }
    
        bool available() override
        {
          yield();
          return static_cast<bool>(server);
        }
    
        void close() override
        {
          yield();
          server.close();
        }
    
        virtual ~Esp32TcpServer()
        {
          if (available()) 
            close();
        }
    
      protected:
        int getSocket() const override
        {
          return -1; // Not Implemented
        }
    
      private:
        WiFiServer server;
    };
  }   // namespace network2_generic
}     // namespace websockets2_generic

#endif // #ifdef ESP32 
