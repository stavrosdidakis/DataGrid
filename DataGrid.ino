/*
Current Values:
 ArduinoMega - x
 Ethernet - x
 TempHumid - 1mA
 Light - 0.6mA
 Sound 20mA 
 MQ2 - 150mA
 MQ6 - 150mA
 MQ7 - 150mA
 MQ8 - 150mA
 MG811 - 200mA
 Dust - 90mA
 Radiation - 200mA
 Neopixel - 800mA - 2.4A
 */

/////////////////////////////////////////////////////
//
//Setup communication - Ethernet, and OSC protocol
//
/////////////////////////////////////////////////////
//#include <Z_OSC.h>
#include <SPI.h>
#include <Ethernet.h>

//New OSC objects
//Z_OSCServer server;
//Z_OSCMessage *rcvMes;

//Setup the network preferences
byte myMac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0x54, 0x94 };
//byte myIp[]  = { 10, 0, 1, 17 };
int  serverPort  = 10000;
EthernetClient client;

/*
byte destIp[] =  { 
  172, 16, 83, 255 }; //Broadcast on the local network
int  destPort = 10000;
Z_OSCClient oscClient;
*/

/////////////////////////////////////////////////////
//
//Setup the SD card
//
/////////////////////////////////////////////////////
#include <SD.h>
// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;
//Strings to store headers and timestamp for the text file
//String headers;
//String timestamp;

/////////////////////////////////////////////////////
//
//Setup RTC 
//
/////////////////////////////////////////////////////
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
DateTime nowRTC;

/////////////////////////////////////////////////////
//Setup Variables
/////////////////////////////////////////////////////
float temperatureFinal = 0;
long int humidityFinal = 0;
float lightFinal = 0;
float soundDBFinal = 0;
long int mq2_LPG_Final = 0;
long int mq2_CO_Final = 0;
long int mq2_Smoke_Final = 0;
long int mq6_LPG_Final = 0;
long int mq6_CH4_Final = 0;
long int mq7Final = 0;
long int mq8_H2_Final = 0;
long int mg811_C02_Final = 0;
float dust_Concentration_Final = 0;
long int radiation_CPM_Final = 0;

/////////////////////////////////////////////////////
//
//Setup the NeoPixel Shield
//
/////////////////////////////////////////////////////
//NeoPixel library - control through DOUT 6
//http://learn.adafruit.com/adafruit-neopixel-uberguide/overview

#include <Adafruit_NeoPixel.h>
#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(40, PIN, NEO_GRB + NEO_KHZ800);
//Green
int ledColorGreenR = 0;
int ledColorGreenG = 255;
int ledColorGreenB = 0;
// Yellow
int ledColorYellowR = 255;
int ledColorYellowG = 255;
int ledColorYellowB = 0;
//Orange
int ledColorOrangeR = 255;
int ledColorOrangeG = 80;
int ledColorOrangeB = 0;
//Red
int ledColorRedR = 255;
int ledColorRedG = 0;
int ledColorRedB = 0;
int ledColorOff = 0;
//Purple
int ledColorPurpleR = 255;
int ledColorPurpleG = 255;
int ledColorPurpleB = 0;
//Blue
int ledColorBlueR = 0;
int ledColorBlueG = 0;
int ledColorBlueB = 255;

/////////////////////////////////////////////////////
//
//  Heroku
//
/////////////////////////////////////////////////////
char serverName[] = "hexspace.herokuapp.com";
String tempLink = "GET /api/device/mainlog/sensor/tempin/report?value=";
String humidLink = "GET /api/device/mainlog/sensor/humidin/report?value=";
String lightLink = "GET /api/device/mainlog/sensor/light/report?value=";
String soundLink = "GET /api/device/mainlog/sensor/sound/report?value=";
String mq2lpgLink = "GET /api/device/mainlog/sensor/mq2lpg/report?value=";
String mq2coLink = "GET /api/device/mainlog/sensor/mq2co/report?value=";
String mq2smokeLink = "GET /api/device/mainlog/sensor/mq2smoke/report?value=";
String mq6lpgLink = "GET /api/device/mainlog/sensor/mq6lpg/report?value=";
String mq6ch4Link = "GET /api/device/mainlog/sensor/mq6ch4/report?value=";
String mq7Link = "GET /api/device/mainlog/sensor/mq7co/report?value=";
String mq8Link = "GET /api/device/mainlog/sensor/mq8h2/report?value=";
String mg811Link = "GET /api/device/mainlog/sensor/mg811co2/report?value=";
String dustLink = "GET /api/device/mainlog/sensor/dust/report?value=";
String radiationLink = "GET /api/device/mainlog/sensor/radiation/report?value=";

