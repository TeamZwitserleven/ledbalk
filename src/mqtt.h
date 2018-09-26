#pragma once

#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>

void connectToMqtt();
bool isMqttConnected();

void setupMqtt(const char *id);

void stopMqttReconnect();

void publishMqttMessage(const char *topic, uint8_t qos, bool retain, const char *payload);
