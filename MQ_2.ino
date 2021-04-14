/*******************Demo for MQ-2 Gas Sensor Module V1.1*****************************
 * Author:  Tiequan Shao: tiequan.shao@sandboxelectronics.com
 * Peng Wei:     peng.wei@sandboxelectronics.com
 * 
 * Lisence: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 * 
 * Note:    This piece of source code is supposed to be used as a demostration ONLY. More
 * sophisticated calibration is required for industrial field application. 
 * 
 * Sandbox Electronics    2013-05-01
 * 
/************************Hardware Related Macros************************************/
#define         MQ2_PIN                          (1)     //define which analog input channel you are going to use
#define         MQ2_RL_VALUE                     (5)     //define the load resistance on the board, in kilo ohms
#define         MQ2_RO_CLEAN_AIR_FACTOR          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
//which is derived from the chart in datasheet


/***********************Software Related Macros************************************/
#define         MQ2_CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         MQ2_CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
//cablibration phase
#define         MQ2_READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         MQ2_READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
//normal operation

/**********************Application Related Macros**********************************/
#define         MQ2_GAS_LPG                      (0)
#define         MQ2_GAS_CO                       (1)
#define         MQ2_GAS_SMOKE                    (2)


/*****************************Globals***********************************************/
float           MQ2_LPGCurve[3]  =  {
  2.3,0.21,-0.47};   //two points are taken from the curve. 
//with these two points, a line is formed which is "approximately equivalent"
//to the original curve. 
//data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59) 
float           MQ2_COCurve[3]  =  {
  2.3,0.72,-0.34};    //two points are taken from the curve. 
//with these two points, a line is formed which is "approximately equivalent" 
//to the original curve.
//data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15) 
float           MQ2_SmokeCurve[3] ={
  2.3,0.53,-0.44};    //two points are taken from the curve. 
//with these two points, a line is formed which is "approximately equivalent" 
//to the original curve.
//data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)                                                     
float           MQ2_Ro           =  10;                 //Ro is initialized to 10 kilo ohms

