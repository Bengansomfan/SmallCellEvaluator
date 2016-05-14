/*
 * This function will send the AT command for listing a specific network
 * and then start listening to the response. it will record the specific
 * data comming from the ESP-unit that shows the recieved signal strength
 * in dBm.

*/
boolean getRSSI() {

  //Serial.println(F("getRSSI initiated")); // for debugging
  esp.print(F("AT+CWLAP=\"SSYX02\",\"1a:fe:34:d5:95:8f\",1\r\n"));
  if(readESP(key_SSY, sizeof(key_SSY), 2000,0)) {
    if(readESP(key_end, sizeof(key_end),2000,1)) {
      for(int i=0; i<3; i++){
        rssi[i]=scratch_esp[i];
      }
      return 1;
    }
  }else {
    // Serial.println(F("failed getRSSI..")); for debugging
    return 0;
  }
}

