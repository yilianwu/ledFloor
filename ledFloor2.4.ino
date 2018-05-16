#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define eightyPin 6
#define seventyPin 7
#define sixtyPin 8
#define sixtyPin1 9
#define fiftyPin 10
#define fiftyPin1 11
#define fiftyPin2 12
#define fiftyPin3 13
int myVibPin[] = {
  A0, A1, A2, A3, A4
};
int count = 360;
long tempResult[] = {0, 0, 0, 0, 0, 0, 0, 0};
long result = 0;
int backgnd = 30;
volatile unsigned long signal = -1;
volatile unsigned long time1 = -1;
volatile unsigned long time2 = -1;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(37, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip;
Adafruit_NeoPixel pixelStrings[] = {
  Adafruit_NeoPixel(48, eightyPin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(43, seventyPin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(37, sixtyPin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(37, sixtyPin1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, fiftyPin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, fiftyPin1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, fiftyPin2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, fiftyPin3, NEO_GRB + NEO_KHZ800),
};
#define NUMSTRIPS (sizeof(pixelStrings)/sizeof(pixelStrings[0]))
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  attachInterrupt(0, timing1, RISING);
  attachInterrupt(1, timing2, FALLING);
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  for (int v = 0; v < 4; v++) {
    pinMode(myVibPin[v], INPUT);
  }
  Serial.begin(9600);
  for (int i = 0; i < NUMSTRIPS; i++)
  {
    pixelStrings[i].begin();
    pixelStrings[i].show(); // Initialize all pixels to 'off'
  }
}

void loop() {
  //  Serial.print("vibration = ");
  //  Serial.println(measurement);
  breathing();
  //    colorWipe(strip.Color(255, 255, 255), 50);
}

//uint16_t n uint32_t c uint8_t wait

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void breathing() {
  for (int i = 6; i < 60 ; i += 6) {

    if (vibResult() > 300) {
      newRipple(i);
      result = 0;
    }

    else {
      for (int k = 0; k < NUMSTRIPS; k++) {
        for (int j = 0; j < pixelStrings[k].numPixels(); j++) {
          pixelStrings[k].setPixelColor(j, pixelStrings[k].Color(i, i , i));
        }
      }
      for (int i = 0; i < NUMSTRIPS; i++) {
        pixelStrings[i].show();
      }
      //      delay(25);
    }

  }
  for (int i = 60; i > 6; i -= 6) {

    if (vibResult() > 300) {
      newRipple(i);
      result = 0;
    }
    else {
      for (int k = 0; k < NUMSTRIPS; k++) {
        for (int j = 0; j < pixelStrings[k].numPixels(); j++) {
          pixelStrings[k].setPixelColor(j, strip.Color(i , i , i ));
        }
      }
      for (int i = 0; i < NUMSTRIPS; i++) {
        pixelStrings[i].show();
      }
      //      delay(25);
    }

  }



}
void timing1()
{
  time1 = micros();

}

void timing2()
{
  time2 = micros();
  signal = (time2 - time1) ;
}

void newRipple(int c) {

  for (int m = 0; m <= 50; m++) {
    for (int n = 0; n < NUMSTRIPS; n++) {
      for (int k = 0; k < m; k++) {
        pixelStrings[n].setPixelColor(k, pixelStrings[n].Color(c, c, c));
      }
      for (int k = strip.numPixels(); k > m; k--) {
        pixelStrings[n].setPixelColor(k, pixelStrings[n].Color(c, c, c));
      }
      pixelStrings[n].setPixelColor(m, pixelStrings[n].Color(255, 255, 255));
      pixelStrings[n].setPixelColor(m + 1, pixelStrings[n].Color(255, 255, 255));
      pixelStrings[n].setPixelColor(m + 2, pixelStrings[n].Color(255, 255, 255));
      //      pixelStrings[n].setPixelColor(m, pixelStrings[n].Color((26 - m) * (26 - m) * 9 / 25, (26 - m) * (26 - m) * 9 / 25, (26 - m) * (26 - m) * 9 / 25));
      //      pixelStrings[n].setPixelColor(m + 1, pixelStrings[n].Color((26 - m) * (26 - m) * 9 / 25, (26 - m) * (26 - m) * 9 / 25, (26 - m) * (26 - m) * 9 / 25));
      //      pixelStrings[n].setPixelColor(m + 2, pixelStrings[n].Color((26 - m) * (26 - m) * 9 / 25, (26 - m) * (26 - m) * 9 / 25, (26 - m) * (26 - m) * 9 / 25));
      //    strip.setPixelColor(m, strip.Color((26-m)*(26-m), (26-m)*(26-m), (26-m)*(26-m)));
      //    strip.setPixelColor(m+1, strip.Color((26-m)*(26-m), (26-m)*(26-m), (26-m)*(26-m)));
      //    strip.setPixelColor(m+2, strip.Color((26-m)*(26-m), (26-m)*(26-m), (26-m)*(26-m)));
      for (int p = 0; p < NUMSTRIPS; p++) {
        pixelStrings[p].show();
      }
      //      delay(40);
    }

  }
}
long vibResult() {
  for (int v = 0; v < 4; v++) {
    tempResult[v] = pulseIn(myVibPin[v], HIGH);
    //    Serial.print("tempResult");
    //    Serial.println(v);
    //    Serial.println(tempResult[v]);
    result += tempResult[v];
    //    Serial.println("------------------");
    //    Serial.println(result);

  }
  return result;
}

