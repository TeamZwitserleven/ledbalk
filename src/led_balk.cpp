#define FASTLED_ESP8266_RAW_PIN_ORDER

#include "led_balk.h"
#include <FastLED.h>

static const String msgTypeTimeChanged = "time-changed";

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            D1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS    3//  150

#define BRIGHTNESS 255

byte timeOfDay;
byte receivedTimeOfDay;
CRGB leds[NUMPIXELS];

#define MORNING   0x00
#define AFTERNOON 0x01 
#define EVENING   0x02
#define NIGHT     0x03

static CHSV colorPalette[] {
  CHSV(68, 200, 180), // Morning
  CHSV(64, 40, 255), // Afternoon
  CHSV(25, 240, 240), // Evening 
  CHSV(140, 140, 120)  // Night
};

#define STEPS 32
static CHSV colorSteps[STEPS * 4];

static void showStaticColor(CRGB c) {
  fill_solid(leds, NUMPIXELS, c);
  FastLED.show();

}

void setupLedBalk() {
  Serial.println("setupLedBalk #1");
  pinMode(PIN, OUTPUT);
  Serial.println("setupLedBalk #2");

  fill_gradient(colorSteps, STEPS*0, colorPalette[0], (STEPS*0)+(STEPS-1), colorPalette[1]);
  Serial.println("setupLedBalk #3");
  fill_gradient(colorSteps, STEPS*1, colorPalette[1], (STEPS*1)+(STEPS-1), colorPalette[2]);
  fill_gradient(colorSteps, STEPS*2, colorPalette[2], (STEPS*2)+(STEPS-1), colorPalette[3]);
  fill_gradient(colorSteps, STEPS*3, colorPalette[3], (STEPS*3)+(STEPS-1), colorPalette[0]);
  Serial.println("setupLedBalk #4");

  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
  Serial.println("setupLedBalk #5");
  showStaticColor(CRGB::Blue);
  Serial.println("setupLedBalk #6");
}

void loopLedBalk() {
  static int s = 0;
  static bool gotSignal = 0;
  static bool b = 0;
  static int step = 0;
  static int lastChange = 0;
  static int interval = 50;

  //Serial.println(tod);
  if (receivedTimeOfDay != timeOfDay) {
    timeOfDay = receivedTimeOfDay;
    gotSignal = 1;
    digitalWrite(LED_BUILTIN, b);
    b = !b;
    int now = millis();
    if (lastChange != 0) {
      interval = (now - lastChange) / STEPS;
    }
    lastChange = now;
    step = 0;
  }

  int offset = (gotSignal) ? (timeOfDay * STEPS) : (s * STEPS);
  showStaticColor(colorSteps[offset + step]);
  if (step + 1 < STEPS) {
    step++;    
  } else {
    if (!gotSignal) {
      s = (s+1) % 4;
      step = 0;
    }
  }
  delay(interval);
}

bool parseMessage(const JsonObject &source, byte* msg, int &msgLen, int maxMsgLen) {
  const String& msgType = source["type"];
  if (msgType.equals(msgTypeTimeChanged)) {
    uint8_t hour = source["hour"] | 0;
    uint8_t minute = source["minute"] | 0;

    if ((hour >= 7) && (hour < 12)) {
      receivedTimeOfDay = MORNING;  
    } else if (hour < 18) {
      receivedTimeOfDay = AFTERNOON;
    } else if (hour < 23) {
      receivedTimeOfDay = EVENING;
    } else {
      receivedTimeOfDay = NIGHT;
    }
    Serial.print("Set timeofday to ");
    Serial.print(receivedTimeOfDay);
    Serial.println();
    return true;
  }
  return false;
}
