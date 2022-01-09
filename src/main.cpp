#include <Arduino.h>
// inculde GPS library and setup one
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define S_RX 3 // arduino RX
#define S_TX 4 // arduino TX
SoftwareSerial SoftSerial(S_RX, S_TX);
TinyGPSPlus gps;
// depending from this variable will be shown time or temperature
bool clockFlag = 0;

// variable for first time synchronization with GPS
bool timeGPSclock = 0;
// here two type of time: time from GPS and current time
// every nigth current time synchronize with GPS time
struct MyTime
{
  int hour;
  int minute;
  int day;
  int month;
  int year;
  int second;
};
struct MyTime GPSclock;
struct MyTime clock;
// include and setup another modules
#include "led.h"
#include "rtc.h"
#include "GPS.h"
#include "temp.h"

// synchronization happening becouse arduino does reset
int reset = 12;

void setup(void)
{
  pinMode(reset, OUTPUT);
  Serial.begin(9600);
  SoftSerial.begin(9600);
  // setupLed();
  setupRTC();
  Serial.println("Dallas Temperature IC Control Library Demo");
  setupTemp();
  Serial.println("Temperature was setup");
}
//------------------------------------------------------------------
void loop(void)
{
  // sinhronization first time
  if (!timeGPSclock)
  {
    GPSTimeUpdate();
  }
  else
  {
    // get time from rtc1307
    DateTime now = rtc.now();
    // display time
    if (!(now.second() % 5) && !clockFlag && prev_sec != now.second())
    {
      prev_sec = now.second();
      clockFlag = true;
      prev_sec = now.second();
      dmd.clearScreen(true);
      prev_min = now.minute();
      byte h1 = now.hour() / 10;
      byte h2 = now.hour() % 10;
      byte m1 = now.minute() / 10;
      byte m2 = now.minute() % 10;
      byte s1 = now.second() / 10;
      byte s2 = now.second() % 10;

      if (h1 > 0)
        digitPrint(0, 0, h1);
      else
        digitPrint(0, 0, h1); // Blank

      digitPrint(6, 0, h2);
      dmd.writePixel(12, 3, GRAPHICS_NORMAL, 1);
      dmd.writePixel(12, 6, GRAPHICS_NORMAL, 1);
      digitPrint(14, 0, m1);
      digitPrint(20, 0, m2);
    }
    // display temperature
    else if (!(now.second() % 5) && clockFlag && prev_sec != now.second())
    {
      prev_sec = now.second();
      clockFlag = false;
      dmd.clearScreen(true);
      byte h1 = abs(temp / 10);
      byte h2 = abs(temp % 10);

      if (h1 > 0)
        digitPrint(4, 0, h1);
      digitPrint(10, 0, h2);
      digitPrint(14, 0, 10);
      if (temp < 0 && h1 > 0)
        dmd.drawLine(0, 5, 2, 5, GRAPHICS_NORMAL);
      else if (temp < 0 && h1 == 0)
        dmd.drawLine(5, 5, 7, 5, GRAPHICS_NORMAL);
      sensors.requestTemperatures(); //  get temperature readings
      temp = sensors.getTempCByIndex(0) - 4;
    }
    // synhronization
    if (now.hour() == 2 && now.minute() == 0 && now.second() == 0)
    {
      delay(1000);
      digitalWrite(reset, HIGH);
      delay(1000);
    }
  }
}
//-----------------------------------------------------------------------------------
