/*
 Interface to Shinyei Model PPD42NS Particle Sensor
 Program by Christopher Nafis 
 Written April 2012
 
 http://www.seeedstudio.com/depot/grove-dust-sensor-p-1050.html
 http://www.sca-shinyei.com/pdf/PPD42NS.pdf
 
 JST Pin 1 (Black Wire)  => Arduino GND
 JST Pin 3 (Red wire)    => Arduino 5VDC
 JST Pin 4 (Yellow wire) => Arduino Digital Pin 8
 */
int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;

long int dustVal = 0;

void setupDust() {
  Serial.print("Dust Sensor Calibrating... ");
  pinMode(8,INPUT);
  starttime = millis();
  Serial.println("Initialized!");
  delay(100);
}

void loopDust() {
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    dust_Concentration_Final = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    //    Serial.print(lowpulseoccupancy);
    //    Serial.print(",");
    //    Serial.print(ratio);
    //    Serial.print(",");
    //    Serial.println(concentration);

    Serial.print("Dust: ");
    Serial.print(dust_Concentration_Final, 2);
    Serial.println(" particles per 0.01 cubic feet");

    /*
    Z_OSCMessage message;
    message.setAddress(destIp, destPort);
    message.setZ_OSCMessage("/Dust", "f", &dust_Concentration_Final, 2);
    oscClient.send(&message);
    message.flush();
    */

    lowpulseoccupancy = 0;
    starttime = millis();
  }
  ledDust();
  delay(1000);
}

void ledDust(){
  //  //////////////////////////////////////////////////////////////////////
  //   // LED POSITION A5 --- DUST
  //   //////////////////////////////////////////////////////////////////////
  colorWipe(6, 7, strip.Color(0, 0, 0), 0);
  colorWipe(14, 15, strip.Color(0, 0, 0), 0);
  colorWipe(22, 23, strip.Color(0, 0, 0), 0);
  colorWipe(30, 31, strip.Color(0, 0, 0), 0);
  colorWipe(38, 39, strip.Color(0, 0, 0), 0);  
  
  if (dust_Concentration_Final<=4) {
    colorWipe(6, 7, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(14, 15, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(22, 23, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(30, 31, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(38, 39, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  }
  else if (dust_Concentration_Final>4 && dust_Concentration_Final<=8) {
    //colorWipe(6, 7, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    colorWipe(14, 15, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(22, 23, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(30, 31, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(38, 39, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
  }
  else if (dust_Concentration_Final>8 && dust_Concentration_Final<=15) {
    //colorWipe(6, 7, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(14, 15, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    colorWipe(22, 23, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(30, 31, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(38, 39, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
  }
  else if (dust_Concentration_Final>15 && dust_Concentration_Final<=30) {
    //colorWipe(6, 7, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(14, 15, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(22, 23, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    colorWipe(30, 31, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
    //colorWipe(38, 39, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
  }
  else if (dust_Concentration_Final>30) {
    //colorWipe(6, 7, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(14, 15, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(22, 23, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(30, 31, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
    colorWipe(38, 39, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
  }
}
