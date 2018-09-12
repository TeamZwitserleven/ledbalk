#define FASTLED_ESP8266_RAW_PIN_ORDER

#include "led_balk.h"
#include <FastLED.h>

static const String msgTypeTimeChanged = "time-changed";

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            D1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS    150

#define BRIGHTNESS 255

byte receivedHour = 0;
CRGB leds[NUMPIXELS];

static CHSV colorPalette[] {
  CHSV(140, 140, 120),  // Night
  CHSV(68, 200, 180),   // Morning
  CHSV(64, 40, 255),    // Afternoon
  CHSV(25, 240, 240)    // Evening 
};

#define STEPS 36
static CHSV colorSteps[STEPS * 4];
#define STEPSPERHOUR ((STEPS*4)/24)

static void showStaticColor(CRGB first, CRGB c) {
  if (NUMPIXELS > 0) {
    fill_solid(&leds[1], NUMPIXELS-1, c);
  }
  fill_solid(leds, 1, first);
  FastLED.show();
}

void setupLedBalk() {
  Serial.println("setupLedBalk #1");
  pinMode(PIN, OUTPUT);
  Serial.println("setupLedBalk #2");

  fill_gradient(colorSteps, STEPS*0, colorPalette[0], (STEPS*0)+(STEPS-1), colorPalette[1]);
  fill_gradient(colorSteps, STEPS*1, colorPalette[1], (STEPS*1)+(STEPS-1), colorPalette[2]);
  fill_gradient(colorSteps, STEPS*2, colorPalette[2], (STEPS*2)+(STEPS-1), colorPalette[3]);
  fill_gradient(colorSteps, STEPS*3, colorPalette[3], (STEPS*3)+(STEPS-1), colorPalette[0]);
  Serial.println("setupLedBalk #4");

  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
  Serial.println("setupLedBalk #5");
  showStaticColor(CRGB::Red, CRGB::Blue);
  Serial.println("setupLedBalk #6");
}

void loopLedBalk() {
  static int s = 0;
  static bool gotSignal = 0;
  static bool b = 0;
  static int step = 0;
  static int lastChange = 0;
  static int interval = 750;
  static int hour = 0;

  //Serial.println(tod);
  if (receivedHour != hour) {
    hour = receivedHour;
    gotSignal = 1;
    digitalWrite(LED_BUILTIN, b);
    b = !b;
    int now = millis();
    if (lastChange != 0) {
      interval = (now - lastChange) / STEPSPERHOUR;
    }
    lastChange = now;
    step = 0;
  }

  int offset = (gotSignal) ? (hour * STEPSPERHOUR) : (s * STEPSPERHOUR);
  showStaticColor(CRGB::Red, colorSteps[offset + step]);
  if (step + 1 < STEPSPERHOUR) {
    step++;    
  } else {
    if (!gotSignal) {
      s = (s+1) % 24;
      step = 0;
    }
  }
  delay(interval);
}

bool parseMessage(const JsonObject &source, byte* msg, int &msgLen, int maxMsgLen) {
  const String& msgType = source["type"];
  if (msgType.equals(msgTypeTimeChanged)) {
    uint8_t hour = source["hour"] | 0;
    //uint8_t minute = source["minute"] | 0;

    if ((hour >= 0) && (hour < 24)) {
      receivedHour = hour;
    }
    Serial.print("Set timeofday to ");
    Serial.print(receivedHour);
    Serial.println();
    return true;
  }
  return false;
}
