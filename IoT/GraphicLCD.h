#ifndef _LCD_H_
#define _LCD_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

const byte PIXEL_ON     = 0x01;
const byte PIXEL_OFF    = 0x00;

class GraphicLCD
{
  public:
    GraphicLCD(const int rxPin, const int txPin);
    
    void print(char *Str);
    void print(int8_t num);
    void println(char *Str);
    void println(int8_t num);
    
    void clear();
    void toggleReverseMode();
    void toggleSplash();
    void setBacklight(byte duty);
    void setBaud(byte baud);
    void restoreDefaultBaud();
    void demo();
    
    void setHome();
    void setX(byte posX);
    void setY(byte posY);

    void drawPixel (byte x , byte y , byte set = PIXEL_ON);
    void drawLine  (byte x1, byte y1, byte x2, byte y2);
    void drawBox   (byte x1, byte y1, byte x2, byte y2);
    void drawCircle(byte x , byte y , byte rad        );
    void clearBlock(byte x1, byte y1, byte x2, byte y2);
  
  
  private:
    SoftwareSerial m_serial;
};

