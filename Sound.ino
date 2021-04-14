/////////////////////////////////////////////////////
//
//Setup the Sound data variables
//
/////////////////////////////////////////////////////

//http://www.seeedstudio.com/wiki/index.php?title=Electronic_brick_-_microphone_with_amplifier_(Analog)

int soundInputPin = A0;
long int soundMinuteSum = 0;              // sum of all the minute samples
int soundFindMinimumAll = 1024;
int soundFindMaximumAll = 0;
int soundAllSum = 0;
const int numReadingsAll= 2;    // number of sampling cycles
int index = 0;                  // count for each cycle

void setupSound(){
  Serial.print("Sound Sensor Calibrating... ");
  soundMinuteSum = 0;
  Serial.println("Initialized!");
  delay(100);
}

void loopSound(){
  /////////////////////////
  // SOUND
  /////////////////////////
  //Initialize min/max values
  int soundMinimum = 1024;
  int soundMaximum = 0;

  //Scan 128 values (to make sure that we definitely get a maximum value - not RMS or DC)
  for(int i=0;i<128;i++) {
    int soundValue = analogRead(soundInputPin);
    if (soundValue>10){
      soundMinimum = min(soundMinimum, soundValue);
      soundMaximum = max(soundMaximum, soundValue);
    }
  }

  //Assign values to get the max and min values for a complete cycle
  //(i.e. min/max for half an hour)
  soundFindMinimumAll = min(soundFindMinimumAll, soundMinimum);
  //Serial.print("Sound MinAll: ");
  //Serial.println(soundFindMinimumAll);

  soundFindMaximumAll = max(soundFindMaximumAll, soundMaximum);
  //Serial.print("Sound MaxAll: ");
  //Serial.println(soundFindMaximumAll);

  //Get the Peak of the volume - value goes to LED 
  soundMinuteSum = soundMaximum-soundMinimum;

  //Function to becibel
  soundDBFinal = (16.801 * log(soundMinuteSum/512.) + 9.872) + 100.;

  Serial.print("Sound: ");
  Serial.print(soundDBFinal);
  Serial.println(" dB");

  /*
  Z_OSCMessage message;
  message.setAddress(destIp, destPort);
  message.setZ_OSCMessage("/Sound", "f", &soundDBFinal);
  oscClient.send(&message);
  message.flush();
  */
  ledSound();
  delay(1000);
}

void ledSound(){
  //  //////////////////////////////////////////////////////////////////////
  //  // LED POSITION A4 --- SOUND
  //  //////////////////////////////////////////////////////////////////////
  colorWipe(3, 4, strip.Color(0, 0, 0), 0);
  colorWipe(11, 12, strip.Color(0, 0, 0), 0);
  colorWipe(19, 20, strip.Color(0, 0, 0), 0);
  colorWipe(27, 28, strip.Color(0, 0, 0), 0);
  colorWipe(35, 36, strip.Color(0, 0, 0), 0);
  
  if (soundDBFinal<=50){
    colorWipe(3, 4, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(11, 12, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(19, 20, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(27, 28, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(35, 36, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  } 
  else if (soundDBFinal>50 && soundDBFinal<=60){
    //colorWipe(3, 4, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    colorWipe(11, 12, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(19, 20, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(27, 28, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(35, 36, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
  } 
  else if (soundDBFinal>60 && soundDBFinal<=80){
    //colorWipe(3, 4, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(11, 12, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    colorWipe(19, 20, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(27, 28, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(35, 36, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0); 
  } 
  else if (soundDBFinal>80 && soundDBFinal<=90){
    //colorWipe(3, 4, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(11, 12, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(19, 20, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    colorWipe(27, 28, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
    //colorWipe(35, 36, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);  
  } 
  else if (soundDBFinal>90){
    //colorWipe(3, 4, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(11, 12, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(19, 20, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(27, 28, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
    colorWipe(35, 36, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
  }
}
