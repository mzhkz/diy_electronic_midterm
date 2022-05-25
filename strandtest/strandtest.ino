

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 6
#define LED_COUNT 4


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.begin(115200);     // シリアル通信を初期化する。通信速度は9600bps
}



void loop() {
  if (Serial.available() > 0){
    String data = Serial.readString();
    int arg = data.substring(0,1).toInt();
    int percentage = data.substring(2).toInt();
    Serial.println(percentage);
    for (int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(150, 150, (int) (percentage / 100 * 255)));
    }
    strip.show();
    delay(100);
    strip.clear();
  }
}
