
#include "RTClib.h"

RTC_DS1307 rtc;
int prev_min;
int prev_sec;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setupRTC()
{

#ifndef ESP8266
    while (!Serial)
        ; // wait for serial port to connect. Needed for native USB
#endif

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }

    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
}
void RTC_time_update()
{

    rtc.adjust(DateTime(GPSclock.year, GPSclock.month, GPSclock.day, GPSclock.hour, GPSclock.minute, GPSclock.second));
    DateTime now = rtc.now();
    timeGPSclock = 1;
    Serial.print("hour   ");
    Serial.print(now.hour());
    Serial.print(" : ");
    Serial.println(now.minute());
}
