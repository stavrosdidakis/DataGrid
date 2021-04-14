#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

//Adafruit Library for TSL2561
//http://www.adafruit.com/products/439
//http://learn.adafruit.com/tsl2561/overview
//https://github.com/adafruit/TSL2561-Arduino-Library

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
 sensor API sensor_t type (see Adafruit_Sensor for more information)
 */
/**************************************************************************/
void displaySensorDetails(void) {
  sensor_t sensor;
  tsl.getSensor(&sensor);
  //  Serial.println("------------------------------------");
  //  Serial.print  ("Sensor:       "); 
  //  Serial.println(sensor.name);
  //  Serial.print  ("Driver Ver:   "); 
  //  Serial.println(sensor.version);
  //  Serial.print  ("Unique ID:    "); 
  //  Serial.println(sensor.sensor_id);
  //  Serial.print  ("Max Value:    "); 
  //  Serial.print(sensor.max_value); 
  //  Serial.println(" lux");
  //  Serial.print  ("Min Value:    "); 
  //  Serial.print(sensor.min_value); 
  //  Serial.println(" lux");
  //  Serial.print  ("Resolution:   "); 
  //  Serial.print(sensor.resolution); 
  //  Serial.println(" lux");  
  //  Serial.println("------------------------------------");
  //  Serial.println("");
  //delay(500);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
 */
/**************************************************************************/
void configureSensor(void) {
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  //tsl.enableAutoGain(true);          /* Auto-gain ... switches automatically between 1x and 16x */

  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */
  //  Serial.println("------------------------------------");
  //  Serial.print  ("Gain:         "); 
  //  Serial.println("Auto");
  //  Serial.print  ("Timing:       "); 
  //  Serial.println("13 ms");
  //  Serial.println("------------------------------------");
}

void setupLight() {
  Serial.print("Light Sensor Calibrating... ");
  /* Initialise the sensor */
  if(!tsl.begin()) { 
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Setup the sensor gain and integration time */
  configureSensor();

  Serial.println("Initialized!");
  delay(100);
}

void loopLight() {
  /* Get a new sensor event */
  sensors_event_t event;
  tsl.getEvent(&event);

  /* Display the results (light is measured in lux) */
  if (event.light) {
    lightFinal = event.light;

    Serial.print("Light: ");
    Serial.print(lightFinal, 2);
    Serial.println(" lux");

    /*
    Z_OSCMessage message;
    message.setAddress(destIp, destPort);
    message.setZ_OSCMessage("/Light", "f", &lightFinal, 2);
    oscClient.send(&message);
    message.flush();
    */
  }
  else {
    lightFinal = 0;
    /* If event.light = 0 lux the sensor is probably saturated
     and no reliable data could be generated! */
    Serial.println("Light: Sensor overload");
  } 
  ledLight();
  delay(1000);
}

void ledLight(){
  //  //////////////////////////////////////////////////////////////////////
  //  // LED POSITION A3 --- LIGHT
  //  //////////////////////////////////////////////////////////////////////
  colorWipe(2, 3, strip.Color(0, 0, 0), 0);
  colorWipe(10, 11, strip.Color(0, 0, 0), 0);
  colorWipe(18, 19, strip.Color(0, 0, 0), 0);
  colorWipe(26, 27, strip.Color(0, 0, 0), 0);
  colorWipe(34, 35, strip.Color(0, 0, 0), 0);
  
  if (lightFinal<=10){
    colorWipe(2, 3, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(10, 11, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(18, 19, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(26, 27, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
    //colorWipe(34, 35, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  } 
  else if (lightFinal>10 && lightFinal<=100){
    //colorWipe(2, 3, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    colorWipe(10, 11, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(18, 19, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(26, 27, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
    //colorWipe(34, 35, strip.Color(ledColorPurpleR, ledColorPurpleG, ledColorPurpleB), 0);
  } 
  else if (lightFinal>100 && lightFinal<=300){
    //colorWipe(2, 3, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(10, 11, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    colorWipe(18, 19, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(26, 27, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0);
    //colorWipe(34, 35, strip.Color(ledColorYellowR, ledColorYellowG, ledColorYellowB), 0); 
  } 
  else if (lightFinal>100 && lightFinal<=300){
    //colorWipe(2, 3, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(10, 11, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    //colorWipe(18, 19, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);
    colorWipe(26, 27, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
    //colorWipe(34, 35, strip.Color(ledColorOrangeR, ledColorOrangeG, ledColorOrangeB), 0);   
  }
  else if (lightFinal>300){
    //colorWipe(2, 3, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(10, 11, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(18, 19, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0);
    //colorWipe(26, 27, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
    colorWipe(34, 35, strip.Color(ledColorRedR, ledColorRedG, ledColorRedB), 0); 
  }
}
