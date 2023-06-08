#ifndef GRAPHIC_h_
#define GRAPHIC_h_

#include <SPI.h>
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite bgSprite= TFT_eSprite(&tft);
TFT_eSprite imgSprite= TFT_eSprite(&tft);
TFT_eSprite rectSprite= TFT_eSprite(&tft);
TFT_eSprite textSprite= TFT_eSprite(&tft);

#include "Image.h"

#define YELLOW 0x07DF //yellow
#define GREEN 0x001F //red
#define RED TFT_GREEN

void initTFT(){
    tft.init();
    tft.setRotation(2);
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);

    bgSprite.createSprite(80, 160);
    bgSprite.setSwapBytes(true);
    
    imgSprite.createSprite(80, 160);
    rectSprite.createSprite(80, 160);
    textSprite.createSprite(80, 160);
}

void drawValue(int spo2_value, int glu_value, int batt_value, bool net_state){
    bgSprite.fillSprite(TFT_BLACK);

    imgSprite.fillSprite(TFT_BLACK);

    imgSprite.setTextDatum(TL_DATUM);
    if(net_state == false){
        imgSprite.pushImage(3, 0, 15, 15, _online);
    }
    
    // if(digitalRead(15) == LOW){
    //   imgSprite.pushImage(62, 0, 15, 15, batt_charging);
    // } else{
        if(batt_value > 99){
            imgSprite.pushImage(62, 0, 15, 15, batt_full);
        } else if(batt_value < 100 && batt_value >= 85){
            imgSprite.pushImage(62, 0, 15, 15, batt_85);
        } else if(batt_value < 85 && batt_value >= 50){
            imgSprite.pushImage(62, 0, 15, 15, batt_50);
        } else if(batt_value < 50 && batt_value >= 20){
            imgSprite.pushImage(62, 0, 15, 15, batt_20);
        } else{
            imgSprite.pushImage(62, 0, 15, 15, batt_5);
        }
    // }
    imgSprite.pushToSprite(&bgSprite, 0, 0, TFT_BLACK);

    textSprite.fillSprite(TFT_BLACK);

    textSprite.setTextDatum(TL_DATUM);
    textSprite.setTextColor(TFT_ORANGE, TFT_BLACK);
    textSprite.drawString("Oksigen", 3, 20, 2);
    textSprite.setTextColor(TFT_PURPLE, TFT_BLACK);
    textSprite.drawString("Glukosa", 3, 95, 2);
    
    textSprite.fillRect(3, 91, 74, 2, TFT_WHITE);
    textSprite.fillRect(3, 17, 74, 2, TFT_WHITE);

    textSprite.setTextDatum(BR_DATUM);
    if(spo2_value >= 95){
      textSprite.setTextColor(GREEN, TFT_BLACK);
    } else if(spo2_value < 95 && spo2_value > 90){
      textSprite.setTextColor(YELLOW, TFT_BLACK);
    } else{
      textSprite.setTextColor(RED, TFT_BLACK);
    }
    textSprite.drawString(String(spo2_value), 80, 87, 7);
    
    if(glu_value > 140){
      textSprite.setTextColor(RED, TFT_BLACK);
    } else if(glu_value <= 140 && glu_value > 90){
      textSprite.setTextColor(GREEN, TFT_BLACK);
    } else{
      textSprite.setTextColor(YELLOW, TFT_BLACK);
    }
    textSprite.drawString(String(glu_value), 80, 160, 7);
    textSprite.pushToSprite(&bgSprite, 0, 0, TFT_BLACK);
    
    bgSprite.pushSprite(0,0);
}

void drawInit(){
    bgSprite.pushImage(0, 0, 80, 160, _init_device); 
    bgSprite.pushSprite(0,0);
    delay(1500);
}

void drawInitialized(){
    bgSprite.pushImage(0, 0, 80, 160, _initialized); 
    bgSprite.pushSprite(0,0);
    delay(1500);
}

void drawSleep(){
    bgSprite.pushImage(0, 0, 80, 160, _sleep_mode); 
    bgSprite.pushSprite(0,0);
}

void drawAP(){
    bgSprite.pushImage(0, 0, 80, 160, _open_app); 
    bgSprite.pushSprite(0,0);
}

void drawConnect(){
    bgSprite.pushImage(0, 0, 80, 160, _connect_network); 
    bgSprite.pushSprite(0,0);
    delay(1500);
}

void drawOffline(){
    bgSprite.pushImage(0, 0, 80, 160, _offline_mode); 
    bgSprite.pushSprite(0,0);
    delay(1500);
}

void drawError(){
    bgSprite.pushImage(0, 0, 80, 160, _device_error); 
    bgSprite.pushSprite(0,0);
    delay(1500);
}

void drawLogo(){
    bgSprite.pushImage(0, 0, 80, 80, _3dp_logo); 
    bgSprite.pushImage(0, 80, 80, 160, app_logo);
    bgSprite.pushSprite(0,0);
    delay(2000);
}

#endif