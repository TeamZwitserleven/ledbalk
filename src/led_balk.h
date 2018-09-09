#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

void setupLedBalk();
void loopLedBalk();

bool parseMessage(const JsonObject &source, byte* msg, int &msgLen, int maxMsgLen);
