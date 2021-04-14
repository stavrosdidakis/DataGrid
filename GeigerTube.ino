
//http://www.cooking-hacks.com/documentation/tutorials/geiger-counter-arduino-radiation-sensor-board

/////////////////////////////////////////////////////
//
//Setup the Radiation
//
/////////////////////////////////////////////////////
// Threshold values
#define TH1 45
#define TH2 95
#define TH3 200
#define TH4 400
#define TH5 600

// Conversion factor - CPM to uSV/h
#define CONV_FACTOR 0.00812

// Variables
int geiger_input = 2;
long count = 0;
long timePreviousMeassure = 0;
long countPrevious = 0;
float radiationValue = 0.0;

void setupRadiation(){
  Serial.print("Radiation Sensor Calibrating... ");
  pinMode(geiger_input, INPUT);
  digitalWrite(geiger_input,HIGH);
  //Serial.print("CPM=");
  //Serial.println(6*count);
  //Serial.println(radiationValue);
  attachInterrupt(0,countPulse,FALLING);
  Serial.println("Initialized!");
  delay(100);
}

void loopRadiation(){
  if (millis()-timePreviousMeassure > 10000){
    radiation_CPM_Final = 6*count;
    radiationValue = radiation_CPM_Final * CONV_FACTOR;
    timePreviousMeassure = millis();

    Serial.print("Radiation Value: ");
    Serial.print(radiation_CPM_Final, DEC);
    Serial.println(" cpm"); 

    Serial.print("Radiation Value: ");
    Serial.print(radiationValue,4);      
    Serial.println(" uSv/h");

    /*
    Z_OSCMessage message;
    message.setAddress(destIp, destPort);
    message.setZ_OSCMessage("/Radiation", "i", &radiation_CPM_Final);
    oscClient.send(&message);
    message.flush();
    */
    //ledRadiation();
    count = 0;
  }
  ledRadiation();
  delay(1000);
}

void countPulse(){
  detachInterrupt(0);
  count++;
  while(digitalRead(2)==0){
  }
  attachInterrupt(0,countPulse,FALLING);
}

void ledRadiation(){
  //   //////////////////////////////////////////////////////////////////////
  //   // LED POSITION B5 --- RADIATION
  //   //////////////////////////////////////////////////////////////////////
  colorWipe(7, 8, strip.Color(0, 0, 0), 0);
  colorWipe(15, 16, strip.Color(0, 0, 0), 0);
  colorWipe(23, 24, strip.Color(0, 0, 0), 0);
  colorWipe(31, 32, strip.Color(0, 0, 0), 0);
  colorWipe(39, 40, strip.Color(0, 0, 0), 0);
  
  if (radiation_CPM_Final<=100) {
    colorWipe(7, 8, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(15, 16, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(23, 24, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(31, 32, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(39, 40, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  }
  else if (radiation_CPM_Final>100 && radiation_CPM_Final<=300) {
    //colorWipe(7, 8, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    colorWipe(15, 16, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(23, 24, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(31, 32, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(39, 40, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
  }
  else if (radiation_CPM_Final>300 && radiation_CPM_Final<=700) {
    //colorWipe(7, 8, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(15, 16, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    colorWipe(23, 24, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(31, 32, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(39, 40, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
  }
  else if (radiation_CPM_Final>700 && radiation_CPM_Final<=1500) {
    //colorWipe(7, 8, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(15, 16, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(23, 24, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    colorWipe(31, 32, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
    //colorWipe(39, 40, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
  }
  else if (radiation_CPM_Final>1500) {
    //colorWipe(7, 8, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(15, 16, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(23, 24, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(31, 32, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
    colorWipe(39, 40, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
  }
}
