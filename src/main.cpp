#include <Arduino.h>
#include "Config.h"

TaskHandle_t main_handle;
TaskHandle_t sleep_handle;

void onBeatDetected(){
  Serial.println("..");
}

void wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0 :
            Serial.println("Wakeup caused by external signal using RTC_IO");
            break;
        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    }
}

void upload()
{
  int dataMillis = millis();
  if(uid!=""){
    String path = "/Data/" + uid + "/Hasil pengukuran/" + timeNow + "/";
    if(counter < 10){
      path+="00" + String(counter);
    } else if(counter < 100){
      path+="0" + String(counter);
    } else{
      path+=String(counter);
    }
    
    String path_bpm = path + "/Bpm/";
    bool sent_status = Firebase.setInt(fb, path_bpm, bpm);
    if(sent_status){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.print("[" + String(counter)+"] ");
      Serial.println("BPM Data sent");
    }

    String path_spo2 = path + "/Oksigen/";
    sent_status = Firebase.setInt(fb, path_spo2, spo2);
    if(sent_status){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.print("[" + String(counter)+"] ");
      Serial.println("SPO2 Data sent");
    }

    String path_glucose = path + "/Gula/";
    sent_status = Firebase.setInt(fb, path_glucose, glucose);
    if(sent_status){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.print("[" + String(counter)+"] ");
      Serial.println("Glucose Data sent");
    }
    
    String path_fuzzy = path + "/Status/";
    sent_status = Firebase.setString(fb, path_fuzzy, fuzzy_result);
    if(sent_status){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.print("[" + String(counter)+"] ");
      Serial.println("Fuzzy Data sent");
    }
  }
}

void update_time()
{
  timeClient.update();
  Serial.print("[" + String(millis())+"] ");
  Serial.println("[T] "+timeClient.getFormattedDate());
  
  if(timeNow != timeClient.getFormattedDate()){
    timeNow = timeClient.getFormattedDate();
    counter = 1;
  }

  tft.drawString(timeClient.getFormattedTime(), 80, 25, 4);
}

void update_data()
{
  int bpm_dump = pox.getHeartRate();
  int spo_dump = pox.getSpO2();
  
  if(bpm_dump != 0 && spo_dump != 0){
    dataCounter++;
    bpm_rate+=bpm_dump;
    spo_rate+=spo_dump;
  }

  if(millis() - lastUpdate > 300){    
    bpm = (bpm_rate / dataCounter) * bpm_calibration;
    spo2 = (spo_rate / dataCounter) * spo_calibration;
    glucose = (bpm * 140 / 120) * glu_calibration;
    fuzzy_result = fuzzy_glucose(glucose);

    Serial.print("[" + String(millis())+"] ");
    Serial.printf("Total Data = %d || BPM = %d || SPO2 = %d% || Glucose = %d || Fuzzy Result = %s\n", dataCounter, bpm, spo2, glucose, fuzzy_result);
    
    String printValue = "  SPO2 "+String(spo2)+"% | Glu "+String(glucose) + "  ";
    tft.drawString(printValue, 80, 55, 2);

    if(offline_mode == true){
      String fress = "  "+fuzzy_result+"  ";
      tft.drawString(fress, 80, 25, 4);
    }

    dataCounter = 1;
    bpm_rate = 0;
    spo_rate = 0;
    
    lastUpdate = millis();
  }
}

void main_task(void *param)
{
  while(1){
    if(update_state == true){
      pox.update();
      update_data();
    } else{
      if(offline_mode == false){
        pox.shutdown();
        update_time();
        
        if(glucose!=0 && spo2!=0){
          upload();
        }
      
        pox.resume();
      }
      
      update_state = true;
    }

    if(millis() - changeState > 5000){
      Serial.print("[" + String(millis())+"] ");
      Serial.println("Setup Completed");
      update_state = false;
      changeState = millis();
    }
  }
}

void sleep_task(void *param)
{
  while(1){
    if(digitalRead(27) == LOW){
      pox.shutdown();
      
      vTaskSuspend(main_handle);
      
      tft.fillScreen(TFT_BLACK);
      tft.drawString("Button", 80, 25, 4);
      tft.drawString("Pressed", 80, 55, 4);
      
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      tft.fillScreen(TFT_BLACK);
        
      pf.begin("credentials", false);
      pf.putInt("counter", counter);
      pf.putString("time", timeNow);
      pf.end();

      rtc_gpio_pullup_en(GPIO_NUM_27);
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
      
      Serial.print("[" + String(millis())+"] ");
      Serial.println("Enter Sleeping Mode In 2 Second...");
      
      tft.fillScreen(TFT_BLACK);
      tft.drawString("Sleeping...", 80, 40, 4);
      
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      
      esp_deep_sleep_start();
    }

    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(27, INPUT_PULLUP);
  
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(CC_DATUM);
  tft.setTextSize(1);

  boot_init();

  if(digitalRead(27) == LOW){
    offline_mode = true;
    
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Offline Mode Actived");

    tft.fillScreen(TFT_BLACK);
    tft.drawString("Offline", 80, 25, 4);
    tft.drawString("Mode", 80, 55, 4);
    delay(2000);
  }

  tft.fillScreen(TFT_BLACK);
  tft.drawString("Initialize", 80, 25, 4);
  tft.drawString("Device", 80, 55, 4);
  delay(2000);

  wakeup_reason();
  
  if(offline_mode == false){
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Connecting", 80, 25, 4);
    tft.drawString("Network", 80, 55, 4);
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Connecting to saved connection...");
    wifi_CONNECT();
    // reconnect();
  }
  delay(1000);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("Initialize", 80, 25, 4);
  tft.drawString("Completed!", 80, 55, 4);
  Serial.print("[" + String(millis())+"] ");
  Serial.println("Setup Completed");
 
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  if(offline_mode == false){
    update_time();
  }
  
  if(pox.begin()){
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Sensor Detected");
  }

  pox.setOnBeatDetectedCallback(onBeatDetected);  
  changeState = millis();
  xTaskCreate(sleep_task, "Sleep Task", 5000, NULL, 2, &sleep_handle);
  xTaskCreate(main_task, "Main Task", 10000, NULL, 3, &main_handle);
}

void loop() {
  // put your main code here, to run repeatedly:
}