/*********************** SETUP ***********************/
void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600); 

  setupNeopixel();

  //Setup Communication - Ethernet & OSC
  Ethernet.begin(myMac);  
  if (Ethernet.begin(myMac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  //server.sockOpen(serverPort);
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    String IpAddress = String (Ethernet.localIP()[thisByte], DEC);
    Serial.print(IpAddress);
    Serial.println(""); 
  }
  
  /*
  /////////////////////////////////////////////////////
   // Initialize the SD card
   /////////////////////////////////////////////////////
   Serial.println("");
   Serial.print("Initializing SD card...");
   // make sure that the default chip select pin is set to
   // output, even if you don't use it:
   pinMode(10, OUTPUT);
   digitalWrite(10, HIGH);
   // see if the card is present and can be initialized:
   if (!SD.begin(chipSelect)) {
   Serial.println("Card failed, or not present");
   // don't do anything more:
   return;
   }
   Serial.println("card initialized.");
   */
   
  /////////////////////////////////////////////////////
  // Initialize the RealTime Clock
  /////////////////////////////////////////////////////
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  nowRTC = RTC.now();

  setupTempHumid();
  setupLight();
  setupSound();
  setupMQ2();
  setupMQ6();
  setupMQ7();
  setupMQ8();
  setupMG811();
  setupDust();
  setupRadiation();

  Serial.println("");
}

/*********************** MAIN ***********************/
void loop() {
  //Run Neopixel LED shield and speaker outside the minute() function

  nowRTC = RTC.now();
  //Scan sensor values every minute. Add the new value to the stack
  //if(nowRTC.second()==0, DEC){
  Serial.print("****** ");
  Serial.print("Time ");
  Serial.print(nowRTC.hour(), DEC);
  Serial.print(":");
  Serial.print(nowRTC.minute(), DEC);
  Serial.print(", ");
  Serial.print("Date, ");
  Serial.print(nowRTC.day(), DEC);
  Serial.print("/");
  Serial.print(nowRTC.month(), DEC);
  Serial.print("/");
  Serial.print(nowRTC.year(), DEC);
  Serial.print(" ******");
  Serial.println("");

  loopTempHumid();
  loopLight();
  loopSound();
  loopMQ2();    
  loopMQ6();
  loopMQ7();
  loopMQ8();
  loopMG811();
  loopDust();
  loopRadiation();

  delay(5000);

  Serial.println("");

  if(nowRTC.minute() == 0 || nowRTC.minute() == 5 || nowRTC.minute() == 10 || nowRTC.minute() == 15 
    || nowRTC.minute() == 20 || nowRTC.minute() == 25 || nowRTC.minute() == 30 || nowRTC.minute() == 35
    || nowRTC.minute() == 40 || nowRTC.minute() == 45 || nowRTC.minute() == 50 || nowRTC.minute() == 55){
    //Enable this for Data Logging on the SD card
    //dataLog();
    herokuUpload();
    delay(60000);
  }
}

