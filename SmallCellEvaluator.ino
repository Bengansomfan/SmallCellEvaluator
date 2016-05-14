/*
 *
 * Bachelor thesis SSYX02-16-15, Bengt Sjögren, Fredrik Johansson, Johan Larsson, Patrik Nilsson
 * 
 * This program evaluates a wi-fi connection between two ESP8266-modules over an area using 
 * GPS positioning (Adafruit GPS shield) and height determination using an ultrasonic range finder (HC-SR04).
 * 
 * The measured data is logged on a connected SD-card
 * 
 * For implementation in to your own project: define keywords and string in getRSSI to send
 * using info about the AP you want to evaluate. See reference guide for AT commands for ESP
 *  
*/

// import libraries for communication with the wifi-module, SD card and the Adafruit GPS shield
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_GPS.h>

// define Serial communication ports for ESP module and GPS module
SoftwareSerial esp(2,3);
SoftwareSerial gpsSerial(8, 7);

// define GPS object and an object for a file on the SD-card
Adafruit_GPS GPS(&gpsSerial);
File dataLog;

// KEYWORDS - make sure they are correct for the AP you want to observe
const char key_end[] = ",";
const char key_SSY[] = "X02\",-";

// global variables
#define GPSECHO false     // GPS will not return any messages to serial monitor
unsigned long to_start;   // Variable for starting timer
char scratch_esp[5];      // Array for holding scratch data 
char rssi[2];             // array to hold signal str. (2 symbols)
int chipSelect = 10;      // chipSelect pin for SD-card reader
int inputPin=4;           //ECHO pin 
int outputPin=5;          //TRIG pin 

void setup() {
  Serial.begin(9600);         // Our ESP modules both programmed to 9600 baud.
  pinMode(inputPin, INPUT); 
  pinMode(outputPin, OUTPUT); 
  // Serial.println(F("Sketch uploaded"));

  // Initiate GPS communication and configure GPS not to send any antenna info
  // Update rate from GPS is set to 1 Hz
  GPS.begin(9600);
  delay(1000);
  GPS.sendCommand("$PGCMD,33,0*6D");
  delay(1000);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  delay(1000);
 
  // initiate communication with esp
  esp.begin(9600); 
  delay(1000);
  
  // initiate SD, open a textfile and write restarted on a new line
  SD.begin(chipSelect);
  delay(1000);
  dataLog = SD.open("D4.txt", FILE_WRITE);
  dataLog.println("restarted");
  dataLog.close();
}

void loop() {
  esp.listen(); // Arduino can only communicate with one device at a time
  
  if(getRSSI()) { // if retrieval of signal strength info succeeds:
    readGPS();  // start listening to GPS and wait for a parse.
    
    if(GPS.fix==1) { // if fix is good, get height
      digitalWrite(outputPin, HIGH); //Trigger ultrasonic detection 
      delayMicroseconds(10); 
      digitalWrite(outputPin, LOW); 
      int distance = pulseIn(inputPin, HIGH); //Read ultrasonic reflection
      distance= distance/58; //Calculate distance

      // now try to open textfile, if successful, log everything
      dataLog = SD.open("D4.txt", FILE_WRITE);
      if(dataLog){
        for(int i=0;i<3;i++){
          dataLog.print(rssi[i]);
        }
        dataLog.print(",");
        dataLog.print(GPS.latitude,4);
        dataLog.print(","); // replace with GPS.lat, GPS.long to get info about hemisphere
        dataLog.print(GPS.longitude,4);
        dataLog.print(",");
        dataLog.println(distance);
        dataLog.close();
        //Serial.println(F("log!")); // for debugging
      }
    }
  }
  delay(500); // delay in order to get new GPS data.
}
