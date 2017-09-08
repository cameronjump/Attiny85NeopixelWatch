#include <SendOnlySoftwareSerial.h>
#include <TinyWireM.h>
#include "TinyRTClib.h"


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Pins 0 and 2 SDA SCL for DS1307
#define NEOPIXEL 1
#define BUTTON 4
#define TEMP 3

boolean state = true;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, NEOPIXEL, NEO_RGBW + NEO_KHZ800);

//SendOnlySoftwareSerial mySerial = SendOnlySoftwareSerial(1);

RTC_DS1307 RTC;

  uint32_t orange = strip.Color(0, 255, 0, 0);
  uint32_t blue = strip.Color(0, 0, 255,0);
  uint32_t red = strip.Color(0,255,0,0);

void setup () {
    #if defined (__AVR_ATtiny85__)
     if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif

    //mySerial.begin(9600);

    pinMode(TEMP,INPUT);
    pinMode(BUTTON, INPUT_PULLUP);
    
    TinyWireM.begin();
    RTC.begin();

    if (! RTC.isrunning()) {
      //mySerial.println("RTC is NOT running!");
      //RTC.adjust(DateTime(2017, 9, 5, 6, 30, 0));
    }

    strip.begin();
    strip.show();
    strip.setBrightness(25);
}

void loop () {
    if(state) {
      time();
    }
    else {
      temperature();
    }
    if(digitalRead(BUTTON) == LOW) {
      state = !state;
      clear();
    }
    delay(200);
}

void clear() {
    for(int i=0; i<12; i++) {
        strip.setPixelColor(i, 0, 0, 0, 0);
    }
}

void time() {
    DateTime now = RTC.now();
    
    int hour = now.hour();
    if(hour>11) hour = hour-12; 
    int minute = now.minute()/5; 
    clear();
    if (minute!= hour) {
      strip.setPixelColor(hour, blue); 
      strip.setPixelColor(minute, red);
    }
    else if (minute == hour) {
      strip.setPixelColor(hour, orange);
    }
    strip.show();
}
void temperature() {
  int reading = analogRead(TEMP); 
  float voltage = reading * 5.1 / 1024;
  float temperatureC = (voltage - 0.5) * 100 ;
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  bool binaryTemp[7];

  //convert the temperature to a binary representation
  if(temperatureF >= 64) {
    temperatureF -= 64;
    binaryTemp[0] = true;
  }
  else {
    binaryTemp[0] = false;
  }
  if(temperatureF >= 32) {
    temperatureF -= 32;
    binaryTemp[1] = true;
  }
  else {
    binaryTemp[1] = false;
  }
  if(temperatureF >= 16) {
    temperatureF -= 16;
    binaryTemp[2] = true;
  }
  else {
    binaryTemp[2] = false;
  }
  if(temperatureF >= 8) {
    temperatureF -= 8;
    binaryTemp[3] = true;
  }
  else {
    binaryTemp[3] = false;
  }
  if(temperatureF >= 4) {
    temperatureF -= 4;
    binaryTemp[4] = true;
  }
  else {
    binaryTemp[4] = false;
  }
  if(temperatureF >= 2) {
    temperatureF -= 2;
    binaryTemp[5] = true;
  }
  else {
    binaryTemp[5] = false;
  }
  if(temperatureF >= 1) {
    temperatureF -= 1;
    binaryTemp[6] = true;
  }
  else {
    binaryTemp[6] = false;
  }

  strip.setPixelColor(9, temperatureColor(binaryTemp[0]));
  strip.setPixelColor(10, temperatureColor(binaryTemp[1]));
  strip.setPixelColor(11, temperatureColor(binaryTemp[1]));
  strip.setPixelColor(0, temperatureColor(binaryTemp[2]));
  strip.setPixelColor(1, temperatureColor(binaryTemp[3]));
  strip.setPixelColor(2, temperatureColor(binaryTemp[4]));
  strip.setPixelColor(3, temperatureColor(binaryTemp[5]));
  strip.show();   
}

uint32_t temperatureColor(bool signal) {
  if(signal) {
    return red;
  }
  else {
    return blue;
  }
}

void serialDebug() {
    /*
    mySerial.print(now.year(), DEC);
    mySerial.print('/');
    mySerial.print(now.month(), DEC);
    mySerial.print('/');
    mySerial.print(now.day(), DEC);
    mySerial.print(' ');
    mySerial.print(now.hour(), DEC);
    mySerial.print(':');
    mySerial.print(now.minute(), DEC);
    mySerial.print(':');
    mySerial.print(now.second(), DEC);
    mySerial.println();
    mySerial.println();
    */
}



