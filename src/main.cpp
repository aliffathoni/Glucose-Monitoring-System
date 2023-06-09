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
            if(offline_mode == false) counter++;
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
    if(sent_status == false){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.print("[" + String(counter)+"] ");
      Serial.println("BPM Data sent");
    }

    String path_spo2 = path + "/Oksigen/";
    sent_status = Firebase.setInt(fb, path_spo2, spo2);
    if(sent_status == false){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.print("[" + String(counter)+"] ");
      Serial.println("SPO2 Data sent");
    }

    String path_glucose = path + "/Gula/";
    sent_status = Firebase.setInt(fb, path_glucose, glucose);
    if(sent_status == false){
      Serial.println(fb.errorReason().c_str());
    } else{
      Serial.print("[" + String(millis())+"] ");
      Serial.print("[" + String(counter)+"] ");
      Serial.println("Glucose Data sent");
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

  if(millis() - lastUpdate > updateRoutine){    
    bpm = (bpm_rate / dataCounter) * bpm_calibration;
    spo2 = (spo_rate / dataCounter) * spo_calibration;
    glucose = (bpm * 140 / 120) * glu_calibration;
    fuzzy_result = fuzzy_glucose(glucose);
    batt = (analogReadMilliVolts(36));

    Serial.print("[" + String(millis())+"] ");
    Serial.printf("Total Data = %d || BPM = %d || SPO2 = %d% || Glucose = %d || Fuzzy Result = %s || Battery = %d\n", dataCounter, bpm, spo2, glucose, fuzzy_result, batt);
    
    drawValue(bpm, spo2, glucose, batt, offline_mode);

    dataCounter = 1;
    bpm_rate = 0;
    spo_rate = 0;
    
    view_state++;
    if(view_state > 18) view_state = 0;

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

    if(millis() - changeState > uploadRoutine){
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
      
      drawSleep();
      
      pf.begin("credentials", false);
      pf.putInt("counter", counter);
      pf.putString("time", timeNow);
      pf.end();

      rtc_gpio_pullup_en(GPIO_NUM_27);
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
      
      Serial.print("[" + String(millis())+"] ");
      Serial.println("Enter Sleeping Mode In 2 Second...");
      
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
  
  if(offline_mode == false){
    update_time();
  }
  
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
