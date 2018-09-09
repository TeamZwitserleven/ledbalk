#pragma once

#include <ESP8266WiFi.h>

// connectToWifi begins connecting to WIFI.
void connectToWifi();

// setupWifiManager creates a WifiManager and configures Wifi from
// eeprom, or creates a portal to allow a user to configure it.
void setupWifi();
