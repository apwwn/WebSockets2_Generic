/****************************************************************************************************************************
  STM32-Ethernet-Client_SINRIC_LAN8720.ino
  For STM32F/L/H/G/WB/MP1 with Ethernet module/shield.
  
  Based on and modified from Gil Maimon's ArduinoWebsockets library https://github.com/gilmaimon/ArduinoWebsockets
  to support STM32F/L/H/G/WB/MP1, nRF52 and SAMD21/SAMD51 boards besides ESP8266 and ESP32


  The library provides simple and easy interface for websockets (Client and Server).

  Built by Khoi Hoang https://github.com/khoih-prog/Websockets2_Generic
  Licensed under MIT license
 *****************************************************************************************************************************/
/****************************************************************************************************************************
  STM32 Websockets SINRIC Client

  This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets SINRIC server
        3. Control the LED_BUILTIN using SINRIC/Alexa commands

  Hardware:
        For this sketch you only need a STM32 board.

*****************************************************************************************************************************/

#include "defines.h"

#define DEBUG_LOCAL   2

#include <WebSockets2_Generic.h>
#include <ArduinoJson.h>          // https://arduinojson.org/ or install via Arduino library manager

using namespace websockets2_generic;

WebsocketsClient* client;

#define HEARTBEAT_INTERVAL      300000 // 5 Minutes 

bool isConnected                = false;
uint64_t heartbeatTimestamp     = 0;
uint64_t now                    = 0;

//To increase no of devices as needed
void turnOn(String deviceId)
{
  if (deviceId == SINRIC_Device_ID_1) // Device ID of 1st device
  {
#if (DEBUG_LOCAL > 1)
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
#endif
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
#if (DEBUG_LOCAL > 1)
    Serial.print("Turn on for unknown device id: ");
    Serial.println(deviceId);
#endif
  }
}

//To increase no of devices as needed
void turnOff(String deviceId)
{

  if (deviceId == SINRIC_Device_ID_1) // Device ID of 1st device
  {
#if (DEBUG_LOCAL > 1)
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
#endif
    digitalWrite(LED_PIN, LOW);
  }
  else
  {
#if (DEBUG_LOCAL > 1)
    Serial.print("Turn off for unknown device id: ");
    Serial.println(deviceId);
#endif
  }
}

void onEventsCallback(WebsocketsEvent event, String data)
{
  if (event == WebsocketsEvent::ConnectionOpened)
  {
    if (!isConnected)
      isConnected = true;

    Serial.println("Connnection Opened");
  }
  else if (event == WebsocketsEvent::ConnectionClosed)
  {
    if (isConnected)
      isConnected = false;

    Serial.println("Connnection Closed");
  }
  else if (event == WebsocketsEvent::GotPing)
  {
    if (!isConnected)
      isConnected = true;

    Serial.println("Got a Ping!");
  }
  else if (event == WebsocketsEvent::GotPong)
  {
    if (!isConnected)
      isConnected = true;

    Serial.println("Got a Pong!");
  }
}

void onMessagesCallback(WebsocketsMessage message)
{
  String SINRIC_message = message.data();
  
  Serial.print("Got Message: ");
  Serial.println(SINRIC_message /*message.data()*/);

  // Example payloads

  // For Switch or Light device types
  // {"deviceId": xxxx, "action": "setPowerState", value: "ON"} // https://developer.amazon.com/docs/device-apis/alexa-powercontroller.html

  // For Light device type
  // Look at the light example in github

#if (ARDUINOJSON_VERSION_MAJOR >= 6)
  DynamicJsonDocument json(1024);
  //auto deserializeError = deserializeJson(json, (char*)message.data());
  auto deserializeError = deserializeJson(json, SINRIC_message);
  
  if ( deserializeError )
  {
    Serial.println("JSON parseObject() failed");
    return;
  }
  //serializeJson(json, Serial);
#else
  DynamicJsonBuffer jsonBuffer;
  // Parse JSON string
  JsonObject& json = jsonBuffer.parseObject(SINRIC_message);
  // Test if parsing succeeds.
  if (!json.success())
  {
    Serial.println("JSON parseObject() failed");
    return;
  }
#endif

  String deviceId = json ["deviceId"];
  String action = json ["action"];

  if (action == "setPowerState")
  {
    // Switch or Light
    String value = json ["value"];
    if (value == "ON")
    {
      turnOn(deviceId);
    }
    else
    {
      turnOff(deviceId);
    }
  }
}

void setup()
{
#if (USE_ETHERNET || USE_ETHERNET2 || USE_ETHERNET_LARGE)
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card
#endif
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.println("\nStarting STM32-Ethernet-Client_SINRIC_LAN8720 on " + String(BOARD_NAME));
  Serial.println("Ethernet using " + String(ETHERNET_TYPE));
  Serial.println(WEBSOCKETS2_GENERIC_VERSION);

#if ( USE_ETHERNET || USE_ETHERNET_LARGE || USE_ETHERNET2 || USE_ETHERNET_ENC )
  // Must use library patch for Ethernet, EthernetLarge libraries
  //Ethernet.setCsPin (USE_THIS_SS_PIN);
  Ethernet.init (USE_THIS_SS_PIN);
#endif  // ( USE_ETHERNET || USE_ETHERNET_LARGE || USE_ETHERNET2 || USE_ETHERNET_ENC )

  // start the ethernet connection and the server:
  // Use Static IP
  //Ethernet.begin(mac, clientIP);
  //Configure IP address via DHCP
  Ethernet.begin(mac);

  Serial.print("WebSockets Client IP address: ");
  Serial.println(Ethernet.localIP());

  Serial.print("Connecting to WebSockets Server @");
  Serial.println(websockets_server_host);

  client = new WebsocketsClient;

  // run callback when events are occuring
  client->onMessage(onMessagesCallback);

  // try to connect to Websockets server
  client->addHeader("apikey", SINRIC_API_KEY);
  client->setAuthorization("apikey", SINRIC_API_KEY);

  bool connected = client->connect(websockets_server_host, websockets_server_port, "/");

  if (connected)
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.println("Not Connected!");
  }
}

void loop()
{
  // let the websockets client check for incoming messages
  if (client->available())
  {
    client->poll();

    now = millis();

    // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
    if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL)
    {
      heartbeatTimestamp = now;
      client->send("H");
    }
  }
}
