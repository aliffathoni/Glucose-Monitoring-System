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

void drawValue(int bpm_value, int spo2_value, int glu_value, String glu_fuzzy, int batt_value, bool net_state){
    bgSprite.fillSprite(TFT_BLACK);

    imgSprite.fillSprite(TFT_BLACK);

    imgSprite.setTextDatum(TL_DATUM);
    if(net_state == false){
        imgSprite.pushImage(3, 0, 15, 15, _online);
    }
    
    if(batt_value >= 3128){
        imgSprite.pushImage(62, 0, 15, 15, batt_full);
    } else if(batt_value < 3128 && batt_value >= 3100){
        imgSprite.pushImage(62, 0, 15, 15, batt_85);
    } else if(batt_value < 3100 && batt_value >= 3000){
        imgSprite.pushImage(62, 0, 15, 15, batt_50);
    } else if(batt_value < 3000 && batt_value >= 2900){
        imgSprite.pushImage(62, 0, 15, 15, batt_20);
    } else{
        imgSprite.pushImage(62, 0, 15, 15, batt_5);
    }
    
    imgSprite.pushToSprite(&bgSprite, 0, 0, TFT_BLACK);

    textSprite.fillSprite(TFT_BLACK);

    textSprite.setTextDatum(TL_DATUM);
    if(view_state < 26){
        textSprite.setTextColor(YELLOW, TFT_BLACK);
        textSprite.drawString("Oksigen", 3, 20, 2);
    } else{
        textSprite.setTextColor(TFT_YELLOW, TFT_BLACK);
        textSprite.drawString("BPM", 3, 20, 2);
    }
    textSprite.setTextColor(TFT_PURPLE, TFT_BLACK);
    textSprite.drawString("Glukosa", 3, 95, 2);
    
    textSprite.fillRect(3, 91, 74, 2, TFT_WHITE);
    textSprite.fillRect(3, 17, 74, 2, TFT_WHITE);

    textSprite.setTextDatum(BR_DATUM);
    if(view_state < 26){
        if(spo2_value >= 95){
            textSprite.setTextColor(GREEN, TFT_BLACK);
        } else if(spo2_value < 95 && spo2_value >= 90){
            textSprite.setTextColor(YELLOW, TFT_BLACK);
        } else if(spo2_value < 90 && spo2_value > 0){
            textSprite.setTextColor(RED, TFT_BLACK);
        } else{
            textSprite.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        
        if(spo2_value != 0) textSprite.drawString(String(spo2_value), 80, 87, 7);
        else textSprite.drawString("--", 80, 87, 7);
    } else{
        if(bpm_value >= 120){
            textSprite.setTextColor(RED, TFT_BLACK);
        } else if(bpm_value < 120 && bpm_value >= 90){
            textSprite.setTextColor(YELLOW, TFT_BLACK);
        } else if(bpm_value < 90 && bpm_value >= 70){
            textSprite.setTextColor(GREEN, TFT_BLACK);
        } else if(bpm_value < 70 && bpm_value >= 50){
            textSprite.setTextColor(YELLOW, TFT_BLACK);
        } else if(bpm_value < 50 && bpm_value > 0){
            textSprite.setTextColor(RED, TFT_BLACK);
        } else{
            textSprite.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        
        if(bpm_value != 0) textSprite.drawString(String(bpm_value), 80, 87, 7);
        else textSprite.drawString("--", 80, 87, 7);
    }

    if(glu_fuzzy == "Tinggi"){
      textSprite.setTextColor(RED, TFT_BLACK);
    } else if(glu_fuzzy == "Rendah"){
      textSprite.setTextColor(YELLOW, TFT_BLACK);
    } else if(glu_fuzzy == "Normal"){
      textSprite.setTextColor(GREEN, TFT_BLACK);
    } else if(glu_fuzzy == "Error"){
      textSprite.setTextColor(RED, TFT_BLACK);
    } else{
      textSprite.setTextColor(TFT_WHITE, TFT_BLACK);
    }

    if(glu_value != 0){
        textSprite.drawString(String(glu_value), 80, 160, 7);
    } else{
        textSprite.setTextColor(TFT_WHITE, TFT_BLACK);
        textSprite.drawString("--", 80, 160, 7);
    }
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
    bgSprite.pushImage(0, 0, 80, 160, app_logo);
    bgSprite.pushSprite(0,0);
    delay(2000);
}

#endif