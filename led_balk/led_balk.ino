#include <FastLED.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      150

#define BRIGHTNESS 255

byte timeOfDay;
CRGB leds[NUMPIXELS];

#define TIMEOFDAYPIN1 11
#define TIMEOFDAYPIN2 12

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

  fill_gradient(colorSteps, STEPS*0, colorPalette[0], (STEPS*0)+(STEPS-1), colorPalette[1]);
  fill_gradient(colorSteps, STEPS*1, colorPalette[1], (STEPS*1)+(STEPS-1), colorPalette[2]);
  fill_gradient(colorSteps, STEPS*2, colorPalette[2], (STEPS*2)+(STEPS-1), colorPalette[3]);
  fill_gradient(colorSteps, STEPS*3, colorPalette[3], (STEPS*3)+(STEPS-1), colorPalette[0]);
}

void setup() {
  pinMode(TIMEOFDAYPIN1, INPUT_PULLUP);
  pinMode(TIMEOFDAYPIN2, INPUT_PULLUP);

  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
  showStaticColor(CRGB::Blue);
}

void loop() {
  static int s = 0;
  static bool gotSignal = 0;
  static bool b = 0;
  static int step = 0;
  static int lastChange = 0;
  static int interval = 50;

  byte tod = 0;
  if (digitalRead(TIMEOFDAYPIN1) == HIGH) {
    tod |= 0x01;
  }
  if (digitalRead(TIMEOFDAYPIN2) == HIGH) {
    tod |= 0x02;
  }

  //Serial.println(tod);
  if (tod != timeOfDay) {
    timeOfDay = tod;
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

