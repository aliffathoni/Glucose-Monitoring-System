#ifndef CONFIG_h_
#define CONFIG_h_

#include <Arduino.h>

String DEFAULT_SSID = "Raddinii";
String DEFAULT_PASS = "raddinii0711";
String DEFAULT_UUID = "AV86BQf8DNZEhr5Uv7PiEHmkC4H3";
// String DEFAULT_UUID = "YjrQiZOD0FXtYsXNOQHAE2vJ4pv2";

const float bpm_calibration = 1.0;
const float spo_calibration = 1.0;
const float glu_calibration = 1.0;

String ssid, pass, uid;
bool connection_status = false;
int counter = 1;
unsigned long lastUpdate = 0;
unsigned long changeState = 0;
bool update_state = true;

bool isDataError = true;
int bpm, spo2, glucose, batt;
String fuzzy_result;

int view_state = 0;

bool offline_mode = false;

const int max_volt_batt = 3134;
const int min_volt_batt = 2800;

unsigned long networkTimeout = 180000;
unsigned long updateRoutine = 200;
unsigned long uploadRoutine = 2000;

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