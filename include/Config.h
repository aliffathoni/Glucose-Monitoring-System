#ifndef CONFIG_h_
#define CONFIG_h_

#include <Arduino.h>

String DEFAULT_SSID = "Raddinii";
String DEFAULT_PASS = "raddinii0711";
String DEFAULT_UUID = "abcdefg";

const float bpm_calibration = 0.7;
const float spo_calibration = 1.0;
const float glu_calibration = 1.0;

String ssid, pass, uid;
bool connection_status = false;
int counter = 1;
unsigned long lastUpdate = 0;
unsigned long lastUpload = 0;
unsigned long changeState = 0;
bool update_state = true;

String timeNow;
int dataCounter = 1;
int bpm_rate = 0;
int spo_rate = 0;

int bpm, spo2, glucose, batt;
String fuzzy_result;

bool offline_mode = false;

const int max_volt_batt = 3134;
const int min_volt_batt = 2800;

#include "Graphic.h"

#include <Wire.h>
#include "driver/rtc_io.h"
#include "MAX30100_PulseOximeter.h"

PulseOximeter pox;

void backToSleep(){
  if(digitalRead(27) == LOW){
    
    drawSleep();

    rtc_gpio_pullup_en(GPIO_NUM_27);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
    
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Enter Sleeping Mode In 2 Second...");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    esp_deep_sleep_start();
  }
}

#include "Network.h"
#include "Fuzzy.h"

#endif