/**
Version: 0a
Tecnocoli - 10/2022
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
espwroom32 - Clothes Dryer
**/
#include <Arduino.h>
#include <WiFi.h>
// #include <WiFiClient.h>

#include "wifi.h"
#include "mqtt_controll.h"
#include "defines.h"
#include "io_defines.h"
#include "display.h"
#include "input_digital_control.h"
#include "output_digital_control.h"
#include "input_analog_control.h"
#include "sounds.h"
#include "program.h"


//Dryer
float tempIn = 0;
float tempOut = 0;
float deltaTemp = 0;
bool dryerState = false;
bool heaterState = false;
bool heaterEnable = false;
bool fanState = false;
bool fanEnable = false; 
bool dryerOn = false;
bool doorState = false;
bool lastDoorState = false;
bool cooling = false;
bool sleeping = false;
int dryerCicle = 0;
int dryerProgram = 0;


String ipaddress;
long rssi;
bool led_out = false;
bool door = false;
bool wifi_connected = false;

unsigned long loopDelay = millis();
unsigned long loopDelay_analog = millis();
unsigned long loopDelay_input = millis();
unsigned long loopDelay_mqtt = millis();
unsigned long lastAvailabilityToggleAt = millis();

unsigned long loopDelay_cooling = millis();
unsigned long loopDelay_delta = millis();
unsigned long loopDelay_door = millis();
unsigned long loopDelay_door_open = millis();
unsigned long loopDelay_door_open_timeout = millis();
unsigned long loopDelay_start = millis();
unsigned long loopDelay_on = millis();
unsigned long loopDelay_sleep = millis();
unsigned long loopDelay_update = millis();


void setup() {
  Serial.begin(115200);
  wifi_begin();
  delay(1000);
  display_begin();
  Serial.println("teste");
  io_input_begin();
  Serial.println("teste1");
  analog_input_begin();
  Serial.println("teste2");
  io_output_begin();
  Serial.println("teste3");
  
  Serial.println("teste4");
  
  delay(300);
  mqtt_begin();
  delay(300);
  show_display();
  Serial.println("teste5");
  io_input_controll(); 
  delay(300);
  change_data();
  Serial.println("teste6");

  
  play_sleep_sound();
}

void loop() {
  
 if (millis() - loopDelay_mqtt > TIMER_MQTT){
    loopDelay_mqtt = millis();
    mqtt_loop();
    //io_input_controll();    
  } 

  if (millis() - loopDelay_input > TIMER_INPUT){
    loopDelay_input = millis();
    io_input_controll();    
  } 



  if ((millis() - loopDelay) > TIMER_SEND_MQTT){
    loopDelay = millis();
    checkStatus();
    if(wifi_connected){
      digitalWrite(WIFI_LED, HIGH);
    }else{
      digitalWrite(WIFI_LED, LOW);
      wifi_begin();
      delay(100);
      
    }
    mqtt_update();  
    
  }

  if (((millis() - loopDelay_sleep) > TIMER_SLEEP) && !sleeping){
    if ((!dryerState) || (!doorState)){
      digitalWrite(TFT_BACKLIGHT, LOW);
      sleeping = true;
      Serial.println("sleeping");  
    }else{
      digitalWrite(TFT_BACKLIGHT, HIGH);  
      loopDelay_sleep = millis();
      sleeping = false;
      Serial.println("not sleeping");  
    }  
   }  

   if ((millis() - loopDelay_analog) > TIMER_ANALOG){
      Serial.println("analog");  
      loopDelay_analog = millis(); 
      read_analog();
      if (dryerOn){
        program_controll();
      }
  }  

  if ((millis() - loopDelay_update) > TIMER_UPDATE){
      Serial.println("update");  
      // change_data();
      loopDelay_update = millis();
  }  


    

}