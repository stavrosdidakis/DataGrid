
/*******************Demo for MG-811 Gas Sensor Module V1.1*****************************
 * Author:  Tiequan Shao: tiequan.shao@sandboxelectronics.com
 * Peng Wei:     peng.wei@sandboxelectronics.com
 * 
 * Lisence: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 * 
 * Note:    This piece of source code is supposed to be used as a demostration ONLY. More
 * sophisticated calibration is required for industrial field application. 
 * 
 * Sandbox Electronics    2012-05-31
 ************************************************************************************/

/************************Hardware Related Macros************************************/
#define         MG811_PIN                          (5)     //define which analog input channel you are going to use
#define         MG811_BOOL_PIN                     (5)
#define         MG811_DC_GAIN                     (8.5)   //define the DC gain of amplifier

/***********************Software Related Macros************************************/
#define         MG811_READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         MG811_READ_SAMPLE_TIMES            (5)     //define the time interval(in milisecond) between each samples in 
//normal operation

/**********************Application Related Macros**********************************/
//These two values differ from sensor to sensor. user should derermine this value.
#define         MG811_ZERO_POINT_VOLTAGE           (0.220) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         MG811_REACTION_VOLTAGE             (0.020) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2

/*****************************Globals***********************************************/
float           MG811_CO2Curve[3]  =  {
  2.602,MG811_ZERO_POINT_VOLTAGE,(MG811_REACTION_VOLTAGE/(2.602-3))};   
//two points are taken from the curve. 
//with these two points, a line is formed which is
//"approximately equivalent" to the original curve.
//data format:{ x, y, slope}; point1: (lg400, 0.324), point2: (lg4000, 0.280) 
//slope = ( reaction voltage ) / (log400 ¨Clog1000) 

int percentage;

void setupMG811()
{
  Serial.print("MG811(CO2) Sensor Calibrating... ");
  pinMode(MG811_BOOL_PIN, INPUT);                        //set pin to input
  digitalWrite(MG811_BOOL_PIN, HIGH);                    //turn on pullup resistors
  Serial.println("Initialized!");
  delay(100);
}

void loopMG811()
{
  float volts;

  volts = MG811Read(MG811_PIN);
  //  Serial.print( "SEN-00007:" );
  //  Serial.print(volts); 
  //  Serial.print( "V           " );
  percentage = MG811GetPercentage(volts, MG811_CO2Curve);
  //Serial.print("CO2:");
  if (percentage == -1) {
    percentage = 0;
  } 
  else {
    //Serial.print(percentage);
  }
  //Serial.print( "ppm" );  
  //Serial.print("\n");
  if (digitalRead(MG811_BOOL_PIN) ){
    //Serial.print( "=====BOOL is HIGH======" );
  } 
  else {
    //Serial.print( "=====BOOL is LOW======" );
  }
  //Serial.print("\n");

  mg811_C02_Final = (long int) percentage;
  Serial.print("MG811 CO2: ");
  Serial.print(mg811_C02_Final);
  Serial.println(" ppm");

  /*
  Z_OSCMessage message;
  message.setAddress(destIp, destPort);
  message.setZ_OSCMessage("/MG811_CO2", "i", &mg811_C02_Final);
  oscClient.send(&message);
  message.flush();
  */

  ledMG811();
  delay(1000);
}

/*****************************  MGRead *********************************************
 * Input:   mg_pin - analog channel
 * Output:  output of SEN-000007
 * Remarks: This function reads the output of SEN-000007
 ************************************************************************************/
float MG811Read(int mg_pin)
{
  int i;
  float v=0;

  for (i=0;i<MG811_READ_SAMPLE_TIMES;i++) {
    v += analogRead(mg_pin);
    delay(MG811_READ_SAMPLE_INTERVAL);
  }
  v = (v/MG811_READ_SAMPLE_TIMES) *5/1024 ;
  return v;  
}

/*****************************  MQGetPercentage **********************************
 * Input:   volts   - SEN-000007 output measured in volts
 * pcurve  - pointer to the curve of the target gas
 * Output:  ppm of the target gas
 * Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
 * of the line could be derived if y(MG-811 output) is provided. As it is a 
 * logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
 * value.
 ************************************************************************************/
int  MG811GetPercentage(float volts, float *pcurve)
{
  if ((volts/MG811_DC_GAIN )>=MG811_ZERO_POINT_VOLTAGE) {
    return -1;
  } 
  else { 
    return pow(10, ((volts/MG811_DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
  }
}

void ledMG811(){
  //   //////////////////////////////////////////////////////////////////////
  //   // LED POSITION B2 --- CO2
  //   //////////////////////////////////////////////////////////////////////  
  colorWipe(5, 6, strip.Color(0, 0, 0), 0);
  colorWipe(13, 14, strip.Color(0, 0, 0), 0);
  colorWipe(21, 22, strip.Color(0, 0, 0), 0);
  colorWipe(29, 30, strip.Color(0, 0, 0), 0);
  colorWipe(37, 38, strip.Color(0, 0, 0), 0);
  
  if (mg811_C02_Final<=400) {
    colorWipe(5, 6, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(13, 14, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(21, 22, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(29, 30, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(37, 38, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  }
  else if (mg811_C02_Final>400 && mg811_C02_Final<=600) {
    //colorWipe(5, 6, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    colorWipe(13, 14, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(21, 22, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(29, 30, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(37, 38, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
  }
  else if (mg811_C02_Final>600 && mg811_C02_Final<=800) {
    //colorWipe(5, 6, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(13, 14, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    colorWipe(21, 22, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(29, 30, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(37, 38, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
  }
  else if (mg811_C02_Final>800 && mg811_C02_Final<=1000) {
    //colorWipe(5, 6, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(13, 14, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(21, 22, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    colorWipe(29, 30, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
    //colorWipe(37, 38, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
  }
  else if (mg811_C02_Final>1000) {
    //colorWipe(5, 6, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(13, 14, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(21, 22, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(29, 30, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
    colorWipe(37, 38, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
  }
}
