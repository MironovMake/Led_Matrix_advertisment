#include <OneWire.h>
OneWire ds2(2);
String res = "";
byte type_s;
byte data[12];
byte addr_ds2[8];
bool temp_sensor_2_found = false;
float temp1 = 999;
//===================================================================

#include <SPI.h>
#include "DMD.h"
#include "TimerOne.h"
#include "SystemFont5x7rus.h"

#define CLR 0x0C
#define US 0x1F
#define ESC 0x1B
#define LF 0x0A

static const char ID[] = "P10_DISPLAY_V01\r\n"; // Device ID

#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN); // Конфигурация экрана, панелей может быть не одна.
#define max_char1 6
#define max_char2 176
unsigned char message1[max_char1] = {0x20, 0x30, 0x30, 0x30, 0x30, 0x00}; // stores you message
unsigned char message2[max_char2];                                        // stores you message

unsigned char r_char;
byte index1 = 4;
byte index2 = 176;
int i;
long scrollSpeed = 25;

//=============================================================================
bool get_sensor(byte addr[8], OneWire ds)
{

  // Ищем адрес датчика
  if (!ds.search(addr))
  {
    ds.reset_search();
    delay(250);
    return false;
  }

  // Проверяем не было ли помех при передаче
  if (OneWire::crc8(addr, 7) != addr[7])
  {
    Serial.println("get_sensor:CRC is not valid!");
    return false;
  }

  // Определяем серию датчика
  switch (addr[0])
  {
  case 0x10:
    // Chip = DS18S20
    type_s = 1;
    break;
  case 0x28:
    // Chip = DS18B20
    type_s = 0;
    break;
  case 0x22:
    // Chip = DS1822
    type_s = 0;
    break;
  default:
    Serial.println("get_sensor:Device is not a DS18x20 family device.");
    return false;
  }
  return true;
}

//=============================================================================

