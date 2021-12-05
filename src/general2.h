#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define S_RX 3 // Вывод RX
#define S_TX 4 // Вывод TX

SoftwareSerial SoftSerial(S_RX, S_TX);
TinyGPSPlus gps;
int start_positon;
bool clockFlag = 0;
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
bool timeGPSclock = 0;
#include "led.h"
#include "rtc.h"
#include "GPS.h"
#include "temp.h"

void setup(void)
{
  Serial.begin(9600);
  SoftSerial.begin(9600);

  // setupLed();
  setupRTC();
  Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  setupTemp();
  Serial.println("Temperature was setup");

  // while (!clockFlag)
  //{
  //   Serial.println("not here");
  //   GPSTimeUpdate();
  // }
}
//------------------------------------------------------------------
void loop(void)
{
  if (!timeGPSclock)
    GPSTimeUpdate();
  else
  {
    DateTime now = rtc.now();
    // if (!(now.second() % 5) && !clockFlag && prev_sec != now.second())
    // {
    prev_sec = now.second();
    clockFlag = true;
    prev_sec = now.second();
    delay(5000);
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
    // }
    //  else if (!(now.second() % 5) && clockFlag && prev_sec != now.second())
    //   {
    prev_sec = now.second();
    clockFlag = false;
    delay(5000);
    dmd.clearScreen(true);
    h1 = abs(temp / 10);
    h2 = abs(temp % 10);

    if (h1 > 0)
      digitPrint(4, 0, h1);
    digitPrint(10, 0, h2);
    digitPrint(14, 0, 10);
    if (temp < 0 && h1 > 0)
      dmd.drawLine(0, 5, 2, 5, GRAPHICS_NORMAL);
    else if (temp < 0 && h1 == 0)
      dmd.drawLine(5, 5, 7, 5, GRAPHICS_NORMAL);
    sensors.requestTemperatures(); // Send the command to get temperature readings
    temp = sensors.getTempCByIndex(0) - 4;
    //  }
  }
}
//-----------------------------------------------------------------------------------
