

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 6
#define LED_COUNT 4
int tick = 0;
int prev_arg = 0;
int pointer = 0;
int ticks = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
//  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.begin(115200);     // シリアル通信を初期化する。通信速度は9600bps
}

void loop() {
  if (Serial.available() > 0){
    strip.clear();
    String data = Serial.readString();
    int arg = data.substring(0,1).toInt();
    int percentage = data.substring(2).toInt();
    int red = (int) (percentage / 100.0 * 255.0);
    int blue = (int) 255 - (percentage / 100.0 * 255.0);

    if (arg == 1) {
      pointer +=1;
      if (prev_arg == 0) {
        pointer = 0;
      }
    } else if(arg == 2) {
      pointer -=1;
      if (prev_arg == 0) {
        pointer = LED_COUNT-1;
      }
    } else if (arg == 0) {
      pointer = -2;
    }

    prev_arg = arg;

    if (pointer == -2) {
      for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(red, blue, 0));
      }
    } else {
      if (pointer < 0) {
        pointer = LED_COUNT-1;
      } else if (pointer >= LED_COUNT) {
        pointer = 0;
      }
      strip.setPixelColor(pointer, strip.Color(red, blue, 0));
    }
    Serial.println(arg);
    strip.show();
  }
}