void setupMQ2() {
  Serial.print("MQ2(LPG,CO,Smoke) Sensor Calibrating... ");                
  MQ2_Ro = MQ2Calibration(MQ2_PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air 
  //when you perform the calibration                    
  //Serial.print("Done...\n"); 
  //Serial.print("Ro=");
  //Serial.print(MQ2_Ro);
  //Serial.print("kohm");
  //Serial.print("\n");
  Serial.println("Initialized!");
  delay(100);
}

void loopMQ2() {
  //  Serial.print("LPG:"); 
  //  Serial.print(MQ2GetGasPercentage(MQ2Read (MQ2_PIN)/MQ2_Ro, MQ2_GAS_LPG) );
  //  Serial.print( "ppm" );
  //  Serial.print("    ");   
  //  Serial.print("CO:"); 
  //  Serial.print(MQ2GetGasPercentage(MQ2Read (MQ2_PIN)/MQ2_Ro, MQ2_GAS_CO) );
  //  Serial.print( "ppm" );
  //  Serial.print("    ");   
  //  Serial.print("SMOKE:"); 
  //  Serial.print(MQ2GetGasPercentage(MQ2Read (MQ2_PIN)/MQ2_Ro, MQ2_GAS_SMOKE) );
  //  Serial.print( "ppm" );
  //  Serial.print("\n");

  mq2_LPG_Final = (long int) MQ2GetGasPercentage(MQ2Read (MQ2_PIN)/MQ2_Ro, MQ2_GAS_LPG);
  Serial.print("MQ2 LPG: ");
  Serial.print(mq2_LPG_Final);
  Serial.println(" ppm");

  /*
  Z_OSCMessage message1;
  message1.setAddress(destIp, destPort);
  message1.setZ_OSCMessage("/MQ2_LPG", "i", &mq2_LPG_Final);
  oscClient.send(&message1);
  message1.flush();
  //ledMQ2LPG();
  delay(1000);
  */

  mq2_CO_Final = (long int) MQ2GetGasPercentage(MQ2Read (MQ2_PIN)/MQ2_Ro, MQ2_GAS_CO);
  Serial.print("MQ2 CO: ");
  Serial.print(mq2_CO_Final);
  Serial.println(" ppm");

  /*
  Z_OSCMessage message2;
  message2.setAddress(destIp, destPort);
  message2.setZ_OSCMessage("/MQ2_CO", "i", &mq2_CO_Final);
  oscClient.send(&message2);
  message2.flush();
  delay(1000);
  */

  mq2_Smoke_Final = (long int) MQ2GetGasPercentage(MQ2Read (MQ2_PIN)/MQ2_Ro, MQ2_GAS_SMOKE);
  Serial.print("MQ2 Smoke: ");
  Serial.print(mq2_Smoke_Final);
  Serial.println(" ppm"); 
  /* 
  Z_OSCMessage message3;
  message3.setAddress(destIp, destPort);
  message3.setZ_OSCMessage("/MQ2_Smoke", "i", &mq2_Smoke_Final);
  oscClient.send(&message3);
  message3.flush();
  //ledMQ2Smoke();
  delay(1000);
  */
}

float getMq2LPG(){
  return MQ2GetGasPercentage(MQ2Read (MQ2_PIN)/MQ2_Ro, MQ2_GAS_LPG);
}

float getMq2CO(){
  return MQ2GetGasPercentage(MQ2Read (MQ2_PIN)/MQ2_Ro, MQ2_GAS_CO);
}

float getMq2Smoke(){
  return MQ2GetGasPercentage(MQ2Read (MQ2_PIN)/MQ2_Ro, MQ2_GAS_SMOKE);
}

/****************** MQResistanceCalculation ****************************************
 * Input:   raw_adc - raw value read from adc, which represents the voltage
 * Output:  the calculated sensor resistance
 * Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
 * across the load resistor and its resistance, the resistance of the sensor
 * could be derived.
 ************************************************************************************/
float MQ2ResistanceCalculation(int raw_adc) {
  return ( ((float) MQ2_RL_VALUE*(1023-raw_adc)/raw_adc));
}


/***************************** MQCalibration ****************************************
 * Input:   mq_pin - analog channel
 * Output:  Ro of the sensor
 * Remarks: This function assumes that the sensor is in clean air. It use  
 * MQResistanceCalculation to calculates the sensor resistance in clean air 
 * and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about 
 * 10, which differs slightly between different sensors.
 ************************************************************************************/
float MQ2Calibration(int mq_pin) {
  int i;
  float val=0;

  for (i=0;i<MQ2_CALIBARAION_SAMPLE_TIMES;i++) {         //take multiple samples
    val += MQ2ResistanceCalculation(analogRead(mq_pin));
    delay(MQ2_CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/MQ2_CALIBARAION_SAMPLE_TIMES;                   //calculate the average value

  val = val/MQ2_RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 
  //according to the chart in the datasheet 

  return val; 
}
/*****************************  MQRead *********************************************
 * Input:   mq_pin - analog channel
 * Output:  Rs of the sensor
 * Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
 * The Rs changes as the sensor is in the different consentration of the target
 * gas. The sample times and the time interval between samples could be configured
 * by changing the definition of the macros.
 ************************************************************************************/
float MQ2Read(int mq_pin) {
  int i;
  float rs=0;

  for (i=0;i<MQ2_READ_SAMPLE_TIMES;i++) {
    rs += MQ2ResistanceCalculation(analogRead(mq_pin));
    delay(MQ2_READ_SAMPLE_INTERVAL);
  }

  rs = rs/MQ2_READ_SAMPLE_TIMES;

  return rs;  
}

/*****************************  MQGetGasPercentage **********************************
 * Input:   rs_ro_ratio - Rs divided by Ro
 * gas_id      - target gas type
 * Output:  ppm of the target gas
 * Remarks: This function passes different curves to the MQGetPercentage function which 
 * calculates the ppm (parts per million) of the target gas.
 ************************************************************************************/
int MQ2GetGasPercentage(float rs_ro_ratio, int gas_id) {
  if ( gas_id == MQ2_GAS_LPG ) {
    return MQ2GetPercentage(rs_ro_ratio,MQ2_LPGCurve);
  } 
  else if ( gas_id == MQ2_GAS_CO ) {
    return MQ2GetPercentage(rs_ro_ratio,MQ2_COCurve);
  } 
  else if ( gas_id == MQ2_GAS_SMOKE ) {
    return MQ2GetPercentage(rs_ro_ratio,MQ2_SmokeCurve);
  }    
  return 0;
}

/*****************************  MQ2GetPercentage **********************************
 * Input:   rs_ro_ratio - Rs divided by Ro
 * pcurve      - pointer to the curve of the target gas
 * Output:  ppm of the target gas
 * Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
 * of the line could be derived if y(rs_ro_ratio) is provided. As it is a 
 * logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
 * value.
 ************************************************************************************/
int  MQ2GetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10, (((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0]))); 
}

void ledMQ2LPG(){
  //   //////////////////////////////////////////////////////////////////////
  //   // LED POSITION B3 --- LPG
  //   //////////////////////////////////////////////////////////////////////

  //Reset from black
  colorWipe(20, 24, strip.Color(0, 0, 0), 0);
  delay(100);

  if (mq2_LPG_Final<=100) {


  }
  else if (mq2_LPG_Final>100 && mq2_LPG_Final<=300) {


  }
  else if (mq2_LPG_Final>300 && mq2_LPG_Final<=500) {


  }
  else if (mq2_LPG_Final>500 && mq2_LPG_Final<=700) {


  }
  else if (mq2_LPG_Final>700) {


    analogWrite(9, 90); //Beep also!!!
    delay(500);


    analogWrite(9, 0);
    delay(500);   


  }
}

void ledMQ2Smoke(){
  //   //////////////////////////////////////////////////////////////////////
  //   // LED POSITION B4 --- SMOKE
  //   //////////////////////////////////////////////////////////////////////

  //Reset from black
  colorWipe(28, 32, strip.Color(0, 0, 0), 0);
  delay(100);

  if (mq2_Smoke_Final<=10) {


  }
  else if (mq2_Smoke_Final>10 && mq2_Smoke_Final<=100) {


  }
  else if (mq2_Smoke_Final>100 && mq2_Smoke_Final<=300) {


  }
  else if (mq2_Smoke_Final>300 && mq2_Smoke_Final<=500) {


  }
  else if (mq2_Smoke_Final>500) {


    analogWrite(9, 90); //Beep also!!!
    delay(500);


    analogWrite(9, 0);
    delay(500);


  }
}
