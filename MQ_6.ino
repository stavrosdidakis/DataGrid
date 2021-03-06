/*******************Demo for MQ-6 Gas Sensor Module V1.1*****************************
 * Author:  Tiequan Shao: tiequan.shao@sandboxelectronics.com
 * Peng Wei:     peng.wei@sandboxelectronics.com
 * 
 * Lisence: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 * 
 * Note:    This piece of source code is supposed to be used as a demostration ONLY. More
 * sophisticated calibration is required for industrial field application. 
 * 
 * Sandbox Electronics    2013-05-01
 ************************************************************************************/

//http://sandboxelectronics.com/store/index.php?main_page=product_info&cPath=66&products_id=188&zenid=749600klq0nnst34f31ng3jaa5

/************************Hardware Related Macros************************************/
#define         MQ6_PIN                          (2)     //define which analog input channel you are going to use
#define         MQ6_RL_VALUE                     (20)    //define the load resistance on the board, in kilo ohms
#define         MQ6_RO_CLEAN_AIR_FACTOR          (10)    //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
//which is derived from the chart in datasheet
/***********************Software Related Macros************************************/
#define         MQ6_CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         MQ6_CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
//cablibration phase
#define         MQ6_READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         MQ6_READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
//normal operation

/**********************Application Related Macros**********************************/
#define         MQ6_GAS_LPG                      (0)
#define         MQ6_GAS_CH4                      (1)

/*****************************Globals***********************************************/
float           MQ6_LPGCurve[3]  =  {
  3,   0,  -0.4};    //two points are taken from the curve. 
//with these two points, a line is formed which is "approximately equivalent"
//to the original curve. 
//data format:{ x, y, slope}; point1: (lg1000, lg1), point2: (lg10000, lg0.4) 
float           MQ6_CH4Curve[3]  =  {
  3.3, 0,  -0.38};   //two points are taken from the curve. 
//with these two points, a line is formed which is "approximately equivalent" 
//to the original curve.
//data format:{ x, y, slope}; point1: (lg2000, lg1), point2: (lg5000,  lg0.7) 
float           MQ6_Ro           =  10;                 //Ro is initialized to 10 kilo ohms

void setupMQ6(){
  Serial.print("MQ6(LPG,CH4) Sensor Calibrating... ");                
  MQ6_Ro = MQ6Calibration(MQ6_PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air 
  //when you perform the calibration                    
  //Serial.print("Done...\n"); 
  //Serial.print("Ro=");
  //Serial.print(MQ6_Ro);
  //Serial.print("kohm");
  //Serial.print("\n");
  Serial.println("Initialized!");
  delay(100);
}

void loopMQ6(){
  //  Serial.print("LPG:"); 
  //  Serial.print(MQ6GetGasPercentage(MQ6Read(MQ6_PIN)/MQ6_Ro, MQ6_GAS_LPG) );
  //  Serial.print( "ppm" );
  //  Serial.print("        ");   
  //  Serial.print("CH4::"); 
  //  Serial.print(MQ6GetGasPercentage(MQ6Read(MQ6_PIN)/MQ6_Ro, MQ6_GAS_CH4) );
  //  Serial.print( "ppm" );
  //  Serial.print("\n");

  mq6_LPG_Final = (long int) MQ6GetGasPercentage(MQ6Read(MQ6_PIN)/MQ6_Ro, MQ6_GAS_LPG);
  Serial.print("MQ6 LPG: ");
  Serial.print(mq6_LPG_Final);
  Serial.println(" ppm");

  /*
  Z_OSCMessage message1;
  message1.setAddress(destIp, destPort);
  message1.setZ_OSCMessage("/MQ6_LPG", "i", &mq6_LPG_Final);
  oscClient.send(&message1);
  message1.flush();
  delay(1000);
  */
  
  mq6_CH4_Final = (long int) MQ6GetGasPercentage(MQ6Read(MQ6_PIN)/MQ6_Ro, MQ6_GAS_CH4);
  Serial.print("MQ6 CH4: ");
  Serial.print(mq6_CH4_Final);
  Serial.println(" ppm");

  /*
  Z_OSCMessage message2;
  message2.setAddress(destIp, destPort);
  message2.setZ_OSCMessage("/MQ6_CH4", "i", &mq6_CH4_Final);
  oscClient.send(&message2);
  message2.flush();
  delay(1000);
  */
}

float getMq6CH4(){
  return MQ6GetGasPercentage(MQ6Read(MQ6_PIN)/MQ6_Ro, MQ6_GAS_CH4);
}

/****************** MQResistanceCalculation ****************************************
 * Input:   raw_adc - raw value read from adc, which represents the voltage
 * Output:  the calculated sensor resistance
 * Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
 * across the load resistor and its resistance, the resistance of the sensor
 * could be derived.
 ************************************************************************************/
float MQ6ResistanceCalculation(int raw_adc)
{
  return ( ((float) MQ6_RL_VALUE*(1023-raw_adc)/raw_adc));
}


/***************************** MQCalibration ****************************************
 * Input:   mq_pin - analog channel
 * Output:  Ro of the sensor
 * Remarks: This function assumes that the sensor is in clean air. It use  
 * MQResistanceCalculation to calculates the sensor resistance in clean air 
 * and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about 
 * 10, which differs slightly between different sensors.
 ************************************************************************************/
float MQ6Calibration(int mq_pin)
{
  int i;
  float val=0;

  for (i=0;i<MQ6_CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQ6ResistanceCalculation(analogRead(mq_pin));
    delay(MQ6_CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/MQ6_CALIBARAION_SAMPLE_TIMES;                   //calculate the average value

  val = val/MQ6_RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 
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
float MQ6Read(int mq_pin)
{
  int i;
  float rs=0;

  for (i=0;i<MQ6_READ_SAMPLE_TIMES;i++) {
    rs += MQ6ResistanceCalculation(analogRead(mq_pin));
    delay(MQ6_READ_SAMPLE_INTERVAL);
  }

  rs = rs/MQ6_READ_SAMPLE_TIMES;

  return rs;  
}

/*****************************  MQGetGasPercentage **********************************
 * Input:   rs_ro_ratio - Rs divided by Ro
 * gas_id      - target gas type
 * Output:  ppm of the target gas
 * Remarks: This function passes different curves to the MQGetPercentage function which 
 * calculates the ppm (parts per million) of the target gas.
 ************************************************************************************/
int MQ6GetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == MQ6_GAS_LPG ) {
    return MQ6GetPercentage(rs_ro_ratio,MQ6_LPGCurve);
  } 
  else if ( gas_id == MQ6_GAS_CH4 ) {
    return MQ6GetPercentage(rs_ro_ratio,MQ6_CH4Curve);
  }    

  return 0;
}

/*****************************  MQGetPercentage **********************************
 * Input:   rs_ro_ratio - Rs divided by Ro
 * pcurve      - pointer to the curve of the target gas
 * Output:  ppm of the target gas
 * Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
 * of the line could be derived if y(rs_ro_ratio) is provided. As it is a 
 * logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
 * value.
 ************************************************************************************/
int  MQ6GetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10, (((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0]))); 
}
