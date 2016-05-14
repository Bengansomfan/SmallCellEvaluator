/* Core for reading GPS, saves data into temp variable until good sentence is recieved.
 *  If a sentence recieved is good, then it parses it.
 * 
 */
void readGPS() {
  char c;
  clearGPS();
  gpsSerial.listen();
  while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence

    c=GPS.read();
  }
  if(GPS.parse(GPS.lastNMEA())) { //Parse latest good NMEA sentence
  }else {
    // Serial.println(F("GPSfail")); // debug
  }
}
