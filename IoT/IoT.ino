// Include Libraries
#include "GraphicLCD.h"


// Pin Definitions
#define GRAPHICLCD_PIN_TX  3
#define GRAPHICLCD_PIN_RX 2
#define MICS5524_PIN_AO A0
#define MQ135_5V_PIN_AOUT A1
#define MQ2_5V_PIN_AOUT A2
#define MQ8_5V_PIN_AOUT A3

// object initialization
GraphicLCD graphicLCD(GRAPHICLCD_PIN_RX,GRAPHICLCD_PIN_TX);


void setup() 
{
    Serial.begin(9600);
    
    Serial.println("start");
    
    
    menuOption = menu();
    
}

void loop() 
{
    graphicLCD.setX(30);                    
    graphicLCD.setY(50);                     
    graphicLCD.print("NASA");  
    delay(1000);                               
    graphicLCD.clear();                      
    delay(1000);
    
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        Serial.println(F("Now Testing Graphic LCD 160x128 Huge"));
        Serial.println(F("Now Testing Adafruit MiCS5524 CO, Alcohol and VOC Gas Sensor Breakout"));
        Serial.println(F("Now Testing Hazardous Gas  Sensor - MQ-135"));
        Serial.println(F("Now Testing Methane, Butane, LPG and Smoke Gas Sensor - MQ-2"));
        Serial.println(F("Now Testing Hydrogen Gas Sensor - MQ-8"));
        Serial.println(F("Now Testing Adafruit SGP30 Air Quality Sensor Breakout - VOC and eCO2"));
        time0 = millis();
        return 1;
        
    }
    Serial.println(F("illegal input!"));
    return 0;
}