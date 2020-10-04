// Include Libraries
#include "GraphicLCD.h"
#include <SPI.h>
#include <SD.h>

#include <MQUnifiedsensor.h>
#include <Wire.h>
#include "Adafruit_SGP30.h"

#include "MutichannelGasSensor.h"

/************************Hardware Related Macros************************************/
#define         Board                   ("Arduino Mega")
#define         Pin2                     (A2)  //Analog input 2 of your arduino
#define         Pin7                     (A7)  //Analog input 7 of your arduino
#define         Pin8                     (A8)  //Analog input 8 of your arduino
/***********************Software Related Macros************************************/
#define         RatioMQ2CleanAir          (9.83) //RS / R0 = 9.83 ppm 
#define         RatioMQ8CleanAir          (70) //RS / R0 = 70 ppm   
#define         RatioMQ135CleanAir        (3.6) //RS / R0 = 3.6 ppm  
#define         ADC_Bit_Resolution        (10) // 10 bit ADC 
#define         Voltage_Resolution        (5) // Volt resolution to calc the voltage
#define         Type                      ("Arduino Mega 2560") //Board used
/*****************************Globals***********************************************/

MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin2, Type);
MQUnifiedsensor MQ8(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin8, Type);
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin8, Type);
Adafruit_SGP30 sgp;


// object initialization
GraphicLCD graphicLCD(GRAPHICLCD_PIN_RX,GRAPHICLCD_PIN_TX);


uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}
void setup() 
{
  Serial.begin(9600);
    
  Serial.println("start");
  MQ2.init();
  MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ2.setA(574.25); MQ2.setB(-2.222); // Configurate the ecuation values to get LPG concentration
  MQ2.setR0(9.659574468);
  
  MQ8.init(); 
  MQ8.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ8.setA(976.97); MQ8.setB(-0.688); // Configurate the ecuation values to get H2 concentration
  MQ8.setR0(1);

  MQ135.init(); 
  MQ135.setRegressionMethod(1);
  MQ135.setA(605.18); MQ135.setB(-3.937);
  MQ8.setR0(1);
  
  
  
  Serial.print("Calibrating please wait.");
  float MQ2calcR0 = 0, 
  MQ8calcR0 = 0, MQ135calcR0 = 0;
  
  for(int i = 1; i<=10; i ++)
  {
    //Update the voltage lectures
    MQ2.update();
    MQ8.update();
    MQ135.update();

    MQ2calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    MQ8calcR0 += MQ2.calibrate(RatioMQ8CleanAir);
    MQ135calcR0 += MQ135.calibrate(RatioMQ135CleanAir);

    Serial.print(".");
  }
  MQ2.setR0(MQ2calcR0/20);
  MQ8.setR0(MQ8calcR0/20);
  MQ135.setR0(calcR0/10);
  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);
  
  Serial.println("  done!.");
  
  Serial.print(MQ2calcR0/10); Serial.print(" | ");
  Serial.print(MQ8calcR0/10); Serial.print(" | ");

  if(isinf(MQ2calcR0) || isinf(MQ8calcR0) || isinf(MQ135calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(MQ2calcR0 == 0 || MQ8calcR0 == 0 || MQ135calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
 
  //Print in serial monitor
  Serial.println("MQ2 and MQ8 and MQ135- lecture");

    Serial.print("Initializing SD card...");

    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
      while (1);
    }
    Serial.println("initialization done.");
    gas.begin(0x04);//the default I2C address of the slave is 0x04
    gas.powerOn();

    
}

int counter = 0;
void loop() 
{
    MQ2.update();
    MQ8.update();
    MQ135.update()
    //Read the sensor and print in serial port
    float MQ2Lecture =  MQ2.readSensor();
    float MQ8Lecture =  MQ8.readSensor();
    float MQ135Lecture =  MQ135.readSensor();

    
    graphicLCD.setX(30);                    
    graphicLCD.setY(50); 
    if (! sgp.IAQmeasure()) {
    graphicLCD.println("Measurement failed");
    return;
    }
    graphicLCD.print("TVOC "); graphicLCD.print(sgp.TVOC); graphicLCD.print(" ppb\t");
    graphicLCD.print("eCO2 "); graphicLCD.print(sgp.eCO2); graphicLCD.println(" ppm");

  if (! sgp.IAQmeasureRaw()) {
    graphicLCD.println("Raw Measurement failed");
    return;
  }
  graphicLCD.print("Raw H2 "); graphicLCD.print(sgp.rawH2); graphicLCD.print(" \t");
  graphicLCD.print("Raw Ethanol "); graphicLCD.print(sgp.rawEthanol); graphicLCD.println("");
 
  delay(1000);

    counter++;
    if (counter == 30) {
      counter = 0;
  
      uint16_t TVOC_base, eCO2_base;
      if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
        Serial.println("Failed to get baseline readings");
        return;
      }
      Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
      Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
    }
    float c;

    c = gas.measure_NH3();
    Serial.print("The concentration of NH3 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_CO();
    Serial.print("The concentration of CO is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_NO2();
    Serial.print("The concentration of NO2 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_C3H8();
    Serial.print("The concentration of C3H8 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_C4H10();
    Serial.print("The concentration of C4H10 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_CH4();
    Serial.print("The concentration of CH4 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_H2();
    Serial.print("The concentration of H2 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_C2H5OH();
    Serial.print("The concentration of C2H5OH is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");               
    graphicLCD.print(MQ2Lecture,MQ8Lecture,MQ135Lecture);  
    delay(1000);                               
    graphicLCD.clear();                      
    delay(1000);
    myFile = SD.open("test.txt", FILE_WRITE);
    
    // if the file opened okay, write to it:
    if (myFile) {
      Serial.print("Writing to test.txt...");
      myFile.println("Sensor,Readings");
      myFile.println("MQ2",+MQ2Lecture);
      myFile.println("MQ8",+MQ8Lecture);
      myFile.println("MQ135",+MQ135Lecture);
      myFile.println("SGP30",+sgp.TVOC+sgp.eCO2+sgp.rawEthanol+sgp.rawH2);
      myFile.println();
      // close the file:
      myFile.close();
      Serial.println("Done.");
    } else {
      Serial.println("Error opening test.txt");
    }

}