void herokuUpload(){
  //Convert float to string using dtostrf of stdlib.h
  //Parameters are: (variable to be converted, width size, precision, buffer);
  char bufferTemp[25];
  String tempUpload = dtostrf (temperatureFinal, 4, 2, bufferTemp);
  String postDataTemp = tempLink+tempUpload+" "+"HTTP/1.1";
  Serial.println(postDataTemp);
  httpRequest(postDataTemp);
  delay(100);

  String postDataHumid = humidLink+humidityFinal+" "+"HTTP/1.1";
  Serial.println(postDataTemp);
  httpRequest(postDataHumid);
  delay(100); 

  char bufferLight[25];
  String lightUpload = dtostrf (lightFinal, 4, 2, bufferLight);
  String postDataLight = lightLink+lightUpload+" "+"HTTP/1.1";
  Serial.println(postDataLight);
  httpRequest(postDataLight);
  delay(100);

  char bufferSound[25];
  String soundUpload = dtostrf (soundDBFinal, 4, 2, bufferSound);
  String postDataSound = soundLink+soundUpload+" "+"HTTP/1.1";
  Serial.println(postDataSound);
  httpRequest(postDataSound);
  delay(100);

  String postDataMQ2LPG = mq2lpgLink+mq2_LPG_Final+" "+"HTTP/1.1";
  Serial.println(postDataMQ2LPG);
  httpRequest(postDataMQ2LPG);
  delay(100);

  String postDataMQ2CO = mq2coLink+mq2_CO_Final+" "+"HTTP/1.1";
  Serial.println(postDataMQ2CO);
  httpRequest(postDataMQ2CO);
  delay(100);

  String postDataMQ2Smoke = mq2smokeLink+mq2_Smoke_Final+" "+"HTTP/1.1";
  Serial.println(postDataMQ2Smoke);
  httpRequest(postDataMQ2Smoke);  
  delay(100);

  String postDataMQ6LPG = mq6lpgLink+mq6_LPG_Final+" "+"HTTP/1.1";
  Serial.println(postDataMQ6LPG);
  httpRequest(postDataMQ6LPG);
  delay(100);

  String postDataMQ6CH4 = mq6ch4Link+mq6_CH4_Final+" "+"HTTP/1.1";
  Serial.println(postDataMQ6CH4);
  httpRequest(postDataMQ6CH4);  
  delay(100);  

  String postDataMQ7 = mq7Link+mq7Final+" "+"HTTP/1.1";
  Serial.println(postDataMQ7);
  httpRequest(postDataMQ7);  
  delay(100);  

  String postDataMQ8 = mq8Link+mq8_H2_Final+" "+"HTTP/1.1";
  Serial.println(postDataMQ8);
  httpRequest(postDataMQ8);  
  delay(100);

  String postDataMG811 = mg811Link+mg811_C02_Final+" "+"HTTP/1.1";
  Serial.println(postDataMG811);
  httpRequest(postDataMG811);  
  delay(100);

  char bufferDust[25];
  String dustUpload = dtostrf (dust_Concentration_Final, 4, 2, bufferDust);
  String postDataDust = dustLink+dustUpload+" "+"HTTP/1.1";
  Serial.println(postDataDust);
  httpRequest(postDataDust);  
  delay(100); 

  String postDataRadiation = radiationLink+radiation_CPM_Final+" "+"HTTP/1.1";
  Serial.println(postDataRadiation);
  httpRequest(postDataRadiation);  
  delay(100);
}

