/////////////////////////////////////////////////////
//
//Setup the Temperature & Humidity data variables
//
/////////////////////////////////////////////////////
//http://www.adafruit.com/products/246#Description

#include <SHT1x.h>
#define clockPin 3
#define dataPin 4

SHT1x sht1x(dataPin, clockPin);

int valTemp = 20;
int valHumid = 80;

void setupTempHumid(){
  Serial.print("Temperature Sensor Calibrating... ");
  temperatureFinal = sht1x.readTemperatureC();
  Serial.println("Initialized!");

  Serial.print("Humidity Sensor Calibrating... ");
  humidityFinal = sht1x.readHumidity();
  Serial.println("Initialized!");

  delay(100);
}

void loopTempHumid(){
  /////////////////////////
  //TEMPERATURE
  /////////////////////////

  //-5.0 (offset)
  temperatureFinal = sht1x.readTemperatureC()-5.0;
  Serial.print("Temperature: ");
  Serial.print(temperatureFinal, 2);
  Serial.println(" C");

  /*
  Z_OSCMessage message1;
  message1.setAddress(destIp, destPort);
  message1.setZ_OSCMessage("/Temperature", "f", &temperatureFinal, 2);
  oscClient.send(&message1);
  message1.flush();
  */
  
  Serial.println("temperature message sent");
  ledTemperature();
  delay(1000);

  /////////////////////////
  // HUMIDITY
  /////////////////////////
  humidityFinal = sht1x.readHumidity();

  Serial.print("Humidity: ");
  Serial.print(humidityFinal);
  Serial.println(" %");

  /*
  Z_OSCMessage message2;
  message2.setAddress(destIp, destPort);
  message2.setZ_OSCMessage("/Humidity", "i", &humidityFinal);
  oscClient.send(&message2);
  message2.flush();
  */
  ledHumidity();
  delay(1000);
}

void ledTemperature(){
  //  //////////////////////////////////////////////////////////////////////
  //  // LED POSITION A1 --- TEMPERATURE
  //  //////////////////////////////////////////////////////////////////////
  colorWipe(0, 1, strip.Color(0, 0, 0), 0);
  colorWipe(8, 9, strip.Color(0, 0, 0), 0);
  colorWipe(16, 17, strip.Color(0, 0, 0), 0);
  colorWipe(24, 25, strip.Color(0, 0, 0), 0);
  colorWipe(32, 33, strip.Color(0, 0, 0), 0);

  if (temperatureFinal<=8) {
    colorWipe(0, 1, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(8, 9, strip.Color(ledColorBlueR, ledColorBlueR, ledColorBlueR), 0);
    //colorWipe(16, 17, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(24, 25, strip.Color(ledColorBlueR, ledColorBlueR, ledColorBlueR), 0);
    //colorWipe(32, 33, strip.Color(ledColorBlueR, ledColorBlueR, ledColorBlueR), 0);    
  }
  else if (temperatureFinal>8 && temperatureFinal<=15) {
    //colorWipe(0, 1, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    colorWipe(8, 9, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(16, 17, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(24, 25, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(32, 33, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
  }
  else if (temperatureFinal>15 && temperatureFinal<=30) {
    //colorWipe(0, 1, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(8, 9, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    colorWipe(16, 17, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(24, 25, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(32, 33, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
  }
  else if (temperatureFinal>30 && temperatureFinal<=40) {
    //colorWipe(0, 1, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(8, 9, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(16, 17, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    colorWipe(24, 25, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
    //colorWipe(32, 33, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
  }
  else if (temperatureFinal>40) {
    //colorWipe(0, 1, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(8, 9, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(16, 17, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(24, 25, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);    
    colorWipe(32, 33, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);        
  }
}

void ledHumidity(){
  //  //  //////////////////////////////////////////////////////////////////////
  //  //  // LED POSITION A2 --- HUMIDITY
  //  //  //////////////////////////////////////////////////////////////////////
  colorWipe(1, 2, strip.Color(0, 0, 0), 0);
  colorWipe(9, 10, strip.Color(0, 0, 0), 0);
  colorWipe(17, 18, strip.Color(0, 0, 0), 0);
  colorWipe(25, 26, strip.Color(0, 0, 0), 0);
  colorWipe(33, 34, strip.Color(0, 0, 0), 0);
  
  if (humidityFinal<=40) {
    colorWipe(1, 2, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(9, 10, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(17, 18, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(25, 26, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(33, 34, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  }
  else if (humidityFinal>40 && humidityFinal<=60) {
    //colorWipe(1, 2, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    colorWipe(9, 10, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(17, 18, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(25, 26, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(33, 34, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
  }
  else if (humidityFinal>60 && humidityFinal<=80) {
    //colorWipe(1, 2, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(9, 10, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    colorWipe(17, 18, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(25, 26, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(33, 34, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);    
  }
  else if (humidityFinal>80 && humidityFinal<=90) {
    //colorWipe(1, 2, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(9, 10, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(17, 18, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    colorWipe(25, 26, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
    //colorWipe(33, 34, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);    
  }
  else if (humidityFinal>90) {
    //colorWipe(1, 2, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(9, 10, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(17, 18, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(25, 26, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
    colorWipe(33, 34, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
  }
}
