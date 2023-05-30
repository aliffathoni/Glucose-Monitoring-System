#include <Arduino.h>
#include "Config.h"

TaskHandle_t upload_handle;

unsigned long lastUpload = 0;
int counter = 1;

void upload(void *param)
{
  for(;;){
    while (counter <= 20)
    {
      int dataMillis = millis();
      if(uid!=""){
        String path = "/test/" + uid + "/";
        if(counter < 10){
          path+="00" + String(counter);
        } else if(counter < 100){
          path+="0" + String(counter);
        } else{
          path+=String(counter);
        }
        
        bool sent_status = Firebase.setInt(fb, path, dataMillis);
        do{
          Serial.println(fb.errorReason().c_str());
          sent_status = Firebase.setInt(fb, path, dataMillis);
        } while(sent_status == false);
        
        Serial.print("[" + String(millis())+"] ");
        Serial.print("[" + String(counter)+"] ");
        
        Serial.println("Data sent");
        counter++;
        // Serial.printf("Set int... %s\n", Firebase.setInt(fb, path, dataMillis) ? "ok" : fb.errorReason().c_str());
      } else{
        Serial.printf("Set int... %s\n", Firebase.setInt(fb, "/test/int", dataMillis) ? "ok" : fb.errorReason().c_str());
      }

      lastUpload = millis();

      if(counter == 20){
        Serial.print("[" + String(millis())+"] ");
        Serial.println("Pause upload task");
      }
      vTaskDelay(700);
    }
    // vTaskSuspend(NULL);
    Serial.print("[" + String(millis())+"] ");
    vTaskDelay(1000);
  }
}

void blink(void *param)
{
  while(1){
    if(millis() - lastUpload > 30000){
      // vTaskResume(upload_handle);
      counter = 1;
      Serial.print("[" + String(millis())+"] ");
      Serial.println("Resume upload task");
    }

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

  xTaskCreate(upload, "Upload Task", 10000, NULL, 2, &upload_handle);
  xTaskCreate(blink, "Blink Task", 5000, NULL, 1, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
