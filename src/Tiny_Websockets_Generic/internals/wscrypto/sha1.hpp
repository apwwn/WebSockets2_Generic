/****************************************************************************************************************************
  sha1.hpp
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

#include <stdint.h>
#include <string.h>

/*
    CREDIT: this implementation is from https://github.com/983/SHA1
*/

namespace websockets2_generic
{
  namespace crypto2_generic
  {
    namespace internals2_generic
    {
      #define SHA1_HEX_SIZE (40 + 1)
      #define SHA1_BASE64_SIZE (28 + 1)
      
      class sha1
      {
        private:
          void add_byte_dont_count_bits(uint8_t x)
          {
            buf[i++] = x;
      
            if (i >= sizeof(buf))
            {
              i = 0;
              process_block(buf);
            }
          }
      
          static uint32_t rol32(uint32_t x, uint32_t n)
          {
            return (x << n) | (x >> (32 - n));
          }
      
          static uint32_t make_word(const uint8_t *p)
          {
            return
              static_cast<uint32_t>(p[0] << 3 * 8) |
              static_cast<uint32_t>(p[1] << 2 * 8) |
              static_cast<uint32_t>(p[2] << 1 * 8) |
              static_cast<uint32_t>(p[3] << 0 * 8);
          }
      
          void process_block(const uint8_t *ptr)
          {
            const uint32_t c0 = 0x5a827999;
            const uint32_t c1 = 0x6ed9eba1;
            const uint32_t c2 = 0x8f1bbcdc;
            const uint32_t c3 = 0xca62c1d6;
      
            uint32_t a = state[0];
            uint32_t b = state[1];
            uint32_t c = state[2];
            uint32_t d = state[3];
            uint32_t e = state[4];
      
            uint32_t w[16];
      
            for (int _i = 0; _i < 16; _i++)
              w[_i] = make_word(ptr + _i * 4);
      
      #define SHA1_LOAD(i) w[i&15] = rol32(w[(i+13)&15] ^ w[(i+8)&15] ^ w[(i+2)&15] ^ w[i&15], 1);
      #define SHA1_ROUND_0(v,u,x,y,z,i)              z += ((u & (x ^ y)) ^ y) + w[i&15] + c0 + rol32(v, 5); u = rol32(u, 30);
      #define SHA1_ROUND_1(v,u,x,y,z,i) SHA1_LOAD(i) z += ((u & (x ^ y)) ^ y) + w[i&15] + c0 + rol32(v, 5); u = rol32(u, 30);
      #define SHA1_ROUND_2(v,u,x,y,z,i) SHA1_LOAD(i) z += (u ^ x ^ y) + w[i&15] + c1 + rol32(v, 5); u = rol32(u, 30);
      #define SHA1_ROUND_3(v,u,x,y,z,i) SHA1_LOAD(i) z += (((u | x) & y) | (u & x)) + w[i&15] + c2 + rol32(v, 5); u = rol32(u, 30);
      #define SHA1_ROUND_4(v,u,x,y,z,i) SHA1_LOAD(i) z += (u ^ x ^ y) + w[i&15] + c3 + rol32(v, 5); u = rol32(u, 30);
      
            SHA1_ROUND_0(a, b, c, d, e,  0);
            SHA1_ROUND_0(e, a, b, c, d,  1);
            SHA1_ROUND_0(d, e, a, b, c,  2);
            SHA1_ROUND_0(c, d, e, a, b,  3);
            SHA1_ROUND_0(b, c, d, e, a,  4);
            SHA1_ROUND_0(a, b, c, d, e,  5);
            SHA1_ROUND_0(e, a, b, c, d,  6);
            SHA1_ROUND_0(d, e, a, b, c,  7);
            SHA1_ROUND_0(c, d, e, a, b,  8);
            SHA1_ROUND_0(b, c, d, e, a,  9);
            SHA1_ROUND_0(a, b, c, d, e, 10);
            SHA1_ROUND_0(e, a, b, c, d, 11);
            SHA1_ROUND_0(d, e, a, b, c, 12);
            SHA1_ROUND_0(c, d, e, a, b, 13);
            SHA1_ROUND_0(b, c, d, e, a, 14);
            SHA1_ROUND_0(a, b, c, d, e, 15);
            SHA1_ROUND_1(e, a, b, c, d, 16);
            SHA1_ROUND_1(d, e, a, b, c, 17);
            SHA1_ROUND_1(c, d, e, a, b, 18);
            SHA1_ROUND_1(b, c, d, e, a, 19);
            SHA1_ROUND_2(a, b, c, d, e, 20);
            SHA1_ROUND_2(e, a, b, c, d, 21);
            SHA1_ROUND_2(d, e, a, b, c, 22);
            SHA1_ROUND_2(c, d, e, a, b, 23);
            SHA1_ROUND_2(b, c, d, e, a, 24);
            SHA1_ROUND_2(a, b, c, d, e, 25);
            SHA1_ROUND_2(e, a, b, c, d, 26);
            SHA1_ROUND_2(d, e, a, b, c, 27);
            SHA1_ROUND_2(c, d, e, a, b, 28);
            SHA1_ROUND_2(b, c, d, e, a, 29);
            SHA1_ROUND_2(a, b, c, d, e, 30);
            SHA1_ROUND_2(e, a, b, c, d, 31);
            SHA1_ROUND_2(d, e, a, b, c, 32);
            SHA1_ROUND_2(c, d, e, a, b, 33);
            SHA1_ROUND_2(b, c, d, e, a, 34);
            SHA1_ROUND_2(a, b, c, d, e, 35);
            SHA1_ROUND_2(e, a, b, c, d, 36);
            SHA1_ROUND_2(d, e, a, b, c, 37);
            SHA1_ROUND_2(c, d, e, a, b, 38);
            SHA1_ROUND_2(b, c, d, e, a, 39);
            SHA1_ROUND_3(a, b, c, d, e, 40);
            SHA1_ROUND_3(e, a, b, c, d, 41);
            SHA1_ROUND_3(d, e, a, b, c, 42);
            SHA1_ROUND_3(c, d, e, a, b, 43);
            SHA1_ROUND_3(b, c, d, e, a, 44);
            SHA1_ROUND_3(a, b, c, d, e, 45);
            SHA1_ROUND_3(e, a, b, c, d, 46);
            SHA1_ROUND_3(d, e, a, b, c, 47);
            SHA1_ROUND_3(c, d, e, a, b, 48);
            SHA1_ROUND_3(b, c, d, e, a, 49);
            SHA1_ROUND_3(a, b, c, d, e, 50);
            SHA1_ROUND_3(e, a, b, c, d, 51);
            SHA1_ROUND_3(d, e, a, b, c, 52);
            SHA1_ROUND_3(c, d, e, a, b, 53);
            SHA1_ROUND_3(b, c, d, e, a, 54);
            SHA1_ROUND_3(a, b, c, d, e, 55);
            SHA1_ROUND_3(e, a, b, c, d, 56);
            SHA1_ROUND_3(d, e, a, b, c, 57);
            SHA1_ROUND_3(c, d, e, a, b, 58);
            SHA1_ROUND_3(b, c, d, e, a, 59);
            SHA1_ROUND_4(a, b, c, d, e, 60);
            SHA1_ROUND_4(e, a, b, c, d, 61);
            SHA1_ROUND_4(d, e, a, b, c, 62);
            SHA1_ROUND_4(c, d, e, a, b, 63);
            SHA1_ROUND_4(b, c, d, e, a, 64);
            SHA1_ROUND_4(a, b, c, d, e, 65);
            SHA1_ROUND_4(e, a, b, c, d, 66);
            SHA1_ROUND_4(d, e, a, b, c, 67);
            SHA1_ROUND_4(c, d, e, a, b, 68);
            SHA1_ROUND_4(b, c, d, e, a, 69);
            SHA1_ROUND_4(a, b, c, d, e, 70);
            SHA1_ROUND_4(e, a, b, c, d, 71);
            SHA1_ROUND_4(d, e, a, b, c, 72);
            SHA1_ROUND_4(c, d, e, a, b, 73);
            SHA1_ROUND_4(b, c, d, e, a, 74);
            SHA1_ROUND_4(a, b, c, d, e, 75);
            SHA1_ROUND_4(e, a, b, c, d, 76);
            SHA1_ROUND_4(d, e, a, b, c, 77);
            SHA1_ROUND_4(c, d, e, a, b, 78);
            SHA1_ROUND_4(b, c, d, e, a, 79);
      
      #undef SHA1_LOAD
      #undef SHA1_ROUND_0
      #undef SHA1_ROUND_1
      #undef SHA1_ROUND_2
      #undef SHA1_ROUND_3
      #undef SHA1_ROUND_4
      
            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;
            state[4] += e;
          }
      
