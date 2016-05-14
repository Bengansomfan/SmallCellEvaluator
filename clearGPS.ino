/*
 * reads GPS info comming in and discards it
 */
void clearGPS() {  
char c;
gpsSerial.listen();
while(!GPS.newNMEAreceived()) {
  c=GPS.read();
  }
GPS.parse(GPS.lastNMEA());
}
