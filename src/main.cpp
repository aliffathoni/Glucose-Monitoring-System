#include "Config.h"

TaskHandle_t main_handle;
TaskHandle_t sleep_handle;

void onBeatDetected(){
  Serial.println("..");
  //Add beat animation
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
    String path = "/Data/" + uid + "/Hasil pengukuran/Monitor";
    
    String path_bpm = path + "/Bpm/";
    bool sent_status = Firebase.setInt(fb, path_bpm, bpm);
    if(sent_status == false){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.println("BPM Data sent");
    }

    String path_spo2 = path + "/Oksigen/";
    sent_status = Firebase.setInt(fb, path_spo2, spo2);
    if(sent_status == false){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.println("SPO2 Data sent");
    }

    String path_glucose = path + "/Gula/";
    sent_status = Firebase.setInt(fb, path_glucose, glucose);
    if(sent_status == false){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.println("Glucose Data sent");
    }
  }
}

void update_data()
{
  if(millis() - lastUpdate > updateRoutine){
    bpm = pox.getHeartRate() * bpm_calibration;
    spo2 = pox.getSpO2() * spo_calibration;
    glucose = ((bpm * 0.8518) + 40.708) * glu_calibration;
    fuzzy_result = fuzzy_glucose(glucose);
    batt = (analogReadMilliVolts(36));
    
    if(bpm_rate != 0 && bpm != 0){
      if(bpm > 40 && bpm < 130){
        bpm_rate = (bpm_rate + bpm) / 2;
      }
    } else{
      bpm_rate = bpm;
    }

    if(bpm_rate > 199) bpm_rate = 199;
    if(spo2 > 100) spo2 = 100;
    if(glucose > 199){
      glucose = 199;
    } else if(glucose < 42) {
      glucose = 0;
    }

    Serial.print("[" + String(millis())+"] ");
    Serial.printf("BPM = %d || SPO2 = %d% || Glucose = %d || Fuzzy Result = %s || Battery = %d\n", bpm_rate, spo2, glucose, fuzzy_result, batt);
    
    drawValue(bpm_rate, spo2, glucose, fuzzy_result, batt, offline_mode);

    if(view_state > 50) view_state = 0;
    else view_state++;
    
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
      if(offline_mode == false && isDataError == false){
        pox.shutdown();
        
        upload();
        
        vTaskDelay(2000);
      
        pox.resume();
      }
      
      if(spo2 == 0 && bpm == 0){
        isDataError = true;

        Serial.print("[" + String(millis())+"] ");
        Serial.println("No data provided, check your finger  placement");
      } else{
        isDataError = false;

        Serial.print("[" + String(millis())+"] ");
        Serial.println("Data Sent");
      }

      update_state = true;
      changeState = millis();
    }

    if(millis() - changeState > uploadRoutine){
      Serial.print("[" + String(millis())+"] ");
      Serial.println("Upload State");
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
      
      drawSleep();

      rtc_gpio_pullup_en(GPIO_NUM_27);
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
      
      Serial.print("[" + String(millis())+"] ");
      Serial.println("Enter Sleeping Mode In 2 Second...");
      
      vTaskDelay(1000 / portTICK_PERIOD_MS);

      Serial.print("[" + String(millis())+"] ");
      Serial.println("Enter Sleeping Mode In 1 Second...");
      
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      
      esp_deep_sleep_start();
    }

    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(27, INPUT_PULLUP);
  
  initTFT();

  drawLogo();

  if(digitalRead(27) == LOW){
    offline_mode = true;
    
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Offline Mode Actived");

    drawOffline();
    delay(2000);
  }

  drawInit();
  
  if(offline_mode == false){
    drawConnect();
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Connecting to saved connection...");
    wifi_CONNECT();
    // reconnect();
  }
  delay(1000);
  
  wakeup_reason();

  drawInitialized();
  Serial.print("[" + String(millis())+"] ");
  Serial.println("Setup Completed");
 
  delay(1000);
  
  if(pox.begin()){
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Sensor Detected");
  } else {
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Sensor not detected");
    
    drawError();

    Serial.print("[" + String(millis())+"] ");
    Serial.println("Restart device");
    
    delay(500);

    ESP.restart();
  }

  pox.setOnBeatDetectedCallback(onBeatDetected);  
  changeState = millis();
  xTaskCreate(sleep_task, "Sleep Task", 5000, NULL, 2, &sleep_handle);
  xTaskCreate(main_task, "Main Task", 10000, NULL, 3, &main_handle);
}

void loop() {
  // put your main code here, to run repeatedly:
}
