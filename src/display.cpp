/**
Version: 0a
Tecnocoli - 10/2022
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
STM32F401- Clothes Dryer
**/
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/TomThumb.h>

#include "defines.h"
#include "io_defines.h"
#include "wifi.h"
#include "mqtt_controll.h"
#include "sounds.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_SDA, TFT_SCL, TFT_RST);

void display_begin();
void show_display();
void change_door();
void change_data();


extern bool wifi_connected;

extern float tempIn;
extern float tempOut;
extern float deltaTemp;
extern bool dryerState;
extern bool heaterState;
extern bool fanState;
extern bool dryerOn;
extern bool doorState;
extern int dryerCicle;
extern int dryerProgram;
extern bool sleeping;

extern unsigned long loopDelay_cooling;
extern unsigned long loopDelay_door;
extern unsigned long loopDelay_start;
extern unsigned long loopDelay_on;
extern unsigned long loopDelay_sleep;




void display_begin(){
    tft.initR(INITR_BLACKTAB); // Initialize ST7735R screen
    // tft.initR(INITR_GREENTAB); 
    tft.setRotation(0);
    delay(100);
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on
    tft.setFont(&FreeMonoBold9pt7b);
    // tft.setFont(&TomThumb);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(0,13);
    tft.print("Secadora");
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(0,50);
    tft.print("Init.....");
    Serial.println("teste");
}

void show_display(){
    tft.setTextWrap(true);
    tft.setTextSize(1);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST7735_YELLOW);
    tft.setCursor(3,13);
    tft.print("Secadora");
    
    tft.setCursor(100, 13);
    if(wifi_connected){
        tft.setTextSize(0);
        tft.setTextColor(ST7735_CYAN);
        tft.print("W");
        tft.setTextSize(1);
    
    } else{
        tft.setTextColor(ST7735_RED);
        tft.setTextSize(0);
        tft.print("N");
        tft.setTextSize(1);
    }
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(3,33);
    tft.print("Estado:");
    tft.setCursor(3,70);
    tft.print("Programa:");   
    tft.setCursor(3,107);
    tft.print("Ciclo:");  
    tft.setCursor(3,142);
    tft.print("Porta:"); 
  
}

void change_data(){
    if (sleeping){
            sleeping = false;
            digitalWrite(TFT_BACKLIGHT, HIGH);
            loopDelay_sleep = millis();
            play_start_sound();
    }
    show_display();
    // tft.setCursor(90,80);
    // tft.fillRect(65,20, 85,125, ST7735_BLACK);  

    tft.setTextColor(ST7735_YELLOW);
    tft.setCursor(3,48);
    if (dryerOn){
        tft.setTextColor(ST7735_RED);
        tft.print("Ligado");
    }else{
        tft.setTextColor(ST7735_YELLOW);
        tft.print("Desl.");
    }
    tft.setTextColor(ST7735_YELLOW);
    tft.setCursor(3,85);
    if (dryerProgram == 0){
        tft.print("Normal");
    }else if (dryerProgram == 1){
        tft.print("Medio");
    }else if (dryerProgram == 2){
        tft.print("Leve");
    }else if (dryerProgram == 3){
        tft.print("Aquecer");
    }
    tft.setCursor(3,122);
    if (!dryerOn){ 
        tft.print("  ---  ");
    }else{
        if(dryerProgram != 3){
            if (dryerCicle == 0){
                tft.print("Inicio");
            }else if (dryerCicle == 1){
                tft.print("Pre-Aquec.");
            }else if (dryerCicle == 2){
                tft.print("Secagem");
            }else if (dryerCicle == 3){
                tft.print("Fim Sec.");
            }else if (dryerCicle == 4){
                tft.print("Resfriam.");
            }
        }else {
            tft.print("Aquec..");
        }
    }
    // tft.setCursor(65,95);
    // tft.print(tempIn);
    tft.setCursor(3,157);
    if (doorState){
        tft.setTextColor(ST7735_CYAN);
        tft.print("Fechada");
    }else{
        tft.setTextColor(ST7735_RED);
        tft.print("Aberta");
    }
    
}  


void change_door(){
    tft.fillRect(0,147, 80,160, ST7735_BLACK);  
    tft.setCursor(3,157);
    if (doorState){
        tft.setTextColor(ST7735_CYAN);
        tft.print("Fechada");
        Serial.println("change_door close"); 
    }else{
        tft.setTextColor(ST7735_RED);
        tft.print("Aberta");
        Serial.println("change_door open"); 
    }
    Serial.println("change_door"); 
}
