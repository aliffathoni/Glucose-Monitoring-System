#include <Arduino.h>
#include "Config.h"

void upload(void *param)
{
  while (1)
  {
    int dataMillis = millis();
    if(uid!=""){
      String path = "/test/" + uid;
      
      bool sent_status = Firebase.setInt(fb, path, dataMillis);
      do{
        Serial.println(fb.errorReason().c_str());
        sent_status = Firebase.setInt(fb, path, dataMillis);
      } while(sent_status == false);
      
      Serial.print("[" + String(millis())+"] ");
      Serial.println("Data sent");
      // Serial.printf("Set int... %s\n", Firebase.setInt(fb, path, dataMillis) ? "ok" : fb.errorReason().c_str());
    } else{
      Serial.printf("Set int... %s\n", Firebase.setInt(fb, "/test/int", dataMillis) ? "ok" : fb.errorReason().c_str());
    }
    vTaskDelay(2500);
  }
}

void blink(void *param)
{
  while(1){
    digitalWrite(23, HIGH);
    vTaskDelay(250);
    digitalWrite(23, LOW);
    vTaskDelay(250);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(23, OUTPUT);
  Serial.begin(115200);
  
  Serial.print("[" + String(millis())+"] ");
  Serial.println("Connecting to saved connection...");
  wifi_CONNECT();
  // reconnect();
  
  delay(1000);
  Serial.print("[" + String(millis())+"] ");
  Serial.println("Setup Completed");

  xTaskCreate(upload, "Upload Task", 10000, NULL, 2, NULL);
  xTaskCreate(blink, "Blink Task", 1024, NULL, 1, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