void httpRequest(String postData){
  if (client.connect(serverName, 80) == 1 ) {  //starts client connection, checks for connection
    Serial.println("connected");

    nowRTC = RTC.now();
    Serial.print("****** ");
    Serial.print("Time ");
    Serial.print(nowRTC.hour(), DEC);
    Serial.print(":");
    Serial.print(nowRTC.minute(), DEC);
    Serial.print(", ");
    Serial.print("Date, ");
    Serial.print(nowRTC.day(), DEC);
    Serial.print("/");
    Serial.print(nowRTC.month(), DEC);
    Serial.print("/");
    Serial.print(nowRTC.year(), DEC);
    Serial.print(" ******");
    Serial.println("");

    client.println(postData);
    client.println("Host: hexspace.herokuapp.com");
    client.println("Connection: close");  //close 1.1 persistent connection  
    client.println(); //end of get request
  } 
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  int connectLoop = 0;

  while(client.connected()){
    while(client.available()){
      char c = client.read(); //gets byte from ethernet buffer
      Serial.print(c); //prints byte to serial monitor 
      connectLoop = 0;
    }

    //Use the connect loop to disconnect if connection hangs
    connectLoop++;
    if(connectLoop > 10000) {
      // then close the connection from this end.
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
    // this is a delay for the connectLoop timing
    delay(1);
  }
  Serial.println();
  Serial.println(F("disconnecting."));
  Serial.println("==================");
  Serial.println();
  // close client end
  client.stop();
}

/********************** DATALOGGING **********************/
void dataLog(){
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    /////////////////////////
    // TEMPERATURE
    /////////////////////////
    dataFile.print("Temperature (logged), ");
    dataFile.print(temperatureFinal, 2);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // HUMIDITY
    /////////////////////////
    dataFile.print("Humidity (logged), ");
    dataFile.print(humidityFinal);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // LIGHT
    /////////////////////////    
    dataFile.print("Light (logged), ");
    dataFile.print(lightFinal, 2);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // SOUND
    /////////////////////////
    dataFile.print("Sound (logged), ");
    dataFile.print(soundDBFinal, 2);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // MQ-2 LPG
    /////////////////////////
    dataFile.print("MQ2-LPG (logged), ");
    dataFile.print(mq2_LPG_Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // MQ-2 CO
    /////////////////////////
    dataFile.print("MQ2-CO (logged), ");
    dataFile.print(mq2_CO_Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // MQ-2 Smoke
    /////////////////////////
    dataFile.print("MQ2-Smoke (logged), ");
    dataFile.print(mq2_Smoke_Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // MQ-6 LPG
    /////////////////////////
    dataFile.print("MQ6-LPG (logged), ");
    dataFile.print(mq6_LPG_Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // MQ-6 CH4
    /////////////////////////
    dataFile.print("MQ6-CH4 (logged), ");
    dataFile.print(mq6_LPG_Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // MQ-7
    /////////////////////////
    dataFile.print("MQ7 (logged), ");
    dataFile.print(mq7Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // MQ-8
    /////////////////////////
    dataFile.print("MQ8 (logged), ");
    dataFile.print(mq8_H2_Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // MG-811
    /////////////////////////
    dataFile.print("MG-811 (logged), ");
    dataFile.print(mg811_C02_Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // Dust
    /////////////////////////
    dataFile.print("Dust (logged), ");
    dataFile.print(dust_Concentration_Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    /////////////////////////
    // Radiation
    /////////////////////////
    dataFile.print("Radiation (logged), ");
    dataFile.print(dust_Concentration_Final);
    dataFile.print(", ");
    dataFile.print("Time, ");
    dataFile.print(nowRTC.hour());
    dataFile.print(",");
    dataFile.print(nowRTC.minute());
    dataFile.print(", ");
    dataFile.print("Date, ");
    dataFile.print(nowRTC.day());
    dataFile.print(",");
    dataFile.print(nowRTC.month());
    dataFile.print(",");
    dataFile.println(nowRTC.year());

    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void setupNeopixel(){
  /////////////////////////////////////////////////////
  // Initialize the NeoPixel Shield
  /////////////////////////////////////////////////////
  strip.begin();
  strip.show();

  //declare pin 9 to be an output for the beep
  pinMode(9, OUTPUT);
  analogWrite(9, 90); //Beep also!!!
  colorWipe(0, 40, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  delay(50);
  analogWrite(9, 0);
  colorWipe(0, 40, strip.Color(0, 0, 0), 0);
  delay(50);  

  //declare pin 9 to be an output for the beep
  pinMode(9, OUTPUT);
  analogWrite(9, 90); //Beep also!!!
  colorWipe(0, 40, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  delay(50);
  analogWrite(9, 0);
  colorWipe(0, 40, strip.Color(0, 0, 0), 0);
  delay(50);  

  //declare pin 9 to be an output for the beep
  pinMode(9, OUTPUT);
  analogWrite(9, 90); //Beep also!!!
  colorWipe(0, 40, strip.Color(ledColorBlueR, ledColorBlueG, ledColorBlueB), 0);
  delay(50);
  analogWrite(9, 0);
  colorWipe(0, 40, strip.Color(0, 0, 0), 0);
  delay(50);  

  Serial.println("LED Shield Initialized!");
  Serial.println("Speaker Initialized!");
}

// Fill the dots one after the other with a color
void colorWipe(int tempX, int tempY, uint32_t c, uint8_t wait) {
  for(uint16_t i=tempX; i<tempY; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