        public:
      
          uint32_t state[5];
          uint8_t buf[64];
          uint32_t i;
          uint64_t n_bits;
      
          sha1(const char *text = NULL): i(0), n_bits(0)
          {
            state[0] = 0x67452301;
            state[1] = 0xEFCDAB89;
            state[2] = 0x98BADCFE;
            state[3] = 0x10325476;
            state[4] = 0xC3D2E1F0;
            add(text);
          }
      
          sha1& add(uint8_t x)
          {
            add_byte_dont_count_bits(x);
            n_bits += 8;
      
            return *this;
          }
      
          sha1& add(char c)
          {
            return add( static_cast<uint8_t>(c) );
          }
      
          sha1& add(const void *data, uint32_t n)
          {
            if (!data)
              return *this;
      
            const uint8_t *ptr = reinterpret_cast<const uint8_t*>(data);
      
            // fill up block if not full
            for (; n && i % sizeof(buf); n--)
              add(*ptr++);
      
            // process full blocks
            for (; n >= sizeof(buf); n -= sizeof(buf))
            {
              process_block(ptr);
              ptr += sizeof(buf);
              n_bits += sizeof(buf) * 8;
            }
      
            // process remaining part of block
            for (; n; n--)
              add(*ptr++);
      
            return *this;
          }
      