double get_temp(byte addr[8], OneWire ds)
{

  double celsius;

  ds.reset();
  ds.select(addr);   // Выбираем адрес
  ds.write(0x44, 1); // Производим замер, в режиме паразитного питания
  delay(750);
  ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Считываем оперативную память датчика

  for (int i = 0; i < 9; i++)
  { // Заполняем массив считанными данными
    data[i] = ds.read();
  }

  // Данные о температуре содержатся в первых двух байтах, переведем их в одно значение и преобразуем в шестнадцатиразрядное число
  int raw = (data[1] << 8) | data[0];
  if (type_s)
  {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10)
    {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  }
  else
  {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00)
      raw = raw << 3;
    else if (cfg == 0x20)
      raw = raw << 2;
    else if (cfg == 0x40)
      raw = raw << 1;
  }

  celsius = (double)raw / 16.0;
  return celsius;
};

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}
void serialEvent()
{

  dmd.writePixel(0, 0, GRAPHICS_NORMAL, 1);
}
//------------------------------------------------------------------
void digitPrint(int Xstart, int Ystart, int value)
{
  switch (value)
  {
  case 0:
    dmd.drawLine(Xstart + 1, Ystart + 0, Xstart + 3, Ystart + 0, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 1, Xstart + 0, Ystart + 8, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 9, Xstart + 3, Ystart + 9, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 4, Ystart + 1, Xstart + 4, Ystart + 8, GRAPHICS_NORMAL);

    break;
  case 1:
    dmd.drawLine(Xstart + 2, Ystart + 0, Xstart + 2, Ystart + 9, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart, Ystart + 9, Xstart + 4, Ystart + 9, GRAPHICS_NORMAL);
    dmd.writePixel(Xstart + 1, Ystart + 1, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart, Ystart + 2, GRAPHICS_NORMAL, 1);
    break;
  case 2:
    dmd.writePixel(Xstart + 0, Ystart + 1, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 1, Ystart + 0, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 2, Ystart + 0, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 3, Ystart + 0, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 4, Ystart + 1, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 4, Ystart + 2, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 4, Ystart + 3, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 4, Ystart + 4, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 3, Ystart + 5, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 2, Ystart + 6, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 1, Ystart + 7, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 0, Ystart + 8, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 0, Ystart + 9, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 1, Ystart + 9, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 2, Ystart + 9, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 3, Ystart + 9, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 4, Ystart + 9, GRAPHICS_NORMAL, 1);
    break;
  case 3:
    dmd.drawLine(Xstart + 0, Ystart + 0, Xstart + 4, Ystart + 0, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 4, Ystart + 0, Xstart + 1, Ystart + 3, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 3, Xstart + 4, Ystart + 6, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 4, Ystart + 6, Xstart + 4, Ystart + 8, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 3, Ystart + 9, Xstart + 1, Ystart + 9, GRAPHICS_NORMAL);
    dmd.writePixel(Xstart + 0, Ystart + 8, GRAPHICS_NORMAL, 1);
    break;
  case 4:
    dmd.drawLine(Xstart + 3, Ystart + 0, Xstart + 3, Ystart + 9, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 6, Xstart + 4, Ystart + 6, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 5, Xstart + 3, Ystart + 0, GRAPHICS_NORMAL);

    break;
  case 5:
    dmd.drawLine(Xstart + 0, Ystart + 0, Xstart + 4, Ystart + 0, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 0, Xstart + 0, Ystart + 4, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 4, Xstart + 3, Ystart + 4, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 4, Ystart + 5, Xstart + 4, Ystart + 8, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 9, Xstart + 3, Ystart + 9, GRAPHICS_NORMAL);
    dmd.writePixel(Xstart + 0, Ystart + 8, GRAPHICS_NORMAL, 1);

    break;
  case 6:
    dmd.drawLine(Xstart + 2, Ystart + 0, Xstart + 4, Ystart + 0, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 2, Ystart + 0, Xstart + 0, Ystart + 3, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 4, Xstart + 0, Ystart + 8, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 9, Xstart + 3, Ystart + 9, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 4, Ystart + 8, Xstart + 4, Ystart + 5, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 4, Ystart + 8, Xstart + 4, Ystart + 6, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 4, Xstart + 3, Ystart + 4, GRAPHICS_NORMAL);

    break;
  case 7:
    dmd.writePixel(Xstart + 4, Ystart + 1, GRAPHICS_NORMAL, 1);
    dmd.drawLine(Xstart + 5, Ystart + 1, Xstart + 1, Ystart + 5, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 5, Xstart + 1, Ystart + 9, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 0, Xstart + 5, Ystart + 0, GRAPHICS_NORMAL);

    break;
  case 8:
    dmd.drawLine(Xstart + 1, Ystart + 0, Xstart + 3, Ystart + 0, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 1, Xstart + 0, Ystart + 3, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 4, Ystart + 1, Xstart + 4, Ystart + 3, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 4, Xstart + 3, Ystart + 4, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 5, Xstart + 0, Ystart + 8, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 4, Ystart + 5, Xstart + 4, Ystart + 8, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 9, Xstart + 3, Ystart + 9, GRAPHICS_NORMAL);
    break;
  case 9:
    dmd.drawLine(Xstart + 0, Ystart + 9, Xstart + 2, Ystart + 9, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 3, Ystart + 8, Xstart + 4, Ystart + 6, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 4, Ystart + 6, Xstart + 4, Ystart + 1, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 0, Xstart + 3, Ystart + 0, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 0, Ystart + 1, Xstart + 0, Ystart + 3, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 1, Ystart + 4, Xstart + 3, Ystart + 4, GRAPHICS_NORMAL);
    break;
  case 10:
    dmd.drawLine(Xstart + 3, Ystart + 0, Xstart + 4, Ystart + 0, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 2, Ystart + 1, Xstart + 2, Ystart + 2, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 3, Ystart + 3, Xstart + 4, Ystart + 3, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 5, Ystart + 1, Xstart + 5, Ystart + 2, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 7, Ystart + 0, Xstart + 9, Ystart + 0, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 6, Ystart + 1, Xstart + 6, Ystart + 8, GRAPHICS_NORMAL);
    dmd.drawLine(Xstart + 7, Ystart + 9, Xstart + 9, Ystart + 9, GRAPHICS_NORMAL);
    dmd.writePixel(Xstart + 10, Ystart + 1, GRAPHICS_NORMAL, 1);
    dmd.writePixel(Xstart + 10, Ystart + 8, GRAPHICS_NORMAL, 1);

    break;
  }
}
void setupLed()
{
  Timer1.initialize(500);
  Timer1.attachInterrupt(ScanDMD);
  dmd.clearScreen(true);
  temp_sensor_2_found = get_sensor(addr_ds2, ds2);
  dmd.selectFont(SystemFont5x7);
}
