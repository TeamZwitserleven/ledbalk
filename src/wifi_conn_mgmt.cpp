#include <Ticker.h>

#include "wifi_conn_mgmt.h"
#include "mqtt.h"
#include "secrets.h"

static WiFiEventHandler wifiConnectHandler;
static WiFiEventHandler wifiDisconnectHandler;
static Ticker wifiReconnectTimer;
/*static String wifi_ssid;
static String wifi_pass;*/

void connectToWifi()
{
  //WiFi.disconnect();
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("WiFi lost connection");
  stopMqttReconnect();
  wifiReconnectTimer.once(2, connectToWifi);
}

// setupWifiManager creates a WifiManager and configures Wifi from
// eeprom, or creates a portal to allow a user to configure it.
void setupWifi()
{
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
}
