/* Core function for reading the output of the ESP module. Based on ESP_READ_UNTIL written by YouTube user Kevin Darrah
 * 
 * 
 */

boolean readESP(const char keyword1[], int key_size, int to, byte mode) {

  to_start=millis();
  char data_in[5];
  int scratch_length=0;
  key_size--;

  
  for(byte i=0; i<key_size; i++) {

    while(!esp.available()) {  // stops function if timeout is reached
      if((millis()-to_start)>to) {
        //Serial.println(F("fail 1")); //for debug
        return 0;
      }
    }

    data_in[i]=esp.read();
    if(mode==1) {
      scratch_esp[scratch_length]=data_in[i];//starts at 1
      scratch_length++;
    }
  }


  while(1) {
    
    for(byte i=0; i<key_size; i++) {
      if(keyword1[i]!=data_in[i]) {
        break;
      }
      if(i==(key_size-1)){
        return 1;
      }
    }
    
    for(byte i=0; i<(key_size-1); i++) {
      data_in[i]=data_in[i+1];
    }

    while(!esp.available()){
      if((millis()-to_start)>to){
        // Serial.println(F("fail2"));
        return 0;
      }
    }

    data_in[(key_size-1)]=esp.read();

    if(mode==1){
      scratch_esp[scratch_length]=data_in[key_size-1];
      scratch_length++;
    }
    
  } // end of while(1)
  
  
} // slut funktion

