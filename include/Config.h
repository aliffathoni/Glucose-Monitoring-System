#ifndef CONFIG_h_
#define CONFIG_h_

String DEFAULT_SSID = "3DPractical";
String DEFAULT_PASS = "embeddedelectronics";
String DEFAULT_UUID = "abcdefg";

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

int bpm, spo2, glucose;
String fuzzy_result;

#include <SPI.h>
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

#include <PNGdec.h>
#include "Image.h" // Image is stored here in an 8 bit array

PNG png; // PNG decoder inatance

#include <Wire.h>
#include "driver/rtc_io.h"
#include "MAX30100_PulseOximeter.h"

PulseOximeter pox;

void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[240];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(57, 5 + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void boot_init(){
  // tft.setFreeFont(&FreeSans9pt7b);
  tft.fillScreen(0x000000);
  int16_t rc = png.openFLASH((uint8_t *)logo, sizeof(logo), pngDraw);
  if (rc == PNG_SUCCESS) {
    tft.startWrite();
    rc = png.decode(NULL, 0);
    tft.endWrite();
  }
  delay(2000);
}

void backToSleep(){
  if(digitalRead(27) == LOW){
    
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Button", 80, 25, 4);
    tft.drawString("Pressed", 80, 55, 4);
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    tft.fillScreen(TFT_BLACK);

    rtc_gpio_pullup_en(GPIO_NUM_27);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
    
    Serial.print("[" + String(millis())+"] ");
    Serial.println("Enter Sleeping Mode In 2 Second...");
    
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Sleeping...", 80, 40, 4);
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    esp_deep_sleep_start();
  }
}

#include "Network.h"
#include "Fuzzy.h"

#endif