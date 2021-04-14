/*
/*******************Demo for MQ-8 Gas Sensor Module V1.1*****************************
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

/************************Hardware Related Macros************************************/
#define         MQ8_PIN                          (4)     //define which analog input channel you are going to use
#define         MQ8_RL_VALUE                     (10)    //define the load resistance on the board, in kilo ohms
#define         MQ8_RO_CLEAN_AIR_FACTOR          (9.21)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
//which is derived from the chart in datasheet

/***********************Software Related Macros************************************/
#define         MQ8_CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         MQ8_CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
//cablibration phase
#define         MQ8_READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         MQ8_READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
//normal operation

/**********************Application Related Macros**********************************/
#define         MQ8_GAS_H2                      (0)

/*****************************Globals***********************************************/
float           MQ8_H2Curve[3]  =  {
  2.3, 0.47,-0.82};    //two points are taken from the curve in datasheet. 
//with these two points, a line is formed which is "approximately equivalent" 
//to the original curve. 
//data format:{ x, y, slope}; point1: (lg200, 0.47), point2: (lg10000, -0.92) 

float           MQ8_Ro           =  10;                  //Ro is initialized to 10 kilo ohms

void setupMQ8() {
  Serial.print("MQ8(H2) Sensor Calibrating... ");                
  MQ8_Ro = MQ8Calibration(MQ8_PIN);                        //Calibrating the sensor. Please make sure the sensor is in clean air 
  //when you perform the calibration                    
  //Serial.print("Calibration is done...\n"); 
  //Serial.print("Ro=");
  //Serial.print(MQ8_Ro);
  //Serial.print("kohm");
  //Serial.print("\n");
  Serial.println("Initialized!");
  delay(100);
}

void loopMQ8()
{
//  Serial.print("H2:"); 
//  Serial.print(MQ8GetGasPercentage(MQ8Read(MQ8_PIN)/MQ8_Ro,MQ8_GAS_H2) );
//  Serial.print( "ppm" );
//  Serial.print("\n");
  
  mq8_H2_Final = (long int) MQ8GetGasPercentage(MQ8Read(MQ8_PIN)/MQ8_Ro,MQ8_GAS_H2);
  Serial.print("MQ8 H2: ");
  Serial.print(mq8_H2_Final);
  Serial.println(" ppm");

  /*
  Z_OSCMessage message;
  message.setAddress(destIp, destPort);
  message.setZ_OSCMessage("/MQ8_H2", "i", &mq8_H2_Final);
  oscClient.send(&message);
  message.flush();
  */

  delay(1000);
}

float getMq8H2(){
  return MQ8GetGasPercentage(MQ8Read(MQ8_PIN)/MQ8_Ro,MQ8_GAS_H2);
}

/****************** MQResistanceCalculation ****************************************
 * Input:   raw_adc - raw value read from adc, which represents the voltage
 * Output:  the calculated sensor resistance
 * Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
 * across the load resistor and its resistance, the resistance of the sensor
 * could be derived.
 ************************************************************************************/
float MQ8ResistanceCalculation(int raw_adc)
{
  return ( ((float)MQ8_RL_VALUE*(1023-raw_adc)/raw_adc));
}

/***************************** MQCalibration ****************************************
 * Input:   mq_pin - analog channel
 * Output:  Ro of the sensor
 * Remarks: This function assumes that the sensor is in clean air. It use  
 * MQResistanceCalculation to calculates the sensor resistance in clean air 
 * and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about 
 * 10, which differs slightly between different sensors.
 ************************************************************************************/
float MQ8Calibration(int mq_pin)
{
  int i;
  float val=0;

  for (i=0;i<MQ8_CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQ8ResistanceCalculation(analogRead(mq_pin));
    delay(MQ8_CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/MQ8_CALIBARAION_SAMPLE_TIMES;                   //calculate the average value

  val = val/MQ8_RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 
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
float MQ8Read(int mq_pin)
{
  int i;
  float rs=0;

  for (i=0;i<MQ8_READ_SAMPLE_TIMES;i++) {
    rs += MQ8ResistanceCalculation(analogRead(mq_pin));
    delay(MQ8_READ_SAMPLE_INTERVAL);
  }

  rs = rs/MQ8_READ_SAMPLE_TIMES;

  return rs;  
}

/*****************************  MQGetGasPercentage **********************************
 * Input:   rs_ro_ratio - Rs divided by Ro
 * gas_id      - target gas type
 * Output:  ppm of the target gas
 * Remarks: This function passes different curves to the MQGetPercentage function which 
 * calculates the ppm (parts per million) of the target gas.
 ************************************************************************************/
int MQ8GetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == MQ8_GAS_H2) {
    return MQ8GetPercentage(rs_ro_ratio, MQ8_H2Curve);
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
int  MQ8GetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10, (((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0]))); 
}