          sha1& add(const char *text)
          {
            return add(text, strlen(text));
          }
      
          sha1& finalize()
          {
            // hashed text ends with 0x80, some padding 0x00 and the length in bits
            add_byte_dont_count_bits(0x80);
      
            while (i % 64 != 56)
              add_byte_dont_count_bits(0x00);
      
            for (int j = 7; j >= 0; j--)
              add_byte_dont_count_bits(n_bits >> j * 8);
      
            return *this;
          }
      
          const sha1& print_hex(char *hex, bool zero_terminate = true, const char *alphabet = "0123456789abcdef") const
          {
            // print hex
            int k = 0;
      
            for (int _i = 0; _i < 5; _i++)
            {
              for (int j = 7; j >= 0; j--)
              {
                hex[k++] = alphabet[(state[_i] >> j * 4) & 0xf];
              }
            }
      
            if (zero_terminate)
              hex[k] = '\0';
      
            return *this;
          }
      
          const sha1& print_base64(char *base64, bool zero_terminate = true) const
          {
            static const uint8_t *table = reinterpret_cast<const uint8_t*>(
                                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                            "abcdefghijklmnopqrstuvwxyz"
                                            "0123456789"
                                            "+/"
                                          );
      
            uint32_t triples[7] =
            {
              ((state[0] & 0xffffff00) >> 1 * 8),
              ((state[0] & 0x000000ff) << 2 * 8) | ((state[1] & 0xffff0000) >> 2 * 8),
              ((state[1] & 0x0000ffff) << 1 * 8) | ((state[2] & 0xff000000) >> 3 * 8),
              ((state[2] & 0x00ffffff) << 0 * 8),
              ((state[3] & 0xffffff00) >> 1 * 8),
              ((state[3] & 0x000000ff) << 2 * 8) | ((state[4] & 0xffff0000) >> 2 * 8),
              ((state[4] & 0x0000ffff) << 1 * 8),
            };
      
            for (int _i = 0; _i < 7; _i++)
            {
              uint32_t x = triples[_i];
              base64[_i * 4 + 0] = table[(x >> 3 * 6) % 64];
              base64[_i * 4 + 1] = table[(x >> 2 * 6) % 64];
              base64[_i * 4 + 2] = table[(x >> 1 * 6) % 64];
              base64[_i * 4 + 3] = table[(x >> 0 * 6) % 64];
            }
      
            base64[SHA1_BASE64_SIZE - 2] = '=';
      
            if (zero_terminate)
              base64[SHA1_BASE64_SIZE - 1] = '\0';
      
            return *this;
          }
      };
    }     // namespace internals2_generic
  }       // namespace crypto2_generic
}         // namespace websockets2_generic
