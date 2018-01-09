// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      160

#define BRIGHTNESS 255

byte timeOfDay;

#define TIMEOFDAYPIN1 11
#define TIMEOFDAYPIN2 12

#define MORNING   0x00
#define AFTERNOON 0x01 
#define EVENING   0x02
#define NIGHT     0x03

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 50; // delay for half a second

void setup() {
  pinMode(TIMEOFDAYPIN1, INPUT_PULLUP);
  pinMode(TIMEOFDAYPIN2, INPUT_PULLUP);

  //pixels.setBrightness(BRIGHTNESS);
  pixels.begin(); // This initializes the NeoPixel library.
}

#define NUMCOLORS 128
#define TODCOLORS (NUMCOLORS/4)
uint32_t colorTable[] = {
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7f7256,       0x7f7256,       0x7f7256,
        0x7f7256,       0x7a6d52,       0x76684e,       0x72644a,
        0x6e5f46,       0x6a5a43,       0x65553f,       0x61503b,
        0x5d4b37,       0x594733,       0x554230,       0x503d2c,
        0x4c3828,       0x483324,       0x442e20,       0x402a1d,
        0x3b2519,       0x372015,       0x331b11,       0x2f160d,
        0x2b110a,       0x260d06,       0x250b05,       0x290e05,
        0x2d1106,       0x301407,       0x341707,       0x381a08,
        0x3c1d09,       0x402009,       0x44230a,       0x47260b,
        0x4b290c,       0x4f2b0c,       0x532e0d,       0x57310e,
        0x5a340e,       0x5e370f,       0x623a10,       0x663d10,
        0x6a4011,       0x6e4312,       0x714613,       0x754913,
        0x774a14,       0x734713,       0x6f4412,       0x6b4111,
        0x673e11,       0x633b10,       0x60380f,       0x5c350f,
        0x58320e,       0x542f0d,       0x502c0c,       0x4c2a0c,
        0x49270b,       0x45240a,       0x41210a,       0x3d1e09,
        0x391b08,       0x361808,       0x321507,       0x2e1206,
        0x2a0f05,       0x260c05,       0x250b05,       0x291008,
        0x2d150c,       0x321910,       0x361e14,       0x3a2317,
        0x3e281b,       0x422d1f,       0x473223,       0x4b3627,
        0x4f3b2a,       0x53402e,       0x574532,       0x5c4a36,
        0x604f3a,       0x64533d,       0x685841,       0x6c5d45 };



            
void loop() {
  static int s = 0;
  static int sOffset = 0;
  static uint32_t c = 0x00FF00;
  static bool b = 0;

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
    s = 0;
    digitalWrite(LED_BUILTIN, b);
    b = !b;
    switch (timeOfDay) {
      case MORNING:
        sOffset = 0;
        //pixels.setBrightness(192);
        c = 0x6e4312;
        break;
      case AFTERNOON:
        sOffset = TODCOLORS;
        //pixels.setBrightness(255);
        c = 0x76684e;
        break;
      case EVENING:
        sOffset = TODCOLORS + TODCOLORS;
        //pixels.setBrightness(128);
        c = 0x3e281b;
        break;
      case NIGHT:
        sOffset = TODCOLORS + TODCOLORS + TODCOLORS;
        //pixels.setBrightness(64);
        c = 0x3e281b;
        break;
    }
    for (int i = 0; i < NUMPIXELS; i++) {
      //c = colorTable[/*s+*/ sOffset];
      pixels.setPixelColor(i, c);
    }
    pixels.show(); // This sends the updated pixel color to the hardware.
  }

  delay(500);

  if (s + 1 < TODCOLORS) {
    s++;
  }
}

