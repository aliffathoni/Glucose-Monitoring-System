#include <Arduino.h>
#include "Config.h"

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  pinMode(23, OUTPUT);
  Serial.begin(115200);
  
  Serial.println("Connecting to saved connection...");
  wifi_CONNECT();
  // reconnect();
  
  delay(1000);
  Serial.println("Equation result : ");
  Serial.println(result);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(23, HIGH);
  int dataMillis = millis();
  if(uid!=""){
    String path = "/test/" + uid;
    
    bool sent_status = Firebase.setInt(fb, path, dataMillis);
    do{
      Serial.println(fb.errorReason().c_str());
      sent_status = Firebase.setInt(fb, path, dataMillis);
      delay(100);
    } while(sent_status == false);
    
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Data sent");
    // Serial.printf("Set int... %s\n", Firebase.setInt(fb, path, dataMillis) ? "ok" : fb.errorReason().c_str());
  } else{
    Serial.printf("Set int... %s\n", Firebase.setInt(fb, "/test/int", dataMillis) ? "ok" : fb.errorReason().c_str());
  }
  digitalWrite(23, LOW);
  delay(5000);
}
