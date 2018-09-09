/*
This example uses FreeRTOS softwaretimers as there is no built-in Ticker library
*/

#include "heartbeat.h"
#include "mqtt.h"
#include "led_balk.h"
#include "wifi_conn_mgmt.h"

static word lastHeartbeat = 0;
static char id[20];

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println("Beginning ...");

  uint64_t chipid = ESP.getChipId();
  sprintf(id, "led-%04X", (uint16_t)(chipid>>32)); //print High 2 bytes
  sprintf(id+4, "%08X", (uint32_t)chipid);//print Low 4bytes.

  setupLedBalk();
  setupHeartbeat();

  setupWifi();
  Serial.println("setupWifi completed.");
  setupMqtt(id);
  Serial.println("setupMqtt completed.");

  connectToWifi();
  Serial.println("connectToWifi completed.");
}

void loop() {
  loopHeartbeat(id);
  loopLedBalk();
}