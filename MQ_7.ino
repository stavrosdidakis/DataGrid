// CO2 Gas Sensor
const int GasSensorPin = A3;  // Analog input pin that the Sensor is attached to
int GasSensorValue = 0;
float GasFactor = 24.57143; //28571428571428571428571429 

void setupMQ7(){
  Serial.print("MQ7(CO) Sensor Calibrating... ");
  GasSensorValue = analogRead(GasSensorPin);
  Serial.println("Initialized!");
  delay(100);
}

void loopMQ7(){
  // read the analog in value:
  GasSensorValue = analogRead(GasSensorPin);   

  mq7Final = (long int) GasSensorValue / GasFactor;
  Serial.print("MQ7 CO: ");
  Serial.print(mq7Final);
  Serial.println(" ppm");

  /*
  Z_OSCMessage message;
  message.setAddress(destIp, destPort);
  message.setZ_OSCMessage("/MQ7_CO", "i", &mq7Final);
  oscClient.send(&message);
  message.flush();
  */

  ledMQ7();
  delay(1000);
}

void ledMQ7(){
  //   //////////////////////////////////////////////////////////////////////
  //   // LED POSITION B1 --- CO
  //   //////////////////////////////////////////////////////////////////////
  colorWipe(4, 5, strip.Color(0, 0, 0), 0);
  colorWipe(12, 13, strip.Color(0, 0, 0), 0);
  colorWipe(20, 21, strip.Color(0, 0, 0), 0);
  colorWipe(28, 29, strip.Color(0, 0, 0), 0);
  colorWipe(36, 37, strip.Color(0, 0, 0), 0);
  
  if (mq7Final<=8) {
    colorWipe(4, 5, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(12, 13, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(20, 21, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(28, 29, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(36, 37, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  }
  else if (mq7Final>8 && mq7Final<=15) {
    //colorWipe(4, 5, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    colorWipe(12, 13, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(20, 21, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(28, 29, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(36, 37, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
  }
  else if (mq7Final>15 && mq7Final<=30) {
    //colorWipe(4, 5, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(12, 13, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    colorWipe(20, 21, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(28, 29, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(36, 37, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
  }
  else if (mq7Final>30 && mq7Final<=100) {
    //colorWipe(4, 5, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(12, 13, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(20, 21, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    colorWipe(28, 29, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
    //colorWipe(36, 37, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
  }
  else if (mq7Final>100) {
    //colorWipe(4, 5, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(12, 13, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(20, 21, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(28, 29, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
    colorWipe(36, 37, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
  }
} 
